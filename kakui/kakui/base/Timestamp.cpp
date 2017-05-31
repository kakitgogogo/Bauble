#include "Timestamp.h"

#include <sys/time.h>
#include <stdio.h>
#include <inttypes.h>
#include <boost/static_assert.hpp>

using namespace kakui;

BOOST_STATIC_ASSERT(sizeof(Timestamp) == sizeof(int64_t));

string Timestamp::toString() const
{
	char buf[32] = {0};
	int64_t seconds = _microSecondsSinceEpoch / kMicroSecondsPerSecond;
	int64_t microseconds = _microSecondsSinceEpoch % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
	return buf;
}

string Timestamp::toFormattedString(bool showMicroseconds) const
{
	char buf[32] = {0};
	time_t seconds = static_cast<time_t>(_microSecondsSinceEpoch / kMicroSecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds, &tm_time);

	if(showMicroseconds)
	{
		int microseconds = static_cast<int>(_microSecondsSinceEpoch % kMicroSecondsPerSecond);
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
				microseconds);
	}
	else
	{
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	}
	return buf;
}

Timestamp Timestamp::now()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int64_t seconds = tv.tv_sec;
	return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}