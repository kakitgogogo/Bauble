#include "Thread.h"
#include "CurrentThread.h"
#include "Exception.h"
#include "Logging.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/weak_ptr.hpp>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace kakui
{

namespace CurrentThread
{

__thread int tCachedTid = 0;
__thread char tTidString[32];
__thread int tTidStringLength = 6;
__thread const char* tThreadName = "unknown";

const bool sameType = boost::is_same<int, pid_t>::value;
BOOST_STATIC_ASSERT(sameType);

}

namespace detail
{

pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork()
{
	kakui::CurrentThread::tCachedTid = 0;
	kakui::CurrentThread::tThreadName = "main";
	CurrentThread::tid();
}

class ThreadNameInitializer
{
public:
	ThreadNameInitializer()
	{
		kakui::CurrentThread::tThreadName = "main";
		CurrentThread::tid();
		pthread_atfork(NULL, NULL, &afterFork);
	}
};

ThreadNameInitializer init;

struct ThreadData
{
	typedef kakui::Thread::ThreadFunc ThreadFunc;
	ThreadFunc _func;
	string _name;
	boost::weak_ptr<pid_t> _wkTid;

	ThreadData(const ThreadFunc& func, const string& name, const boost::shared_ptr<pid_t>& tid): _func(func), _name(name), _wkTid(tid) {}

	void runThread()
	{
		pid_t tid = kakui::CurrentThread::tid();

		boost::shared_ptr<pid_t> ptid = _wkTid.lock();
		if(ptid)
		{
			*ptid = tid;
			ptid.reset();
		}

		kakui::CurrentThread::tThreadName = _name.empty() ? "kakuiThread" : _name.c_str();
		::prctl(PR_SET_NAME, kakui::CurrentThread::tThreadName);
		try
		{
			_func();
			kakui::CurrentThread::tThreadName = "finished";
		}
		catch(const Exception& ex)
		{
			kakui::CurrentThread::tThreadName = "crashed";		
			fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
			abort();
		}
		catch(const std::exception& ex)
		{
			kakui::CurrentThread::tThreadName = "crashed";		
			fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			abort();
		}
		catch(...)
		{
			kakui::CurrentThread::tThreadName = "crashed";		
			fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
			throw;
		}
	}
};

void* startThread(void* obj)
{
	ThreadData* data = static_cast<ThreadData*>(obj);
	data->runThread();
	delete data;
	return NULL;
}

}

}

using namespace kakui;

void CurrentThread::cacheTid()
{
	if(tCachedTid == 0)
	{
		tCachedTid = detail::gettid();
		tTidStringLength = snprintf(tTidString, sizeof(tTidString), "%5d", tCachedTid);
	}

}

bool CurrentThread::isMainThread()
{
	return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec)
{
	struct timespec ts = { 0, 0 };
	ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
	::nanosleep(&ts, NULL);
}

AtomicInt32 Thread::_numCreated;

Thread::Thread(const ThreadFunc& func, const string& n):
	_started(false),
	_joined(false),
	_pthreadId(0),
	_tid(new pid_t(0)),
	_func(func),
	_name(n)
{
	setDefaultName();
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
Thread::Thread(ThreadFunc&& func, const string& n):
	_started(false),
	_joined(false),
	_pthreadId(0),
	_tid(new pid_t(0)),
	_func(func),
	_name(n)
{
	setDefaultName();
}
#endif

Thread::~Thread()
{
	if(_started && !_joined)
	{
		pthread_detach(_pthreadId);
	}
}

void Thread::setDefaultName()
{
	int num = _numCreated.incrementAndGet();
	if(_name.empty())
	{
		char buf[32];
		snprintf(buf, sizeof(buf), "Thread%d", num);
		_name = buf;
	}
}

void Thread::start()
{
	assert(!_started);
	_started = true;

	detail::ThreadData* data = new detail::ThreadData(_func, _name, _tid);
	if(pthread_create(&_pthreadId, NULL, &detail::startThread, data))
	{
		_started = false;
		delete data;
		LOG_SYSFATAL << "fail in pthread_create";
	}
}

int Thread::join()
{
	assert(_started);
	assert(!_joined);
	_joined = true;
	return pthread_join(_pthreadId, NULL);
}