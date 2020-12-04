/******************************************************************************
							Event Manager                  
                            Roee Latzres , Nir Tali
                            04.12.20  
 *****************************************************************************/
#include	<stddef.h>	// NULL
#include	<stdlib.h>  // MALLOC
#include	<string.h>	// strcmp

#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"
#include "dlist.h"

typedef PriorityQueue Members;
typedef Info Event;

typedef struct Event_t
{
	Info info;
	Date date;
}Event;

struct EventManager_t
{
	PriorityQueue events;
	Members list;
	Date sys_date;
};

EventManager createEventManager(Date date)
{
	EventManager em = malloc(sizeof(struct EventManager_t));
	if (em)
	{
		em->events = pqCreate(...);
		em->list = pqCreate(...);
		em->sys_date = dateCopy(date);

		if (!em->events || !em->list || !em->sys_date)
		{
			destroyEventManager(em);
			return (NULL);
		}
	}

	return (em);
}

void destroyEventManager(EventManager em)
{
	if (em)
	{
		pqDestroy(em->events);
		pqDestroy(em->list);
		dateDestroy(em->sys_date);
	}
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
	Event event;

	EventManagerResult create_ret = createEvent(event_name, date, event_id, &event);
	if (em && EM_SUCCESS == create_ret)
	{
		EventManagerResult can_insert_ret =  canInsert(em,event);
		if (EM_SUCCESS == can_insert_ret)
		{
			PriorityQueueResult insert_ret = pqInsert(em->events,&event,&date);
			if (PQ_OUT_OF_MEMORY == insert_ret)
			{
				destroyEventManager(em);
			}
			
			return (insert_ret);
		}

		return (can_insert_ret);
	}

	return (em ? create_ret : EM_NULL_ARGUMENT);
}

PQElement copyPQElement(PQElement elem)
{
	
}
// Check if event can be inserted to <em>
// Return suitable return value
static EventManagerResult canInsert(EventManager em,Event event)
{
	if (dateCompare(em->sys_date,event.date) <= 0)
	{
		Event* runner = pqGetFirst(em->events);
		int date_cmp_ret = dateCompare(runner->date, event.date);
		
		while (runner && 0 >= date_cmp_ret)
		{
			if (0 == date_cmp_ret)
			{
				if (0 == strcmp(runner->info.name, event.info.name))
				{
					return (EM_EVENT_ALREADY_EXISTS);
				}
				if (runner->info.uid == event.info.uid)
				{
					return (EM_EVENT_ID_ALREADY_EXISTS);
				}
			}
			runner = pqGetNext(em->events);
			if (runner)
			{
				date_cmp_ret = dateCompare(runner->date, event.date);
			}
		}
		return (EM_SUCCESS);
	}

	return (EM_INVALID_DATE);
}

static EventManagerResult createEvent(char* event_name, Date date, int event_id, Event* event)
{
	if (event_name && date)
	{
		if (0 <= dateCompare(date,em->sys_date))
		{
			if (event_id >= 0)
			{	
				event->date = copyDate(date);
				event->info.name = event_name;
				event->info.uid = event_id;

				return (EM_SUCCESS);
			}
			return (EM_INVALID_EVENT_ID);
		}
		return (EM_INVALID_DATE);
	}
	return (EM_NULL_ARGUMENT);	
}

CopyPQElement copy_element
FreePQElement free_element
EqualPQElements equal_elements
CopyPQElementPriority copy_priority
FreePQElementPriority free_priority
ComparePQElementPriorities compare_priorities