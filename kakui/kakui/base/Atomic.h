#pragma once

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace kakui
{

namespace detail
{

template<typename T>
class AtomicInteger: boost::noncopyable
{
public:
	AtomicInteger(): _value(0) {}

	T get()
	{
		return __atomic_load_n(&_value, __ATOMIC_SEQ_CST);
	}

	T getAndAdd(T x)
	{
		return __atomic_fetch_add(&_value, x, __ATOMIC_SEQ_CST);
	}

	T addAndGet(T x)
	{
		return getAndAdd(x) + x;
	}

	T incrementAndGet()
	{
		return addAndGet(1);
	}

	T decrementAndGet()
	{
		return addAndGet(-1);
	}

	void add(T x)
	{
		getAndAdd(x);
	}

	void increment()
	{
		incrementAndGet();
	}

	void decrement()
	{
		decrementAndGet();;
	}

	T getAndSet(T newValue)
	{
		return __sync_lock_test_and_set(&_value);
	}

private:
	volatile T _value;
};

}

typedef detail::AtomicInteger<int32_t> AtomicInt32;
typedef detail::AtomicInteger<int64_t> AtomicInt64;

}