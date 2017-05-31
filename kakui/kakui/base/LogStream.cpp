#include "LogStream.h"

#include <algorithm>
#include <limits>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wtype-limits"

namespace kakui
{

namespace detail
{

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
BOOST_STATIC_ASSERT(sizeof(digits) == 20);

const char digitsHex[] = "0123456789ABCDEF";
BOOST_STATIC_ASSERT(sizeof digitsHex == 17);

template<typename T>
size_t convert(char buf[], T value)
{
	T i = value;
	char* p = buf;

	do
	{
		int lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	} while(i != 0);

	if(value < 0)
	{
		*p++ = '-';
	}
	*p = '\0';
	std::reverse(buf, p);

	return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
	uintptr_t i = value;
	char* p = buf;

	do
	{
		int lsd = static_cast<int>(i % 16);
		i /= 16;
		*p++ = digitsHex[lsd];
	} while(i != 0);

	*p = '\0';
	std::reverse(buf, p);

	return p - buf;
}

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;

template<int SIZE>
const char* FixedBuffer<SIZE>::debugString()
{
	*_cur = '\0';
	return _data;
}

template<int SIZE>
void FixedBuffer<SIZE>::cookieStart()
{
}

template<int SIZE>
void FixedBuffer<SIZE>::cookieEnd()
{
}

}


void LogStream::staticCheck()
{
	BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10);
	BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10);
	BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10);
	BOOST_STATIC_ASSERT(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10);
}

template<typename T>
void LogStream::formatInteger(T v)
{
	if(_buffer.avail() >= kMaxNumericSize)
	{
		size_t len = detail::convert(_buffer.current(), v);
		_buffer.add(len);
	}
}

LogStream& LogStream::operator<<(short v)
{
	*this << static_cast<int>(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
	*this << static_cast<unsigned int>(v);
	return *this;
}

LogStream& LogStream::operator<<(int v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(const void* p)
{
	uintptr_t v = reinterpret_cast<uintptr_t>(p);
	if(_buffer.avail() >= kMaxNumericSize)
	{
		char* buf = _buffer.current();
		buf[0] = '0';
		buf[1] = 'x';
		size_t len = detail::convertHex(buf+2, v);
		_buffer.add(len+2);
	}
	return *this;
}

LogStream& LogStream::operator<<(double v)
{
	if(_buffer.avail() >= kMaxNumericSize)
	{
		int len = snprintf(_buffer.current(), kMaxNumericSize, "%.12g", v);
		_buffer.add(len);
	}
	return *this;
}

template<typename T>
Format::Format(const char* fmt, T val)
{
	BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value == true);

	_len = snprintf(_buf, sizeof(_buf), fmt, val);
	assert(static_cast<size_t>(_len) < sizeof(_buf));
}

template Format::Format(const char* fmt, char);

template Format::Format(const char* fmt, short);
template Format::Format(const char* fmt, unsigned short);
template Format::Format(const char* fmt, int);
template Format::Format(const char* fmt, unsigned int);
template Format::Format(const char* fmt, long);
template Format::Format(const char* fmt, unsigned long);
template Format::Format(const char* fmt, long long);
template Format::Format(const char* fmt, unsigned long long);

template Format::Format(const char* fmt, float);
template Format::Format(const char* fmt, double);

}