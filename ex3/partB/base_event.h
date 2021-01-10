#ifndef __BASE_EVENT_H__
#define __BASE_EVENT_H__

#include <string>

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "pq.h"

namespace mtm
{
	class BaseEvent
	{
	public:
		BaseEvent(const DateWrap& date, const std::string& name);
		virtual ~BaseEvent() = default;
		BaseEvent(const BaseEvent& other);
		BaseEvent& operator=(const BaseEvent&) = delete;

		virtual void registerParticipant(size_t participant) throw(AlreadyRegistered, RegistrationBlocked);
		void unregisterParticipant(size_t participant) throw (NotRegistered);
		
		void printShort(std::ostream& os);
		void printLong(std::ostream& os);

		virtual BaseEvent* clone() const = 0;

		DateWrap date() const;
		std::string name() const;
		
		struct IDCompare
		{
			bool operator()(size_t a, size_t b) const
			{
				return (a < b);
			}
		};
	private:
		DateWrap m_date;
		std::string m_name;

		PriorityQueue <size_t, IDCompare> m_participants;
	};

} // namespace mtm

#endif     /* __BASE_EVENT_H__ */