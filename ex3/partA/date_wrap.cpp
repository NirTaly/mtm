/*******************************************************************************
 * File name:   date_wrap.h
 * Developer:   Roee Latzres, Nir Taly	
 * Version:		0 
 * Description: 
 ******************************************************************************/

#include "date_wrap.h"

namespace mtm
{
	Date DateWrap::create(int day, int month, int year)
	{
		Date date = dateCreate(day, month, year);
		if (!date)
		{
			if (dateIsLegal(day, month))
			{
				throw (std::bad_alloc());
			}
			
			throw (InvalidDate());
		}

		return (date);
	}

	DateWrap::DateWrap(int day, int month, int year):
			m_date(create(day, month, year))
	{	}

	DateWrap::DateWrap(const DateWrap& other):
			m_date(create(other.day(), other.month(), other.year()))
	{	}	

	void DateWrap::swap(DateWrap& other)
	{
		Date tmp = m_date;

		m_date = other.m_date;

		other.m_date = tmp;
	}

	DateWrap& DateWrap::operator=(const DateWrap& other)
	{
		DateWrap tmp(other);

		swap(tmp);

		return (*this);
	}	

	DateWrap::~DateWrap()
	{
		dateDestroy(m_date);
		m_date = NULL;
	}

	int DateWrap::day() const
	{
		int day, month, year;
		dateGet(m_date, &day, &month, &year);

		return (day);
	}
	
	int DateWrap::month() const
	{
		int day, month, year;
		dateGet(m_date, &day, &month, &year);

		return (month);
	}
	
	int DateWrap::year() const
	{
		int day, month, year;
		dateGet(m_date, &day, &month, &year);

		return (year);
	}

	const DateWrap operator+(const DateWrap& date, int days)
	{
		if (days < 0)
		{
			throw NegativeDays();
		}

		DateWrap tmp(date);
		while (days--)
		{
			tmp++;
		}

		return tmp;
	}
	const DateWrap operator+(int days, const DateWrap& date)
	{
		return (date + days);
	}

	DateWrap& DateWrap::operator+=(int days)
	{
		*this = *this + days;

		return (*this);
	}

	DateWrap DateWrap::operator++(int)
	{
		DateWrap tmp(*this); 

		dateTick(m_date);

		return (tmp); 
	}

	bool DateWrap::operator==(const DateWrap& other) const
	{
		return (0 == dateCompare(m_date, other.m_date));
	}

	bool DateWrap::operator!=(const DateWrap& other) const
	{
		return !(*this == other);
	}
	bool DateWrap::operator>(const DateWrap& other)  const
	{
		return  (0 < dateCompare(m_date, other.m_date));
	}
	bool DateWrap::operator<(const DateWrap& other)  const
	{
		return (0 > dateCompare(m_date, other.m_date));
	}
	bool DateWrap::operator<=(const DateWrap& other)  const
	{
		return !(*this > other);
	}
	bool DateWrap::operator>=(const DateWrap& other)  const
	{
		return !(*this < other);
	}
	std::ostream& operator<<(std::ostream& os, const DateWrap& date)
	{
		return (os << date.day() << "/" << date.month() << "/" << date.year());
	}
	
	bool DateWrap::dateIsLegal(int day, int month) const
	{
		return (1 <= month && month <= 12 && 1 <= day && day <= 30);
	}
} // namespace mtm
