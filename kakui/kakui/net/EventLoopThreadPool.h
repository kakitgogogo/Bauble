#pragma once

#include "base/Condition.h"
#include "base/Mutex.h"
#include "base/Thread.h"

#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace kakui
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool: boost::noncopyable
{
public:
	EventLoopThreadPool(EventLoop* baseLoop);
	~EventLoopThreadPool();
	void setThreadNum(int num) { _numThreads = num; }
	void start();
	EventLoop* getNextLoop();

private:
	EventLoop* _baseLoop;
	bool _started;
	int _numThreads;
	int _next;
	boost::ptr_vector<EventLoopThread> _threads;
	std::vector<EventLoop*> _loops;
};

}
