/*******************************************************************************
 * File name:   date_wrap.h
 * Developer:   Roee Latzres, Nir Taly	
 * Version:		0 
 * Description: 
 ******************************************************************************/

#ifndef __DATE_WRAP_H__
#define __DATE_WRAP_H__

// #include <iostream>

extern "C" 
{
	#include "date.h"
}

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
		explicit DateWrap(int day, int month, int year) throw(InvalidDate, std::bad_alloc);
		DateWrap(const DateWrap& other) throw(InvalidDate, std::bad_alloc);
		DateWrap& operator=(const DateWrap& other) throw(InvalidDate, std::bad_alloc);
		~DateWrap();

		int day() const;
		int month() const;
		int year() const;

        DateWrap& operator+=(int days) throw(NegativeDays);
		DateWrap operator++(int);
		bool operator==(const DateWrap& other) const;
		bool operator!=(const DateWrap& other) const;
		bool operator>(const DateWrap& other)  const;
		bool operator<(const DateWrap& other)  const;
		bool operator<=(const DateWrap& other)  const;
		bool operator>=(const DateWrap& other)  const;

	private:
		Date m_date;

		bool dateIsLegal(int day, int month) const;
		Date create(int day, int month, int year);
		void swap(DateWrap& other);
	};

}	// namespace mtm

#endif     /* __DATE_WRAP_H__ */