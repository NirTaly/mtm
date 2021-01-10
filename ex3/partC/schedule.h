#ifndef __SCHEDUELE_H__
#define __SCHEDUELE_H__

#include <string>
#include <functional>
#include <queue>

#include "../partA/exceptions.h"
#include "../partA/date_wrap.h"
#include "../partB/event_container.h"

namespace mtm
{
    class Schedule
    {
    public:
        Schedule();
        ~Schedule();

        Schedule(const Schedule& schedule);
        Schedule& operator=() = delete;

        addEvents(const EventContainer& event) throw( EventAlreadyExists);

        registerToEvent(const DateWrap& date, const std::string name, int student) 
            throw(AlreadyRegistered, RegistertionBlocked, EventDoesNotExit);

        unRegisterFromEvent(const DateWrap& date, const std::string name, int student)
            throw(NotRegistered, EventDoesNotExist);

        printAllEvents();

        printMonthEvents(int month, int year);

		printSomeEvents(Predicate func, bool verbose = false)
        
        printEventDetail(const std::string& name,const DateWrap& date) throw(EventDoesNotExist);

    private:
		std::priority_queue<BaseEventWrap, std::vector<BaseEventWrap>, EventCompare> m_pq;
    }   


    



}

#endif     /* __SCHEDUELE_H__ */

