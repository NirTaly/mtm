#ifndef __EVENT_CONTAINER_H__
#define __EVENT_CONTAINER_H__

#include "../partA/exceptions.h"
#include "base_event.h"
#include "pq.h"

namespace mtm
{
	struct BaseEventWrap
	{
		BaseEventWrap(const BaseEvent& event) : m_event(event.clone()) { }
		BaseEventWrap(const BaseEventWrap& other) : m_event(other.m_event->clone()) { }
		~BaseEventWrap()
		{
			delete m_event;
		}
		bool operator==(const BaseEventWrap& other)
		{
			return ((m_event->date() == other.m_event->date()) && 
					(m_event->name().compare(other.m_event->name()) == 0));
		}
		BaseEvent* m_event;	
	};

	struct EventCompare
	{
		bool operator()(const BaseEventWrap& a, const BaseEventWrap& b) const
		{
			if (a.m_event->date() == b.m_event->date())
			{
				return ((a.m_event->name()).compare(b.m_event->name()) < 0);
			}
			
			return (a.m_event->date() < b.m_event->date());
		}
	};

	class EventContainer
	{
	public:
		class EventIterator
		{
		public:
			EventIterator(PriorityQueue<BaseEventWrap, EventCompare>::Node* iter = nullptr) : m_iter(iter) { }
			~EventIterator() = default;
			EventIterator(const EventIterator& other);
			EventIterator& operator=(const EventIterator&);

			BaseEvent& operator*();
			EventIterator operator++();
			bool operator==(const EventIterator& other) const;
			bool operator!=(const EventIterator& other) const;

		private:
			PriorityQueue<BaseEventWrap, EventCompare>::Node* m_iter;
		};
	/***************************************************************************/
		EventContainer() = default;
	
		virtual ~EventContainer() = 0;

		EventContainer(const EventContainer&) = delete;
		EventContainer& operator=(const EventContainer&) = delete;
		
		virtual void add (const BaseEvent& event);
	
		EventIterator begin() const;
		EventIterator end() const;

	
	private:
		PriorityQueue <BaseEventWrap, EventCompare> m_event_pq;
		EventIterator m_event_iter;
	};

	
} // namespace mtm

#endif     /* __EVENT_CONTAINER_H__ */
