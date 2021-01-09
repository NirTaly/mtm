#ifndef __EVENT_CONTAINER_H__
#define __EVENT_CONTAINER_H__


namespace mtm
{
	#include "../partA/exceptions.h"
	#include "base_event.h"
	#include "pq.h"
	
	class EventColntainer
	{
	public:
		class EventIterator;

		EventContainer() = default;
		virtual ~EventContainer() = 0;

		virtual void add(const BaseEvent& event);
	
		EventIterator begin() const;
		EventIterator end() const;

	
	private:
		struct EventCompare
		{
			bool operator()(const BaseEvent& a, const BaseEvent& b) const
			{
				if (a.date() < b.date())
					return a < b;
			}
		};
		
		PriorityQueue <BaseEvent, EventCompare> m_event_pq;
		EventIterator m_event_iter;
	};

	class EventContainer::EventIterator
	{
		EventIterator() : m_iter(nullptr) { }
		~EventIterator() = default;
		EventIterator(const EventIterator& other);
		EventIterator& operator=(const EventIterator&);

		BaseEvent& operator*();
		EventIterator operator++();
		bool operator==(const EventIterator& other) const;
		bool operator!=(const EventIterator& other) const;

	private:
		PriorityQueue<BaseEvent, EventCompare>::Node* m_iter;
	};
	
/*******************************************************************************/

	EventContainer::~EventContainer()	{ }

	virtual void EventContainer::add(const BaseEvent& event)
	{
		m_event_pq.push(event);
	}

	EventContainer::EventIterator EventContainer::begin() const
	{
		return EventIterator(m_event_pq.begin());
	}
	
	EventContainer::EventIterator EventContainer::end() const
	{
		return EventIterator(m_event_pq.end());
	}

/*******************************************************************************/

	EventContainer::EventIterator(const EventContainer::EventIterator& other) : m_iter(other.m_iter) {	}

	EventContainer::EventIterator& EventIterator::operator=(const EventContainer::EventIterator& other)
	{
		m_iter = other.m_iter;
	}
	
	BaseEvent& EventIterator::operator*()
	{
		return (m_iter->m_element);
	}

	EventContainer::EventIterator EventContainer::EventIterator::operator++()
	{
		m_iter = m_iter->m_next;
		return *this;
	}

	bool EventContainer::EventIterator::operator==(const EventContainer::EventIterator& other)  const
	{
		return (m_iter == other.m_iter);
	}
	
	bool EventContainer::EventIterator::operator!=(const EventContainer::EventIterator& other)  const
	{
		return (m_iter != other.m_iter);
	}
	
} // namespace mtm

#endif     /* __EVENT_CONTAINER_H__ */
