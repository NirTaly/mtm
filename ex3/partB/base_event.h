#ifndef __BASE_EVENT_H__
#define __BASE_EVENT_H__

#include <string>

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "dv.h"

namespace mtm
{
	class BaseEvent
	{
	public:
		BaseEvent(const DateWrap& date, const std::string& name);
		~BaseEvent() = default;
		BaseEvent(const BaseEvent& other);
		BaseEvent& operator=(const BaseEvent&) = delete;

		virtual void registerParticipant(size_t participant) throw(AlreadyRegistered, RegistrationBlocked);
		void unregisterParticipant(size_t participant) throw (NotRegistered);
		
		void printShort(std::ostream& os);
		void printLong(std::ostream& os);

		virtual BaseEvent* clone() = 0;

	private:
		DateWrap m_date;
		std::string m_name;
		PriorityQueue<size_t, std::less> m_participants;
	};

	BaseEvent::BaseEvent(const DateWrap& date, const std::string& name) :
		m_date(date), m_name(name), m_participants()
	{	}

	virtual void BaseEvent::registerParticipant(size_t participant) throw(AlreadyRegistered)
	{
		if (m_participants.isIn(participant))
		{
			throw(AlreadyRegistered());
		}

		m_participants.push(participant);
	}

	void BaseEvent::unregisterParticipant(size_t participant) throw (NotRegistered)
	{   
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
		os << m_participants;									// ????????????
	}

	BaseEvent::BaseEvent(const BaseEvent& other) : 
			m_date(other.m_date), m_name(other.m_name), m_participants(other.m_participants)	// PQ(const& PQ)
	{	}

} // namespace mtm
#endif     /* __BASE_EVENT_H__ */