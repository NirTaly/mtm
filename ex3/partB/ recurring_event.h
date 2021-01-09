#ifndef __RECURRING_EVENT_H__
#define __RECURRING_EVENT_H__

#include "ex3/partA/date_wrap.h"
#include "ex3/partA/exceptions.h"
#include "event_container.h"
#include "base_event.h"

#include <iostream>

namespace mtm
{
	template <class EventType>
	class RecurringEvent: public EventContainer
    {
    public:
        RecurringEvent(const DateWrap& first_date, int num_occurences, int interval_days) throw (InvalidNumber, InvalidInterval);
		~RecurringEvent() = default;

		void add(const BaseEvent& event) throw(DateMismatch) override;
    
    private:
        DateWrap m_first_date;
        int m_num_occurences;
        int m_interval_days;
    };
	
    template <class EventType>
    RecurringEvent<EventType>::RecurringEvent(const DateWrap& first_date, int num_occurences, int interval_days)
        throw (InvalidNumber, InvalidInterval):
        m_first_date(first_date), m_num_occurences(num_occurences), m_interval_days(interval_days)
    {   
		if (m_num_occurences <= 0)
		{
		    throw InvalidNumber();
		}
		
		if (m_interval_days <= 0)
		{
			throw InvalidInterval();
		}
	}

	template <class EventType>
    void RecurringEvent<EventType>::add(const BaseEvent& event) throw(DateMismatch)	//waiting for piazza
    {   
		std::is_same(EventType, BaseEvent)
    }



}

#endif     /* __RECURRING_EVENT_H__ */
