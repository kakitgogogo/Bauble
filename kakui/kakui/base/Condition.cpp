#include "Condition.h"

#include <errno.h>

bool kakui::Condition::waitForSeconds(double seconds)
{
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);

	const int64_t kNanoSecondsPerSecond = 1e9;
	int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

	abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

	MutexLock::UnassignGuard ug(_mutex);
	return ETIMEDOUT == pthread_cond_timedwait(&_cond, _mutex.getPthreadMutex(), &abstime);
}