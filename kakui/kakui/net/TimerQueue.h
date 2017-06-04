#pragma once

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include "base/Timestamp.h"
#include "base/Mutex.h"
#include "Callbacks.h"
#include "Channel.h"

namespace kakui
{

class EventLoop;
class Timer;
class TimerId;

class TimerQueue: boost::noncopyable
{
public:
	TimerQueue(EventLoop* loop);
	~TimerQueue();

	TimerId addTimer(const TimerCallback& cb, Timestamp when, double interval);

	void cancel(TimerId timerId);

private:
	typedef std::pair<Timestamp, Timer*> Entry;
	typedef std::set<Entry> TimerList;
	typedef std::pair<Timer*, int64_t> ActiveTimer;
	typedef std::set<ActiveTimer> ActiveTimerSet;

	void addTimerInLoop(Timer* timer);
	void cancelInLoop(TimerId timerId);

	void handleRead();

	std::vector<Entry> getExpired(Timestamp now);
	void reset(const std::vector<Entry>& expired, Timestamp now);

	bool insert(Timer* timer);

	EventLoop* _loop;
	const int _timerfd;
	Channel _timerfdChannel;
	TimerList _timers;

	bool _callingExpiredTimers;
	ActiveTimerSet _activeTimers;
	ActiveTimerSet _cancelingTimers;
};

}