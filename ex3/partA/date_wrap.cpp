/*******************************************************************************
 * File name:   date_wrap.h
 * Developer:   Roee Latzres, Nir Taly	
 * Version:		0 
 * Description: 
 ******************************************************************************/

#include "date_wrap.h"

extern "C" 
{
	#include "date.h"
}

namespace mtm
{
	DateWrap::DateWrap(int day, int month, int year): 
			m_day(day), m_month(month), m_year(year)
	{
		if (!(1 <= day <= 30 && 1 <= month <= 12))
		{
			throw InvalidDate();
		}
	}

	DateWrap::DateWrap(const DateWrap& other):
			m_day(other.day()), m_month(other.month()), m_year(other.year()) {}	// handle exception?

	DateWrap& DateWrap::operator=(const DateWrap& other) 						// handle exception
	{
		m_day = other.day();
		m_month = other.month();
		m_year = other.year();

		return (*this);
	}	

	int DateWrap::day() const
	{
		return (m_day);
	}
	
	int DateWrap::month() const
	{
		return (m_month);
	}
	
	int DateWrap::year() const
	{
		return (m_year);
	}

	const DateWrap operator+(const DateWrap& date, int days) throw(NegativeDays)
	{
		if (days < 0)
		{
			throw NegativeDays();
		}
		// BAD SOLUTION
		DateWrap tmp(date);
		while (days--)
		{
			tmp++;
		}
	}
	const DateWrap operator+(int days, const DateWrap& date) throw(NegativeDays)
	{
		return (date + days);
	}

	DateWrap& DateWrap::operator+=(int days) throw(NegativeDays)
	{
		*this = *this + days;

		return (*this);
	}

	DateWrap& DateWrap::operator++(int days)
	{
		if (dateIsLegal(m_day + 1, m_month))
		{
			m_day += 1;
		}
		else if (dateIsLegal(1, m_month + 1))
		{
			m_day = 1;
			m_month += 1;
		}
		else
		{
			m_day = 1;
			m_month = 1;
			m_year += 1;
		}

		return (*this);
	}

	bool DateWrap::operator==(const DateWrap& other) const
	{
		return (m_day == other.day() && m_month == other.month() && m_year == other.year());
	}
	bool DateWrap::operator!=(const DateWrap& other) const
	{
		return !(*this == other);
	}
	bool DateWrap::operator>(const DateWrap& other)  const
	{
		if (m_year == other.year())
		{
			if (m_month == other.month())
			{
				return (m_day > other.day());
			}
			
			return (m_month > other.month());
		}
		return (m_year > other.year());
	}
	bool DateWrap::operator<(const DateWrap& other)  const
	{
		return !(*this > other || *this == other);
	}

	std::ostream& operator<<(std::ostream& os, const DateWrap& date)
	{
		return (os << date.day() << "/" << date.month() << "/" << date.year() << std::endl);
	}
	
	static bool dateIsLegal(int day, int month)
	{
		return (1 <= month && month <= 12 && 1 <= day && day <= 30);
	}
} // namespace mtm
