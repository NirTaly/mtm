#ifndef __RECURRING_EVENT_H__
#define __RECURRING_EVENT_H__

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "event_container.h"
#include "base_event.h"

#include <iostream>

namespace mtm
{
	template <class EventType>
	class RecurringEvent: public EventContainer
    {
    public:
        RecurringEvent(const DateWrap& first_date, const std::string&, int num_occurences, int interval_days);
		~RecurringEvent() = default;

		void add(const BaseEvent& event) override;
    
    private:
        DateWrap m_first_date;
        std::string m_name;
        int m_num_occurences;
        int m_interval_days;

    };
	
    template <class EventType>
    RecurringEvent<EventType>::RecurringEvent
            (const DateWrap& first_date,const std::string& name, int num_occurences, int interval_days):
        m_first_date(first_date), m_name(name), m_num_occurences(num_occurences), m_interval_days(interval_days)
    {   
		if (m_num_occurences <= 0)
		{
		    throw InvalidNumber();
		}
		
		if (m_interval_days <= 0)
		{
			throw InvalidInterval();
		}

        DateWrap date(first_date);
        for (int i = 0; i < m_num_occurences; i++)
        {
            EventType event(date, name);
            EventContainer::add(event);
            
            date += m_interval_days;
        }
	}

	template <class EventType>
    void RecurringEvent<EventType>::add(const BaseEvent& event)
    {
        (void) event;
        throw NotSupported();
    }
}

#endif     /* __RECURRING_EVENT_H__ */
