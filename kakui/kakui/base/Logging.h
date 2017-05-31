#pragma once

#include "LogStream.h"
#include "Timestamp.h"

namespace kakui
{

class TimeZone;

class Logger
{
public:
	enum LogLevel
	{
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
		NUM_LOG_LEVELS,
	};

	class SourceFile
	{
	public:
		template<int N>
		inline SourceFile(const char (&arr)[N]): _data(arr), _size(N-1)
		{
			const char* slash = strrchr(_data, '/');
			if(slash)
			{
				_data = slash + 1;
				_size -= static_cast<int>(_data - arr);
			}
		}
		explicit SourceFile(const char* filename): _data(filename)
		{
			const char* slash = strrchr(_data, '/');
			if(slash)
			{
				_data = slash + 1;
			}
			_size = static_cast<int>(strlen(_data));
		}

		const char* _data;
		int _size;
	};

	Logger(SourceFile file, int line);
	Logger(SourceFile file, int line, LogLevel level);
	Logger(SourceFile file, int line, LogLevel level, const char* func);
	Logger(SourceFile file, int line, bool toAbort);

	~Logger();

	LogStream& stream() { return _impl._stream; }

	static LogLevel logLevel();
	static void setLogLevel(LogLevel level);

	typedef void (*OutputFunc)(const char* msg, int len);
	typedef void (*FlushFunc)();
	static void setOutput(OutputFunc);
	static void setFlush(FlushFunc);
	static void setTimeZone(const TimeZone& tz);

private:

	class Impl
	{
	public:
		typedef Logger::LogLevel LogLevel;
		Impl(LogLevel level, int savedErrno, const SourceFile& file, int line);
		void formatTime();
		void finish();

		Timestamp _time;
		LogStream _stream;
		LogLevel _level;
		int _line;
		SourceFile _basename;
	};

	Impl _impl;
};

extern Logger::LogLevel gLogLevel;

inline Logger::LogLevel Logger::logLevel()
{
	return gLogLevel;
}

#define LOG_TRACE if (kakui::Logger::logLevel() <= kakui::Logger::TRACE) \
  kakui::Logger(__FILE__, __LINE__, kakui::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (kakui::Logger::logLevel() <= kakui::Logger::DEBUG) \
  kakui::Logger(__FILE__, __LINE__, kakui::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (kakui::Logger::logLevel() <= kakui::Logger::INFO) \
  kakui::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN kakui::Logger(__FILE__, __LINE__, kakui::Logger::WARN).stream()
#define LOG_ERROR kakui::Logger(__FILE__, __LINE__, kakui::Logger::ERROR).stream()
#define LOG_FATAL kakui::Logger(__FILE__, __LINE__, kakui::Logger::FATAL).stream()
#define LOG_SYSERR kakui::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL kakui::Logger(__FILE__, __LINE__, true).stream()

const char* strError(int savedErrno);

#define CHECK_NOTNULL(val) \
	CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr)
{
	if(ptr == NULL)
	{
		Logger(file, line, Logger::FATAL).stream() << names;
	}
	return ptr;
}

}