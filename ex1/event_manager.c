/******************************************************************************
							Event Manager                  
                            Roee Latzres , Nir Tali
                            04.12.20  
 *****************************************************************************/
#include <stdio.h>	// fopen	
#include <stddef.h>	// NULL
#include <stdlib.h> // MALLOC
#include <string.h>	// strcmp

#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"

typedef struct MemPrio_t
{
	unsigned int event_count;
	int uid;
}MemPrio;

struct Information_t
{
	char* name;
	int uid;
	unsigned int event_count;
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
static Event* eventFromUID(EventManager em, int event_id);
static bool eventExistInDate(EventManager em, Event* event, Date date);
static void unlinkMembersfromEvent(EventManager em, Event* event);
static EventManagerResult canInsert(EventManager em,Event event);
static EventManagerResult createEvent(EventManager em, char* event_name, Date date, int event_id, Event* event);
static Info infoFromUID(EventManager em, int member_id);
static void printEvent(Event* event, FILE* file);
static void printDate(Date date, FILE* file);
static void printMembers(PriorityQueue mem_list, FILE* file);

// PQ System Member List Fucntions
PQElement copyInfoElem(PQElement elem);
void freeInfoElem(PQElement elem);
bool equalInfoElem(PQElement info1, PQElement info2);
PQElementPriority copyMemPrio(PQElementPriority priority);
void freeMemPrio(PQElementPriority priority);
int compareMemPrio(PQElementPriority prio1, PQElementPriority prio2);

// PQ Event Manager Fucntions
PQElement copyEventElem(PQElement elem);
PQElementPriority copyDatePrio(PQElementPriority date);
void freeEventElem(PQElement elem);
void freeDatePrio(PQElementPriority date);
bool equalEventElem(PQElement event1, PQElement event2);
int compareDatePrio(PQElementPriority date1, PQElementPriority date2);

// Event's Member List PQ Functions
PQElement copyIDElem(PQElement element);
PQElementPriority copyIDPrio(PQElementPriority priority);
void freeIDElem(PQElement element);
void freeIDPrio(PQElementPriority priority);
bool isEqualIDElem(PQElement elem1,PQElement elem2);
int compareIDPrio(PQElementPriority prio1, PQElementPriority prio2);

/******************************************************************************/

EventManager createEventManager(Date date)
{
	EventManager em = malloc(sizeof(struct EventManager_t));
	if (em)
	{
		em->events = pqCreate(copyEventElem,freeEventElem,equalEventElem,dateCopy,
								dateDestroy,dateCompare);
		em->mem_list = pqCreate(copyInfoElem,freeInfoElem,equalInfoElem,copyMemPrio,
								freeMemPrio, compareMemPrio);
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

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
	if (em && 0 <= event_id)
	{
		Event* event = eventFromUID(em, event_id);
		if (event)
		{
			unlinkMembersfromEvent(em,event);
			pqRemoveElement(em->events, &event_id);
		}
		
		return (EM_EVENT_NOT_EXISTS);
	}
	
	return (em ? EM_INVALID_EVENT_ID : EM_NULL_ARGUMENT);
}
/******************************************************************************/

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
	if (em && new_date)
	{
		if (0 <= dateCompare(new_date,em->sys_date))
		{
			if (event_id >= 0)
			{
				if (pqContains(em->events,event_id))
				{
					Event* event = eventFromUID(em, event_id);
					if (!eventExistInDate(em, event, new_date))
					{
						return (pqChangePriority(em->events,event,event->date,new_date));
					}
					
					return (EM_EVENT_ALREADY_EXISTS);
				}
				
				return (EM_EVENT_NOT_EXISTS);
			}
			
			return (EM_INVALID_EVENT_ID);
		}
		
		return (EM_INVALID_DATE);
	}

	return (EM_NULL_ARGUMENT);
}

/******************************************************************************/
EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
	if (em && member_name)
	{
		if (member_id >= 0)
		{
			if (!pqContains(em->mem_list, member_id))
			{
				struct Information_t info = {member_name, member_id, 0};

				MemPrio prio = {0, member_id};

				return (pqInsert(em->mem_list, &info, &prio));
			}
			
			return (EM_MEMBER_ID_ALREADY_EXISTS);
		}

		return (EM_INVALID_EVENT_ID);
	}
	
	return (EM_NULL_ARGUMENT);
}

/******************************************************************************/
EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
	if (em)
	{
		if (event_id >= 0 && member_id >= 0)
		{
			Event* event = eventFromUID(em, event_id);
			if (event)
			{
				if (pqContains(em->mem_list, member_id))
				{
					if (!pqContains(event->mem_list,member_id))
					{
						Info member_info = infoFromUID(em, member_id);
						MemPrio new_prio = {member_info->event_count + 1, member_id};
						MemPrio old_prio = {member_info->event_count, member_id};
						
						member_info->event_count += 1;

						if (PQ_SUCCESS == pqInsert(event->mem_list, member_id, member_id) &&
							PQ_SUCCESS == pqChangePriority(em->mem_list,member_info,&old_prio,&new_prio))
						{
							return (EM_SUCCESS);
						}

						return (EM_OUT_OF_MEMORY);
					}
					
					return (EM_EVENT_AND_MEMBER_ALREADY_LINKED);
				}
				
				return (EM_MEMBER_ID_NOT_EXISTS);
			}
			return (EM_EVENT_ID_NOT_EXISTS);
		}

		return (event_id < 0 ? EM_INVALID_EVENT_ID : EM_INVALID_MEMBER_ID);
	}
	
	return (EM_NULL_ARGUMENT);
}

/******************************************************************************/
EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id)
{
	if (em)
	{
		if (event_id >= 0 && member_id >= 0)
		{
			Event* event = eventFromUID(em, event_id);
			if (event)
			{
				if (pqContains(em->mem_list, member_id))
				{
					if (pqContains(event->mem_list,member_id))
					{
						Info member_info = infoFromUID(em, member_id);
						MemPrio new_prio = {member_info->event_count - 1, member_id};
						MemPrio old_prio = {member_info->event_count, member_id};
						
						member_info->event_count -= 1;

						if (PQ_SUCCESS == pqRemoveElement(event->mem_list, member_id) &&
							PQ_SUCCESS == pqChangePriority(em->mem_list,member_info,&old_prio,&new_prio))
						{
							return (EM_SUCCESS);
						}

						return (EM_OUT_OF_MEMORY);
					}
					
					return (EM_EVENT_AND_MEMBER_NOT_LINKED);
				}
			
				return (EM_MEMBER_ID_NOT_EXISTS);
			}
			
			return (EM_EVENT_ID_NOT_EXISTS);
		}

		return (event_id < 0 ? EM_INVALID_EVENT_ID : EM_INVALID_MEMBER_ID);
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
				unlinkMembersfromEvent(em, event);
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
void emPrintAllEvents(EventManager em, const char* file_name)
{
	FILE* file = fopen(file_name, "w+");
	if (em && file_name && file)
	{
		PQ_FOREACH(Event*, event_runner, em->events)
		{
			printEvent(event_runner, file);
		}
	}
}

/******************************************************************************/

void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
	FILE* file = fopen(file_name, "w+");
	if (em && file_name && file)
	{
		PQ_FOREACH(Info, mem_info, em->sys_date)
		{
			fprintf(file,"%s,%d\n", mem_info->name, mem_info->event_count);
		}	
	}
}

/******************************************************************************/
// write line of event details into file
static void printEvent(Event* event, FILE* file)
{
	fprintf(file, "%s,",event->info->name);
	printDate(event->date, file);
	printMembers(event->mem_list, file);
	fprintf(file,"\n");
}

/******************************************************************************/
static void printDate(Date date, FILE* file)
{
	int day, month, year;
	dateGet(date, &day, &month, &year);

	fprintf(file,"%d.%d.%d", day, month, year);
}
/******************************************************************************/

static void printMembers(PriorityQueue mem_list, FILE* file)
{
	PQ_FOREACH(int, uid, mem_list)
	{
		fprintf(file, ",%d",uid);
	}
}
/******************************************************************************/
// find Event* connected to event_id
// return Event* if found, else NULL
static Event* eventFromUID(EventManager em, int event_id)
{
	PQ_FOREACH(Event*, event_runner, em->events)
	{
		if (event_runner->info->uid == event_id)
		{
			return (event_runner);
		}
	}

	return (NULL);
}
// find Info connected to member_id
// return Info if found, else NULL
static Info infoFromUID(EventManager em, int member_id)
{
	PQ_FOREACH(Info, info_runner, em->sys_date)
	{
		if (info_runner->uid == member_id)
		{
			return (info_runner);
		}
	}

	return (NULL);
}
// return true if event's name is already in current date, else return false
static bool eventExistInDate(EventManager em, Event* event, Date date)
{
	int date_cmp_ret = 0;
	Event* runner = pqGetFirst(em->events);
	do
	{
		if (runner)
		{
			date_cmp_ret = dateCompare(runner->date, date);
			if (0 == date_cmp_ret)
			{
				if (0 == strcmp(runner->info->name, event->info->name))
				{
					return (true);
				}
			}
		}
		runner = pqGetNext(em->events);
	}
	while (runner && 0 >= date_cmp_ret);

	return (false);
}
/******************************************************************************/
// unlink members from event
static void unlinkMembersfromEvent(EventManager em, Event* event)
{
	int* member_uid = pqGetFirst(event->mem_list);
	while (member_uid)
	{
		emRemoveMemberFromEvent(em,*member_uid,event->info->uid);

		member_uid = pqGetFirst(event->mem_list);
	}
}

/******************************************************************************/
// Check if event can be inserted to <em>
// Return suitable return value
static EventManagerResult canInsert(EventManager em,Event event)
{
	if (dateCompare(em->sys_date,event.date) <= 0)
	{
		if (eventExistInDate(em, &event, event.date))
		{
			if (!pqContains(em->events,&(event.info->uid)))
			{
				return (EM_SUCCESS);
			}

			return (EM_EVENT_ID_ALREADY_EXISTS);
		}
		
		return (EM_EVENT_ALREADY_EXISTS);
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
				event->mem_list = pqCreate(copyIDElem,freeIDElem,isEqualIDElem,
											copyIDPrio,freeIDPrio,compareIDPrio);	
				event->date = copyDate(date);
				if (!event->date || !event->mem_list)
				{
					pqDestroy(event->mem_list);
					dateDestroy(event->date);

					return (EM_OUT_OF_MEMORY);
				}
				
				event->info->name = event_name;
				event->info->uid = event_id;
				event->info->event_count = 0;

				return (EM_SUCCESS);
			}
			return (EM_INVALID_EVENT_ID);
		}
		return (EM_INVALID_DATE);
	}
	return (EM_NULL_ARGUMENT);	
}
/******************************************************************************/

/******************************************************************************
 * 	System Member List PQ Functions
 *****************************************************************************/
PQElement copyInfoElem(PQElement elem)
{
	Info cpy_info = (Info)elem;

	Info info = malloc(sizeof(struct Information_t));
	if (info)
	{
		info->event_count = cpy_info->event_count;
		info->uid = cpy_info->uid;
		info->name = strdup(cpy_info->name);
		if (!info->name)
		{
			return (NULL);
		}
	}
	
	return (info);
}

void freeInfoElem(PQElement elem)
{
	free(((Info)(elem))->name);
	free(elem);
	elem = NULL;
}

bool equalInfoElem(PQElement elem1, PQElement elem2)
{
	Info info1 = (Info)elem1;
	Info info2 = (Info)elem2;

	return (info1->uid == info1->uid);
}

PQElementPriority copyMemPrio(PQElementPriority priority)
{
	MemPrio* prio = malloc(sizeof(struct MemPrio_t));
	if (prio)
	{
		prio->event_count = ((MemPrio*)(priority))->event_count;
		prio->uid = ((MemPrio*)(priority))->uid;
	}
}
void freeMemPrio(PQElementPriority priority)
{
	free (priority);
}
int compareMemPrio(PQElementPriority prio1, PQElementPriority prio2)
{
	MemPrio* lhs = prio1;
	MemPrio* rhs = prio2;

	int first_order = (lhs->event_count - rhs->event_count);

	if (0 == first_order)
	{
		return (rhs->uid - lhs->uid);
	}
	
	return (first_order);
}
/******************************************************************************
 * Event's PQ Functions
 *****************************************************************************/
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
void freeEventElem(PQElement elem)
{
	Event* event = (Event*)elem;

	freeInfoElem(event->info);
	dateDestroy(event->date);
	pqDestroy(event->mem_list);

}
bool equalEventElem(PQElement uid1, PQElement uid2)
{
	return (*(int*)uid1 == *(int*)uid2);
}

/******************************************************************************
 * 	Event's Member List PQ Functions
 *****************************************************************************/
PQElement copyIDElem(PQElement element)
{
	return (element);
}
PQElementPriority copyIDPrio(PQElementPriority priority)
{
	return (priority);
}
void freeIDElem(PQElement element)
{
	(void)element;
}
void freeIDPrio(PQElementPriority priority)
{
	(void)priority;
}
bool isEqualIDElem(PQElement elem1,PQElement elem2)
{
	// return (*(int*)elem1 == *(int*)elem2);
	return (elem1 == elem2);
}
int compareIDPrio(PQElementPriority prio1, PQElementPriority prio2)
{
	// return (*(int*)prio1 - *(int*)prio2);
	return (prio1 - prio2);
}