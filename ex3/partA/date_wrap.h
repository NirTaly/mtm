/*******************************************************************************
 * File name:   date_wrap.h
 * Developer:   Roee Latzres, Nir Taly	
 * Version:		0 
 * Description: 
 ******************************************************************************/

#ifndef __DATE_WRAP_H__
#define __DATE_WRAP_H__

#include <iostream>

#include "exceptions.h"

namespace mtm
{
	class DateWrap;

	const DateWrap operator+(int days, const DateWrap& date) throw(NegativeDays);
	const DateWrap operator+(const DateWrap& date, int days) throw(NegativeDays);
	std::ostream& operator<<(std::ostream& os, const DateWrap& date);

	class DateWrap
	{
	public:
		explicit DateWrap(int day, int month, int year) throw(InvalidDate);
		DateWrap(const DateWrap& other);
		DateWrap& operator=(const DateWrap& other);
		~DateWrap() = default;

		int day() const;
		int month() const;
		int year() const;

        DateWrap& operator+=(int days) throw(NegativeDays);
		DateWrap& operator++(int days);
		bool operator==(const DateWrap& other) const;
		bool operator!=(const DateWrap& other) const;
		bool operator>(const DateWrap& other)  const;
		bool operator<(const DateWrap& other)  const;

	private:
		int m_day;
		int m_month;
		int m_year;
	};

}	// namespace mtm

#endif     /* __DATE_WRAP_H__ */