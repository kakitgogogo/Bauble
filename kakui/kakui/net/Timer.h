#pragma once

#include <boost/noncopyable.hpp>

#include "base/Timestamp.h"
#include "base/Atomic.h"
#include "Callbacks.h"

namespace kakui
{

class Timer: boost::noncopyable
{
public:
	Timer(const TimerCallback& cb, Timestamp when, double interval):
		_callback(cb),
		_expiration(when),
		_interval(interval),
		_repeat(interval > 0.0),
		_sequence(_sNumCreated.incrementAndGet())
	{	
	}

	void run() const
	{
		_callback();
	}

	Timestamp expiration() const { return _expiration; }
	bool repeat() const { return _repeat; }
	int64_t sequence() const { return _sequence; }

	void restart(Timestamp now);

private:
	const TimerCallback _callback;
	Timestamp _expiration;
	const double _interval;
	const bool _repeat;
	const int64_t _sequence;

	static AtomicInt64 _sNumCreated;
};

}
