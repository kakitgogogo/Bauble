#include "EventLoop.h"

#include "Channel.h"
#include "Epoller.h"
#include "TimerQueue.h"
#include "base/Logging.h"

#include <boost/bind.hpp>

#include <assert.h>
#include <signal.h>
#include <sys/eventfd.h>


using namespace kakui;

__thread EventLoop* tLoopInThisThread = 0;
const int kPollTimeMs = 10000;

static int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if(evtfd < 0)
	{
		LOG_SYSERR << "Failed in eventfd";
		abort();
	}
	return evtfd;
}

class IgnoreSigPipe
{
public:
	IgnoreSigPipe()
	{
		::signal(SIGPIPE, SIG_IGN);
	}
};

IgnoreSigPipe initObj;

EventLoop::EventLoop():
	_looping(false),
	_quit(false),
	_callingPendingFunctors(false),
	_threadId(CurrentThread::tid()),
	_poller(new Epoller(this)),
	_timerQueue(new TimerQueue(this)),
	_wakeupFd(createEventfd()),
	_wakeupChannel(new Channel(this, _wakeupFd))
{
	LOG_TRACE << "EventLoop created " << this << " in thread " << _threadId;
	if (tLoopInThisThread)
	{
		LOG_FATAL << "Another EventLoop " << tLoopInThisThread << " exists in this thread " << _threadId;
	}
	else
	{
		tLoopInThisThread = this;
	}

	_wakeupChannel->setReadCallback(boost::bind(&EventLoop::handleRead, this));
	_wakeupChannel->enableReading();
}

EventLoop::~EventLoop()
{
	assert(!_looping);
	::close(_wakeupFd);
	tLoopInThisThread = NULL;
}

void EventLoop::loop()
{
	assert(!_looping);
	assertInLoopThread();
	_looping = true;
	_quit = false;

	while(!_quit)
	{
		_activeChannels.clear();
		_pollReturnTime = _poller->poll(kPollTimeMs, &_activeChannels);
		for(ChannelList::iterator it = _activeChannels.begin(); it != _activeChannels.end(); ++it)
		{
			(*it)->handleEvent(_pollReturnTime);
		}
		doPendingFunctors();
	}

	LOG_TRACE << "EventLoop " << this << " stop looping";
	_looping = false;
}

void EventLoop::quit()
{
	_quit = true;
	if(!isInLoopThread())
	{
		wakeup();
	}
}

void EventLoop::runInLoop(const Functor& cb)
{
	if(isInLoopThread())
	{
		cb();
	}
	else
	{
		queueInLoop(cb);
	}
}

void EventLoop::queueInLoop(const Functor& cb)
{
	{
		MutexLockGuard lock(_mutex);
		_pendingFunctors.push_back(cb);
	}

	if(!isInLoopThread() || _callingPendingFunctors)
	{
		if(!isInLoopThread())
			LOG_INFO << "should be executed in loop thread"; 
		wakeup();
	}
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
	return _timerQueue->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), delay));
	return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), interval));
	return _timerQueue->addTimer(cb, time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
	return _timerQueue->cancel(timerId);
}

void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	_poller->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	_poller->removeChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
	LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
				<< " was created in _threadId = " << _threadId
				<< ", current thread id = " <<  CurrentThread::tid();
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(_wakeupFd, &one, sizeof(one));

	if(n != sizeof(one))
	{
		LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(_wakeupFd, &one, sizeof(one));

	if(n != sizeof(one))
	{
		LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	_callingPendingFunctors = true;

	{
		MutexLockGuard lock(_mutex);
		functors.swap(_pendingFunctors);
	}

	for(size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}

	_callingPendingFunctors = false;
}