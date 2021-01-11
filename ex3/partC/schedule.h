#ifndef __SCHEDUELE_H__
#define __SCHEDUELE_H__

#include <string>
#include <functional>

#include "../partA/exceptions.h"
#include "../partA/date_wrap.h"
#include "../partB/event_container.h"

namespace mtm
{
    class Schedule
    {
    public:
        Schedule() = default;
        ~Schedule() = default;
        
        Schedule(const Schedule& schedule) = delete;
        Schedule& operator=(const Schedule&) = delete;

        void addEvents(const EventContainer& event_container) throw(EventAlreadyExists);

        void registerToEvent(const DateWrap& date, const std::string name, int student) 
            throw(AlreadyRegistered, RegistrationBlocked, EventDoesNotExist);

        void unregisterFromEvent(const DateWrap& date, const std::string name, int student)
            throw(NotRegistered, EventDoesNotExist);

        void printAllEvents() const;

        void printMonthEvents(int month, int year) const;

		void printSomeEvents(std::function<bool(const BaseEvent&)> predicate, bool verbose = false) const;
        
        void printEventDetails(const DateWrap& date, const std::string& name) const throw(EventDoesNotExist);


    private:
        // void foreach(std::function <void (PriorityQueue<BaseEventWrap,EventCompare>::Node*)>);

		PriorityQueue <BaseEventWrap, EventCompare> m_schedule;
    };
}

#endif     /* __SCHEDUELE_H__ */