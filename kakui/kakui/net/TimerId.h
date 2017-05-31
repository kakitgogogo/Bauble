#pragma once

namespace kakui
{

class Timer;

class TimerId
{
public:
	TimerId(Timer* timer = NULL, int64_t seq = 0): _timer(timer), _sequence(seq) {}

	friend class TimerQueue;

private:
	Timer* _timer;
	int64_t _sequence;
};

}