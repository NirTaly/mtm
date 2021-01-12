#ifndef __CLOSED_EVENT_H__
#define __CLOSED_EVENT_H__

#include <string>

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "base_event.h"
#include "pq.h"

namespace mtm
{
	class ClosedEvent : public BaseEvent
	{
	public:
		ClosedEvent(const DateWrap& date, const std::string& name);
		~ClosedEvent() = default;
		ClosedEvent(const ClosedEvent&);
		ClosedEvent& operator=(const ClosedEvent&) = delete;

		void registerParticipant(size_t participant) 
						throw(InvalidStudent,AlreadyRegistered, RegistrationBlocked) override;
		
		void addInvitee(size_t participant)  throw(InvalidStudent);

		ClosedEvent* clone() const override;
	
	private:
		PriorityQueue <size_t, IDCompare> m_invitee;
	};


} // namespace mtm
#endif     /* __CLOSED_EVENT_H__ */