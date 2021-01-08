#ifndef __CLOSED_EVENT_H__
#define __CLOSED_EVENT_H__

#include <string>

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "base_event.h"
#include "dv.h"

namespace mtm
{
	class ClosedEvent : public BaseEvent
	{
	public:
		ClosedEvent(const DateWrap& date, const std::string& name);
		~ClosedEvent() = default;
		ClosedEvent(const ClosedEvent&);
		ClosedEvent& operator=(const ClosedEvent&) = delete;

		void registerParticipant(size_t participant) throw(AlreadyRegiste, RegistrationBlocked) override;
		
		void addInvitee(size_t participant);

		ClosedEvent* clone() override;
	
	private:
		DynamicVector<size_t> m_invitee;
	};

	ClosedEvent::ClosedEvent(const DateWrap& date, const std::string& name) :
		BaseEvent(date, name), m_invitee()
	{	}

	ClosedEvent::ClosedEvent(const ClosedEvent& other) : BaseEvent(other), m_invitee(other.m_invitee)
	{ }

	void ClosedEvent::registerParticipant(size_t participant) throw(AlreadyRegistered, RegistrationBlocked) override
	{
		if (m_invitee[participant])
		{
			BaseEvent::registerParticipant(participant);
		}
	}

	void addInvitee(size_t participant)
	{
		if (m_invitee[participant])
		{
			throw (AlreadyInvited());
		}

		m_invitee[participant] = true;
	}

	ClosedEvent* clone() override
	{
		return new ClosedEvent(*this);
	}

} // namespace mtm
#endif     /* __CLOSED_EVENT_H__ */