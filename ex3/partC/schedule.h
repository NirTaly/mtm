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

        void addEvents(const EventContainer& event_container);

        void registerToEvent(const DateWrap& date, const std::string name, size_t student);

        void unregisterFromEvent(const DateWrap& date, const std::string name, size_t student);

        void printAllEvents() const;

        void printMonthEvents(int month, int year) const;

		void printSomeEvents(std::function<bool(const BaseEvent&)> predicate, bool verbose = false) const;
        
        void printEventDetails(const DateWrap& date, const std::string& name) const;


    private:
		PriorityQueue <BaseEventWrap, EventCompare> m_schedule;
    };
}

#endif     /* __SCHEDUELE_H__ */