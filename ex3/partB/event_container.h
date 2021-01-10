#ifndef __EVENT_CONTAINER_H__
#define __EVENT_CONTAINER_H__

#include "../partA/exceptions.h"
#include "base_event.h"
#include "pq.h"

namespace mtm
{
	struct EventCompare
	{
		bool operator()(const BaseEvent* a, const BaseEvent* b) const
		{
			return (a->date() < b->date());
		}
	};

	class EventContainer
	{
	public:
		class EventIterator
		{
		public:
			EventIterator(PriorityQueue<BaseEvent*, EventCompare>::Node* iter = nullptr) : m_iter(iter) { }
			~EventIterator() = default;
			EventIterator(const EventIterator& other);
			EventIterator& operator=(const EventIterator&);

			BaseEvent& operator*();
			EventIterator operator++();
			bool operator==(const EventIterator& other) const;
			bool operator!=(const EventIterator& other) const;

		private:
			class BaseEventWrap
			{
			public:
				BaseEventWrap(const BaseEvent& event) : m_event(event.clone()) { }
				~BaseEventWrap() {
					delete m_event;
				}
			private:
				BaseEvent* m_event;	
			}
			
			PriorityQueue<BaseEvent*, EventCompare>::Node* m_iter;
		};
	/***************************************************************************/
		EventContainer() = default;
	
		virtual ~EventContainer() = 0;

		EventContainer(const EventContainer&) = delete;
		EventContainer& operator=(const EventContainer&) = delete;
		
		virtual void add (const BaseEvent& event);
	
		EventIterator begin();
		EventIterator end();

	
	private:
		PriorityQueue <BaseEvent*, EventCompare> m_event_pq;
		EventIterator m_event_iter;
	};

	
} // namespace mtm

#endif     /* __EVENT_CONTAINER_H__ */
