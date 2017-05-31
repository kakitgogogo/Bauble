#pragma once

#include "base/Condition.h"
#include "base/Mutex.h"
#include "base/Thread.h"

#include <boost/noncopyable.hpp>

namespace kakui
{

class EventLoop;

class EventLoopThread: boost::noncopyable
{
public:
	EventLoopThread();
	~EventLoopThread();
	EventLoop* startLoop();

private:
	void threadFunc();

	EventLoop* _loop;
	bool _exiting;
	Thread _thread;
	MutexLock _mutex;
	Condition _cond;
};

}