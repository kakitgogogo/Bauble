#pragma once

#include <stdint.h>

namespace kakui
{

namespace CurrentThread
{

extern __thread int tCachedTid;
extern __thread char tTidString[32];
extern __thread int tTidStringLength;
extern __thread const char* tThreadName;

void cacheTid();

inline int tid()
{
	if(__builtin_expect(tCachedTid == 0, 0))
	{
		cacheTid();
	}
	return tCachedTid;
}

inline const char* tidString()
{
	return tTidString;
}

inline int tidStringLength()
{
	return tTidStringLength;
}

inline const char* name()
{
	return tThreadName;
}

bool isMainThread();

void sleepUsec(int64_t usec);

}

}