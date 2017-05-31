#pragma once

#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace kakui
{

class Condition: boost::noncopyable
{
public:
	explicit Condition(MutexLock& mutex):_mutex(mutex)
	{
		CHECK(pthread_cond_init(&_cond, NULL));
	}

	~Condition()
	{
		CHECK(pthread_cond_destroy(&_cond));
	}

	void wait()
	{
		MutexLock::UnassignGuard ug(_mutex);
		CHECK(pthread_cond_wait(&_cond, _mutex.getPthreadMutex()));
	}

	bool waitForSeconds(double seconds);

	void notify()
	{
		CHECK(pthread_cond_signal(&_cond));
	}

	void notifyAll()
	{
		CHECK(pthread_cond_broadcast(&_cond));
	}

private:
	MutexLock& _mutex;
	pthread_cond_t _cond;
};

}