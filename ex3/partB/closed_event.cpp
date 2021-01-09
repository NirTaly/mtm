#include "closed_event.h"

namespace mtm
{
	ClosedEvent::ClosedEvent(const DateWrap& date, const std::string& name) :
		BaseEvent(date, name), m_invitee()
	{	}

	ClosedEvent::ClosedEvent(const ClosedEvent& other) : BaseEvent(other), m_invitee(other.m_invitee)
	{ }

	void ClosedEvent::registerParticipant(size_t participant) throw(AlreadyRegistered, RegistrationBlocked)
	{
		if (m_invitee[participant])
		{
			BaseEvent::registerParticipant(participant);
		}
	}

	void ClosedEvent::addInvitee(size_t participant)
	{
		if (m_invitee[participant])
		{
			throw (AlreadyInvited());
		}

		m_invitee[participant] = true;
	}

	ClosedEvent* ClosedEvent::clone()
	{
		return new ClosedEvent(*this);
	}

} // namespace mtm