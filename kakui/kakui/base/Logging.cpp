#include "Logging.h"
#include "CurrentThread.h"
#include "Timestamp.h"
#include "TimeZone.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

namespace kakui
{

__thread char tErrorBuf[512];
__thread char tTime[32];
__thread time_t tLastSecond;

const char* strError(int savedErrno)
{
	return strerror_r(savedErrno, tErrorBuf, sizeof(tErrorBuf));
}

Logger::LogLevel initLogLevel()
{
	if(::getenv("KAKUI_LOG_TRACE"))
		return Logger::TRACE;
	else if(::getenv("KAKUI_LOG_DEBUG"))
		return Logger::DEBUG;
	else
		return Logger::INFO;
}

Logger::LogLevel gLogLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
	" TRACE ",
	" DEBUG ",
	" INFO  ",
	" WARN  ",
	" ERROR ",
	" FATAL ",
};

class T
{
public:
	T(const char* str, unsigned len): _str(str), _len(len)
	{
		assert(strlen(_str) == _len);
	}

	const char* _str;
	const unsigned _len;
};

inline LogStream& operator<<(LogStream& s, T v)
{
	s.append(v._str, v._len);
	return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
	s.append(v._data, v._size);
	return s;
}

void defaultOutput(const char* msg, int len)
{
	size_t n = fwrite(msg, 1, len, stdout);
}

void defaultFlush()
{
	fflush(stdout);
}

Logger::OutputFunc gOutput = defaultOutput;
Logger::FlushFunc gFlush = defaultFlush;
TimeZone gLogTimeZone;

}

using namespace kakui;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line): 
	_time(Timestamp::now()),
	_stream(),
	_level(level),
	_line(line),
	_basename(file)
{
	formatTime();
	CurrentThread::tid();
	_stream << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
	_stream << T(LogLevelName[level], 7);
	if(savedErrno != 0)
	{
		_stream << strError(savedErrno) << " (errno=" << savedErrno << ")";
	}
}

void Logger::Impl::formatTime()
{
	int64_t microSecondsSinceEpoch = _time.microSecondsSinceEpoch();
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
	int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
	if(seconds != tLastSecond)
	{
		tLastSecond = seconds;
		struct tm tm_time;
		if(gLogTimeZone.valid())
		{
			tm_time = gLogTimeZone.toLocalTime(seconds);
		}
		else
		{
			::gmtime_r(&seconds, &tm_time);
		}

		int len = snprintf(tTime, sizeof(tTime), "%4d%02d%02d %02d:%02d:%02d",
							tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
							tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		assert(len == 17);
	}

	if(gLogTimeZone.valid())
	{
		Format us(".%06d ", microseconds);
		assert(us.size() == 8);
		_stream << T(tTime, 17) << T(us.data(), 8);
	}
	else
	{
		Format us(".%06dZ ", microseconds);
		assert(us.size() == 9);
		_stream << T(tTime, 17) << T(us.data(), 9);
	}
}

void Logger::Impl::finish()
{
	_stream << " - " << _basename << ":" << _line << '\n';
}

Logger::Logger(SourceFile file, int line): _impl(INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func): _impl(level, 0, file, line)
{
	_impl._stream << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level): _impl(level, 0, file, line) {}

Logger::Logger(SourceFile file, int line, bool toAbort): _impl(toAbort?FATAL:ERROR, errno, file, line) {}

Logger::~Logger()
{
	_impl.finish();
	const LogStream::Buffer& buf(stream().buffer());
	gOutput(buf.data(), buf.size());
	if(_impl._level == FATAL)
	{
		gFlush();
		abort();
	}
}

void Logger::setLogLevel(Logger::LogLevel level)
{
	gLogLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
	gOutput = out;
}

void Logger::setFlush(FlushFunc flush)
{
	gFlush = flush;
}

void Logger::setTimeZone(const TimeZone& tz)
{
	gLogTimeZone = tz;
}