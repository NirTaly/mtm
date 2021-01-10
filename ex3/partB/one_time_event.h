#ifndef __ONE_TIME_EVENT_H__
#define __ONE_TIME_EVENT_H__

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "event_container.h"
#include "base_event.h"

namespace mtm
{
    template <class EventType>
	class OneTimeEvent: public EventContainer
    {
    public:
        OneTimeEvent(const DateWrap& date,const std::string name);
		~OneTimeEvent() = default;

		void add(const BaseEvent& event) throw(NotSupported) override;
    
    private:
        DateWrap m_date;
        std::string m_name;

    };

    template <class EventType>
    OneTimeEvent<EventType>::OneTimeEvent(const DateWrap& date, const std::string name): m_date(date), m_name(name)  
    {   
        EventType event(date, name);
        EventContainer::add(event);
    }


    template <class EventType>
    void OneTimeEvent<EventType>::add(const BaseEvent& event) throw(NotSupported)
    {   
        (void) event;
        throw NotSupported();
    }
}

#endif     /* __ONE_TIME_EVENT_H__ */
