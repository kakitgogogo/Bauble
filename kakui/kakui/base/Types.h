#pragma once

#include <string>

namespace kakui
{
using std::string;

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
	return f;
}

template<typename To, typename From>
inline To down_cast(From* f)
{
	return static_cast<To>(f);
}

}