#pragma once

#include "Condition.h"
#include "Mutex.h"

#include <boost/noncopyable.hpp>

namespace kakui
{

class CountDownLatch: boost::noncopyable
{
public:
	explicit CountDownLatch(int count);

	void wait();

	void countDown();

	int getCount() const;

private:
	mutable MutexLock _mutex;
	Condition _condition;
	int _count;
};

}