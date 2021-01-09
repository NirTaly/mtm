#ifndef __ONE_TIME_EVENT_H__
#define __ONE_TIME_EVENT_H__

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "event_container.h"
#include "base_event.h"

namespace mtm
{

	class Festival: public EventContainer
    {
    public:
        Festival(const DateWrap& date);
		~Festival() = default;

		void add(const BaseEvent& event) throw(DateMismatch) override;
    
    private:
        DateWrap m_date;
    };

    Festival::Festival(const DateWrap& date): m_date(date)  {   }

    void Festival::add(const BaseEvent& event) throw(DateMismatch)
    {   
        if(event.date() != m_date)
		{
            throw DateMismatch();
		}
        
		EventContainer::add(event);
    }

}

#endif     /* __ONE_TIME_EVENT_H__ */
