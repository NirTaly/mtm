#ifndef __FESTIVAL_H__
#define __FESTIVAL_H__

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "event_container.h"

namespace mtm
{
	class Festival: public EventContainer
    {
    public:
        Festival(const DateWrap& date): m_date(date) {}
		~Festival() = default;

		void add(const BaseEvent& event) throw(DateMismatch) override
        {
            if(event.date() != m_date)
            {
                throw DateMismatch();
            }
        
		    EventContainer::add(event);
        }
    
    private:
        DateWrap m_date;
    };
}

#endif     /* __FESTIVAL_H__ */
