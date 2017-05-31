#pragma once

#include "Types.h"
#include <exception>

namespace kakui
{

class Exception: public std::exception
{
public:
	explicit Exception(const char* what);
	explicit Exception(const string& what);
	virtual ~Exception() throw();
	virtual const char* what() const throw();
	const char* stackTrace() const throw();

private:
	void fillStackTrace();

	string _message;
	string _stack;
};

}