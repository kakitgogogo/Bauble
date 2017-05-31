#pragma once

#include "base/Timestamp.h"
#include "base/Mutex.h"
#include "base/Thread.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace kakui
{

class Channel;
class Epoller;
class TimerQueue;

class EventLoop: boost::noncopyable
{
public:
	typedef boost::function<void()> Functor;

	EventLoop();

	~EventLoop();

	void loop();
	void quit();

	Timestamp pollReturnTime() const { return _pollReturnTime; }

	void runInLoop(const Functor& cb);
	void queueInLoop(const Functor& cb);

	TimerId runAt(const Timestamp& time, const TimerCallback& cb);
	TimerId runAfter(double delay, const TimerCallback& cb);
	TimerId runEvery(double interval, const TimerCallback& cb);

	void cancel(TimerId timerId);

	void wakeup();
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);

	void assertInLoopThread()
	{
		if (!isInLoopThread())
		{
			abortNotInLoopThread();
		}
	}

	bool isInLoopThread() const { return _threadId == CurrentThread::tid(); }

private:
	void abortNotInLoopThread();
	void handleRead();
	void doPendingFunctors();

	typedef std::vector<Channel*> ChannelList;

	bool _looping;
	bool _quit;
	bool _callingPendingFunctors;

	const pid_t _threadId;
	Timestamp _pollReturnTime;
	int _wakeupFd;

	boost::scoped_ptr<Epoller> _poller;
	boost::scoped_ptr<TimerQueue> _timerQueue;
	boost::scoped_ptr<Channel> _wakeupChannel;

	ChannelList _activeChannels;
	MutexLock _mutex;
	std::vector<Functor> _pendingFunctors;
};

}