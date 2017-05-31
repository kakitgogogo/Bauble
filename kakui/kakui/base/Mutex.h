#pragma once

#include "CurrentThread.h"

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail(	int errnum,
									const char *file,
									unsigned int line,
									const char *function)
	__THROW __attribute__ ((__noreturn__));
__END_DECLS
#endif

#define CHECK(ret) ({ __typeof__ (ret) errnum = (ret);		\
					if (__builtin_expect(errnum != 0, 0))	\
					__assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else  

#define CHECK(ret) ({ __typeof__ (ret) errnum = (ret);		\
					assert(errnum == 0); (void) errnum;})

#endif

namespace kakui 
{

class MutexLock: boost::noncopyable
{
public:
	MutexLock(): _holder(0)
	{
		CHECK(pthread_mutex_init(&_mutex, NULL));
	}

	~MutexLock()
	{
		assert(_holder == 0);
		CHECK(pthread_mutex_destroy(&_mutex));
	}

	bool isLockedByThisThread() const
	{
		return _holder == CurrentThread::tid();
	}

	void assertLocked() const
	{
		assert(isLockedByThisThread());
	}

	void lock()
	{
		CHECK(pthread_mutex_lock(&_mutex));
		assignHolder();
	}

	void unlock()
	{
		unassignHolder();
		CHECK(pthread_mutex_unlock(&_mutex));
	}

	pthread_mutex_t* getPthreadMutex()
	{
		return &_mutex;
	}

private:
	friend class Condition;

	class UnassignGuard: boost::noncopyable
	{
	public:
		UnassignGuard(MutexLock& owner): _owner(owner)
		{
			_owner.unassignHolder();
		}
		~UnassignGuard()
		{
			_owner.assignHolder();
		}
	private:
		MutexLock& _owner;
	};

	void assignHolder()
	{
		_holder = CurrentThread::tid();
	}

	void unassignHolder()
	{
		_holder = 0;
	}

	pthread_mutex_t _mutex;
	pid_t _holder;
};

class MutexLockGuard: boost::noncopyable
{
public:
	explicit MutexLockGuard(MutexLock& mutex): _mutex(mutex)
	{
		_mutex.lock();
	}

	~MutexLockGuard()
	{
		_mutex.unlock();
	}

private:
	MutexLock& _mutex;
};

}