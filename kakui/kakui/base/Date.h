#pragma once

#include "Types.h"

struct tm;

namespace kakui
{

class Date
{
public:
	struct YearMonthDay
	{
		int year;
		int month;
		int day;
	};

	static const int kDaysPerWeek = 7;
	static const int kJulianDayOf1970_01_01;

	Date(): _julianDayNumber(0) {}

	Date(int year, int month, int day);

	explicit Date(int julianDayNum): _julianDayNumber(julianDayNum) {}

	explicit Date(const struct tm&);

	void swap(Date& that)
	{
		std::swap(_julianDayNumber, that._julianDayNumber);
	}

	bool valid() const { return _julianDayNumber > 0; }

	string toIsoString() const;

	struct YearMonthDay yearMonthDay() const;

	int year() const
	{
		return yearMonthDay().year;
	}

	int month() const
	{
		return yearMonthDay().month;
	}

	int day() const
	{
		return yearMonthDay().day;
	}

	int weekDay() const
	{
		return (_julianDayNumber+1) % kDaysPerWeek;
	}

	int julianDayNumber() const { return _julianDayNumber; }

private:
	int _julianDayNumber;
};

inline bool operator<(Date x, Date y)
{
	return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator==(Date x, Date y)
{
	return x.julianDayNumber() == y.julianDayNumber();
}

}