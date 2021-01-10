#include "closed_event.h"

namespace mtm
{
	ClosedEvent::ClosedEvent(const DateWrap& date, const std::string& name) :
		BaseEvent(date, name)
	{	}

	ClosedEvent::ClosedEvent(const ClosedEvent& other) : BaseEvent(other), m_invitee(other.m_invitee)
	{ }

	void ClosedEvent::registerParticipant(size_t participant) throw(AlreadyRegistered, RegistrationBlocked)
	{
		if (m_invitee.isIn(participant))
		{
			BaseEvent::registerParticipant(participant);
		}
	}

	void ClosedEvent::addInvitee(size_t participant)
	{
		if (m_invitee.isIn(participant))
		{
			throw (AlreadyInvited());
		}

		m_invitee.push(participant);
	}

	ClosedEvent* ClosedEvent::clone() const
	{
		return new ClosedEvent(*this);
	}

} // namespace mtm