#include "event_container.h"

namespace mtm
{

	EventContainer::~EventContainer()	{ }

	void EventContainer::add(const BaseEvent& event)
	{
		m_event_pq.push(event);
	}

	EventContainer::EventIterator EventContainer::begin()
	{
		return EventContainer::EventIterator(m_event_pq.begin());
	}
	
	EventContainer::EventIterator EventContainer::end()
	{
		return EventContainer::EventIterator(m_event_pq.end());
	}

/*******************************************************************************/

	EventContainer::EventIterator::EventIterator(const EventContainer::EventIterator& other) 
		: m_iter(other.m_iter) 	{	}

	EventContainer::EventIterator& EventContainer::EventIterator::operator=
												(const EventContainer::EventIterator& other)
	{
		m_iter = other.m_iter;
		return *this;
	}
	
	BaseEvent& EventContainer::EventIterator::operator*()
	{
		return *(m_iter->m_element);
	}

	EventContainer::EventIterator EventContainer::EventIterator::operator++()
	{
		m_iter = m_iter->m_next;
		return *this;
	}

	bool EventContainer::EventIterator::operator==(const EventContainer::EventIterator& other) const
	{
		return (m_iter == other.m_iter);
	}
	
	bool EventContainer::EventIterator::operator!=(const EventContainer::EventIterator& other) const
	{
		return (m_iter != other.m_iter);
	}
	
} // namespace mtm