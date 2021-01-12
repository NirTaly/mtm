#include <iostream>

#include "base_event.h"

namespace mtm
{
	BaseEvent::BaseEvent(const DateWrap& date, const std::string& name) :
		m_date(date), m_name(name)
	{	}
	
	BaseEvent::BaseEvent(const BaseEvent& other) :
			m_date(other.m_date), m_name(other.m_name), m_participants(other.m_participants)
	{	}

	void BaseEvent::registerParticipant(size_t participant) 
								throw(InvalidStudent, AlreadyRegistered, RegistrationBlocked)
	{
		studentIsLegal(participant);

		if (m_participants.isIn(participant))
		{
			throw(AlreadyRegistered());
		}

		m_participants.push(participant);
	}

	void BaseEvent::unregisterParticipant(size_t participant) throw (InvalidStudent, NotRegistered)
	{   
		studentIsLegal(participant);
		
		if (!m_participants.isIn(participant))
		{
			throw(NotRegistered());
		}

		m_participants.remove(participant);
	}

	void BaseEvent::printShort(std::ostream& os)
	{
		os << m_name << ' ' << m_date << std::endl;
	}

	void BaseEvent::printLong(std::ostream& os)
	{
		printShort(os);
		
		m_participants.print(os);
		
	}

	DateWrap BaseEvent::date() const
	{
		return m_date;
	}

	std::string BaseEvent::name() const
	{
		return m_name;
	}

	void BaseEvent::studentIsLegal(size_t student)
	{
		if (student < 1 || student > 1234567890)
		{
			throw InvalidStudent();
		}
	}
} // namespace mtm