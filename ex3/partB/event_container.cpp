#include "event_container.h"

namespace mtm
{
	EventContainer::~EventContainer()	{ }

	void EventContainer::add(const BaseEvent& event)
	{	
		m_event_pq.push(BaseEventWrap(event));
	}
 
	EventContainer::EventIterator EventContainer::begin() const
	{
		return EventContainer::EventIterator(m_event_pq.begin());
	}
	
	EventContainer::EventIterator EventContainer::end() const
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
		return *(m_iter->m_element->m_event);
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