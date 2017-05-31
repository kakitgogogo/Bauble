#pragma once

#include "StringPiece.h"
#include "Types.h"
#include <assert.h>
#include <string.h>
#include <string>
#include <boost/noncopyable.hpp>

namespace kakui
{

namespace detail
{

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template<int SIZE>
class FixedBuffer: boost::noncopyable
{
public:
	FixedBuffer(): _cur(_data) 
	{
		setCookie(cookieStart);
	}

	~FixedBuffer()
	{
		setCookie(cookieEnd);
	}

	void append(const char* buf, size_t len)
	{
		if(implicit_cast<size_t>(avail()) > len)
		{
			memcpy(_cur, buf, len);
			_cur += len;
		}
	}

	const char* data() const { return _data; }
	int size() const { return static_cast<int>(_cur - _data); }

	char* current() { return _cur; }
	int avail() const { return static_cast<int>(end() - _cur); }
	void add(size_t len) { _cur += len; }
	void reset() { _cur = _data; }
	void bzero() { ::bzero(_data, sizeof(_data)); }

	const char* debugString();
	void setCookie(void (*cookie)()) { _cookie = cookie; }
	string toString() const { return string(_data, size()); }
	StringPiece toStringPiece() const { return StringPiece(_data, size()); }

private:
	const char* end() const { return _data + sizeof(_data); }

	static void cookieStart();
	static void cookieEnd();

	void (*_cookie)();
	char _data[SIZE];
	char* _cur;
};

}

class LogStream: boost::noncopyable
{
public:
	typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;

	LogStream& operator<<(bool v)
	{
		_buffer.append(v ? "1" : "0", 1);
		return *this;
	}

	LogStream& operator<<(short);
	LogStream& operator<<(unsigned short);
	LogStream& operator<<(int);
	LogStream& operator<<(unsigned int);
	LogStream& operator<<(long);
	LogStream& operator<<(unsigned long);
	LogStream& operator<<(long long);
	LogStream& operator<<(unsigned long long);

	LogStream& operator<<(const void*);

	LogStream& operator<<(float v)
	{
		*this << static_cast<double>(v);
		return *this;
	}
	LogStream& operator<<(double);

	LogStream& operator<<(char v)
	{
		_buffer.append(&v, 1);
		return *this;
	}

	LogStream& operator<<(const char* str)
	{
		if(str)
		{
			_buffer.append(str, strlen(str));
		}
		else
		{
			_buffer.append("(null)", 6);
		}
		return *this;
	}

	LogStream& operator<<(const string& v)
	{
		_buffer.append(v.c_str(), v.size());
		return *this;
	}

	LogStream& operator<<(const StringPiece& v)
	{
		_buffer.append(v.data(), v.size());
		return *this;
	}

	LogStream& operator<<(const Buffer& v)
	{
		*this << v.toStringPiece();
		return *this;
	}

	void append(const char* data, int len) { _buffer.append(data, len); }
	const Buffer& buffer() const { return _buffer; }
	void resetBuffer() { _buffer.reset(); }

private:
	void staticCheck();

	template<typename T>
	void formatInteger(T);

	Buffer _buffer;

	static const int kMaxNumericSize = 32;
};

class Format
{
public:
	template<typename T>
	Format(const char* fmt, T val);

	const char* data() const { return _buf; }
	int size() const { return _len; }

private:
	char _buf[32];
	int _len;
};

inline LogStream& operator<<(LogStream& s, const Format& fmt)
{
	s.append(fmt.data(), fmt.size());
	return s;
}

}