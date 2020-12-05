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

struct Information_t
{
	char* name;
	int uid;
};
typedef struct Information_t* Info;

typedef struct Event_t
{
	Info info;
	Date date;
	PriorityQueue mem_list;
}Event;

struct EventManager_t
{
	PriorityQueue events;
	PriorityQueue mem_list;
	Date sys_date;
};


/******************************************************************************/
// PQ System Member List Fucntions
PQElement copyInfoElem(PQElement elem);
PQElementPriority copyEventCountPrio(PQElementPriority events_count);
void freeInfoElem(PQElement elem);
void freeEventCountPrio(PQElementPriority event_count);
bool equalInfoElem(PQElement info1, PQElement info2);
int cmpEventCountPrio(PQElementPriority count1, PQElementPriority count2);
/******************************************************************************/
// PQ Event Manager Fucntions
PQElement copyEventElem(PQElement elem);
PQElementPriority copyDatePrio(PQElementPriority date);
void freeEventElem(PQElement elem);
void freeDatePrio(PQElementPriority date);
bool equalEventElem(PQElement event1, PQElement event2);
int compareDatePrio(PQElementPriority date1, PQElementPriority date2);

/******************************************************************************/

EventManager createEventManager(Date date)
{
	EventManager em = malloc(sizeof(struct EventManager_t));
	if (em)
	{
		em->events = pqCreate(copyEventElem,freeEventElem,equalEventElem,dateCopy,
								dateDestroy,dateCompare);
		em->mem_list = pqCreate(copyInfoElem,freeInfoElem,equalInfoElem,copyEventCountPrio,
								freeEventCountPrio, cmpEventCountPrio);
		em->sys_date = dateCopy(date);

		if (!em->events || !em->mem_list || !em->sys_date)
		{
			destroyEventManager(em);
			return (NULL);
		}
	}

	return (em);
}
/******************************************************************************/

void destroyEventManager(EventManager em)
{
	if (em)
	{
		pqDestroy(em->events);
		pqDestroy(em->mem_list);
		dateDestroy(em->sys_date);
	}
}

/******************************************************************************/
EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
	Event event;

	EventManagerResult create_ret = createEvent(em, event_name, date, event_id, &event);
	if (em && EM_SUCCESS == create_ret)
	{
		EventManagerResult can_insert_ret = canInsert(em,event);
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
/******************************************************************************/
EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
	if (em && event_name && event_id)
	{
		int sys_day, sys_month, sys_year;
		dateGet(em->sys_date, &sys_day, &sys_month, &sys_year);
		
		Date date = dateCreate(sys_day + days, sys_month, sys_year);
		
		return (emAddEventByDate(em,event_name,date,event_id));
	}

	return (EM_NULL_ARGUMENT);
}



/******************************************************************************/
EventManagerResult emTick(EventManager em, int days)
{
	if (em)
	{
		if (days > 0)
		{
			while (days--)
			{
				dateTick(em->sys_date);
			}
			
			Event* event = pqGetFirst(em->events);
			while (event && 0 > dateCompare(event->date,em->sys_date))
			{
				unlinkMemberToEvent(em, event); // NEED TO LET MEMBERS KNOW THAT EVENT REMOVED
				pqRemove(em->events);
				event = pqGetFirst(em->events);
			}
			
			return (EM_SUCCESS);
		}
		
		return (EM_INVALID_DATE);
	}

	return (EM_NULL_ARGUMENT);
}

/******************************************************************************/
int emGetEventsAmount(EventManager em)
{
	if (em)
	{
		return (pqGetSize(em->events));
	}
	
	return (-1);
}
/******************************************************************************/

char* emGetNextEvent(EventManager em)
{
	if (em)
	{
		Event* event = pqGetFirst(em->events);
		if (event)
		{
			return (event->info->name);
		}
	}

	return (NULL);
}

/******************************************************************************/
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
				if (0 == strcmp(runner->info->name, event.info->name))
				{
					return (EM_EVENT_ALREADY_EXISTS);
				}
				if (runner->info->uid == event.info->uid)
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

// Create event as element events pq
// Return suitable return value
static EventManagerResult createEvent(EventManager em, char* event_name, Date date, int event_id, Event* event)
{
	if (event_name && date)
	{
		if (0 <= dateCompare(date,em->sys_date))
		{
			if (event_id >= 0)
			{
				event->mem_list = pqCreate(...............);	
				event->date = copyDate(date);
				if (!event->date || !event->mem_list)
				{
					pqDestroy(event->mem_list);
					dateDestroy(event->date);

					return (EM_OUT_OF_MEMORY);
				}
				
				event->info->name = event_name;
				event->info->uid = event_id;

				return (EM_SUCCESS);
			}
			return (EM_INVALID_EVENT_ID);
		}
		return (EM_INVALID_DATE);
	}
	return (EM_NULL_ARGUMENT);	
}

/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
PQElement copyInfoElem(PQElement elem)
{
	Info cpy_info = (Info)elem;

	Info info = malloc(sizeof(struct Information_t));
	if (info)
	{
		info->uid = cpy_info->uid;
		info->name = strdup(cpy_info->name);
		if (!info->name)
		{
			return (NULL);
		}
	}
	
	return (info);
}
PQElementPriority copyEventCountPrio(PQElementPriority events_count)
{
	return (events_count);
}

void freeInfoElem(PQElement elem)
{
	free(((Info)(elem))->name);
	free(elem);
	elem = NULL;
}

void freeEventCountPrio(PQElementPriority event_count)
{
	(void)event_count;
}
bool equalInfoElem(PQElement elem1, PQElement elem2)
{
	Info info1 = (Info)elem1;
	Info info2 = (Info)elem2;

	return (0 == strcmp(info1->name,info2->name) &&	info1->uid == info1->uid);
}
int cmpEventCountPrio(PQElementPriority count1, PQElementPriority count2)
{
	return (count1 - count2);
}

/******************************************************************************/
/******************************************************************************/
PQElement copyEventElem(PQElement elem)
{
	Event* cpy_event = (Event*)elem;
	Event* event = malloc(sizeof(struct Event_t));
	if (event)
	{
		event->info = (Info)copyInfoElem(cpy_event->info);
		event->date = dateCopy(cpy_event->date);
		event->mem_list = pqCopy(cpy_event->mem_list);

		if (!event->info || !event->date || !event->mem_list)
		{
			return (NULL);
		}
	}

	return (event);
}
// PQElementPriority copyDatePrio(PQElementPriority events_count)
// {
// 	return copyDate()
// }
void freeEventElem(PQElement elem)
{
	Event* event = (Event*)elem;

	freeInfoElem(event->info);
	dateDestroy(event->date);
	pqDestroy(event->mem_list);

}
// void freeDatePrio(PQElementPriority date)
// {
// 	dateDestroy((Date)date);
// }
bool equalEventElem(PQElement event1, PQElement event2)
{
	Info info1 = ((Event*)event1)->info;
	Info info2 = ((Event*)event2)->info;

	return (equalInfoElem(info1, info2));
}
// int compareDatePrio(PQElementPriority date1, PQElementPriority date2)
// {
// 	return (dateCompare())
// }