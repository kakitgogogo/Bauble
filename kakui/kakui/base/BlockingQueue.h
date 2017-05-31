#pragma once

#include "Condition.h"
#include "Mutex.h"

#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

namespace kakui
{

template<typename T>
class BlockingQueue: boost::noncopyable
{
public:
	BlockingQueue(): _mutex(), _notEmpty(_mutex),_notFull(_mutex) , _queue() {}

	void put(const T& x)
	{
		MutexLockGuard lock(_mutex);
		while(_queue.full())
		{
			_notFull.wait();
		}
		assert(!_queue.full());
		_queue.push_back(x);
		_notEmpty.notify();
	}

	T take()
	{
		MutexLockGuard lock(_mutex);
		while(_queue.empty())
		{
			_notEmpty.wait();
		}
		assert(!_queue.empty());
		T front(_queue.front());
		_queue.pop_front();
		_notFull.notify();
		return front;
	}

	bool empty() const
	{
		MutexLockGuard lock(_mutex);
		return _queue.empty();
	}

	bool full() const
	{
		MutexLockGuard lock(_mutex);
		return _queue.full();
	}

	size_t size() const
	{
		MutexLockGuard lock(_mutex);
		return  _queue.size();
	}

	size_t capacity() const
	{
		MutexLockGuard lock(_mutex);
		return  _queue.capacity();
	}

private:
	mutable MutexLock _mutex;
	Condition _notEmpty;
	Condition _notFull;
	boost::circular_buffer<T> _queue;
};

}