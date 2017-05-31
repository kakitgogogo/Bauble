#include "Timer.h"

using namespace kakui;

AtomicInt64 Timer::_sNumCreated;

void Timer::restart(Timestamp now)
{
	if(_repeat)
	{
		_expiration = addTime(now, _interval);
	}
	else
	{
		_expiration = Timestamp::invalid();
	}
}