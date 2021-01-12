#include "schedule.h"

namespace mtm
{

	void Schedule::addEvents(const EventContainer& event_container)
	{
		for (auto iter = event_container.begin(); iter != event_container.end(); ++iter)
		{
			if (m_schedule.isIn(*iter))
			{
				throw (EventAlreadyExists());
			}
		}

		for (auto iter = event_container.begin(); iter != event_container.end(); ++iter)
		{
			m_schedule.push(*iter);
		}
	}

	void Schedule::registerToEvent(const DateWrap& date, const std::string name, size_t student) 
	{
		for (auto event_runner = m_schedule.begin(); event_runner != m_schedule.end(); 
														event_runner = event_runner->m_next)
		{
			BaseEvent* event = event_runner->m_element->m_event;
			if(event->date() == date && event->name() == name)
			{
				event->registerParticipant(student);
				return;
			}
		}

		throw EventDoesNotExist();
	}

	void Schedule::unregisterFromEvent(const DateWrap& date, const std::string name, size_t student)
	{
		for (auto event_runner = m_schedule.begin(); event_runner != m_schedule.end(); 
														event_runner = event_runner->m_next)
		{
			BaseEvent* event = event_runner->m_element->m_event;
			if(event->date() == date && event->name() == name)
			{
				event->unregisterParticipant(student);
				return;
			}
		}

		throw EventDoesNotExist();
	}

	void Schedule::printAllEvents() const
	{
		for (auto event_runner = m_schedule.begin(); event_runner != m_schedule.end(); 
														event_runner = event_runner->m_next)
		{
			BaseEvent* event = event_runner->m_element->m_event;

			event->printShort(std::cout);
			std::cout << std::endl;
		}
	}

	void Schedule::printMonthEvents(int month, int year) const
	{
		for (auto event_runner = m_schedule.begin(); event_runner != m_schedule.end(); 
														event_runner = event_runner->m_next)
		{
			BaseEvent* event = event_runner->m_element->m_event;
			if (event->date().month() == month && event->date().year() == year)
			{
				event->printShort(std::cout);
				std::cout << std::endl;
			}
		}
	}

	void Schedule::printSomeEvents(std::function<bool(const BaseEvent&)> predicate, bool verbose) const
	{
		for (auto event_runner = m_schedule.begin(); event_runner != m_schedule.end(); 
														event_runner = event_runner->m_next)
		{
			BaseEvent* event = event_runner->m_element->m_event;
			if (predicate(*event))
			{
				if (verbose)
				{
					event->printLong(std::cout);
					std::cout << std::endl;
				}
				else
				{
					event->printShort(std::cout);
					std::cout << std::endl;
				}
			}
		}
	}
	
	void Schedule::printEventDetails(const DateWrap& date, const std::string& name) const
	{
		for (auto event_runner = m_schedule.begin(); event_runner != m_schedule.end(); 
														event_runner = event_runner->m_next)
		{
			BaseEvent* event = event_runner->m_element->m_event;
			if(event->date() == date && event->name() == name)
			{
				event->printLong(std::cout);
				std::cout << std::endl;
				return;
			}
		}

		throw EventDoesNotExist();
	}
} // namespace mtm
