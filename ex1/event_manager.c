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
#include "info.h"
#include "event.h"


// THE EVENT MANAGER
struct EventManager_t
{
	PriorityQueue events;
	PriorityQueue mem_list;
	Date sys_date;
};


/******************************************************************************/
/**
 * @brief find Event with <event_id> inside event's pq
 * @return Event if found, else NULL
 */
static Event eventFromUID(EventManager em, int event_id);
/**
 * @return if event's name is in <date>
 */
static bool eventExistInDate(EventManager em, Event event, Date date);
/**
 * @brief unlink committee members from their event, and event from its committee managers
 */
static void unlinkMembersfromEvent(EventManager em, Event event);
/**
 * @brief Create a Event object
 * 
 * @return Event in success, NULL in EM_OUT_OF_MEMORY
 */
static Event createEventEM(char* event_name, Date date, int event_id);
/**
 * @brief find Info with <member_id> inside committee pq
 * @return Info if found, else NULL
 */
static Info infoFromUID(EventManager em, int member_id);
static void printEvent(EventManager em, Event event, FILE* file);
static void printDate(Date date, FILE* file);
static void printMembers(EventManager em, PriorityQueue mem_list, FILE* file);

// PQ Commitee Member List Fucntions
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
int cmpDatePrio(PQElementPriority date1, PQElementPriority date2);

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
		em->events = pqCreate(copyEventElem,freeEventElem,equalEventElem,copyDatePrio,
								freeDatePrio, cmpDatePrio);
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
		em->events = NULL;
		em->mem_list = NULL;
		em->sys_date = NULL;
		free(em);
	}
}

/******************************************************************************/
EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
	if (em && event_name && date)
	{
		if (0 <= dateCompare(date,em->sys_date))
		{
			if (event_id >= 0)
			{
				Event event = createEventEM(event_name, date, event_id);
				if (!event)
				{
					destroyEventManager(em);
					freeEvent(event);
					return (EM_OUT_OF_MEMORY);
				}
				
				if (!eventExistInDate(em, event, date))
				{
					if (!pqContains(em->events,event))
					{
						PriorityQueueResult insert_ret = pqInsert(em->events,event,date);
						if (PQ_OUT_OF_MEMORY == insert_ret)
						{
							destroyEventManager(em);
						}
						freeEvent(event);
						return (PQ_SUCCESS == insert_ret ? EM_SUCCESS : EM_OUT_OF_MEMORY);
					}
					freeEvent(event);
					return (EM_EVENT_ID_ALREADY_EXISTS);
				}
				freeEvent(event);
				return (EM_EVENT_ALREADY_EXISTS);
			}
			return (EM_INVALID_EVENT_ID);
		}
		return (EM_INVALID_DATE);
	}
	return (EM_NULL_ARGUMENT);
}

/******************************************************************************/
EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
	if (em && event_name)
	{
		if (days >= 0)
		{
			Date date = dateCopy(em->sys_date);
			if (!date)
			{
				destroyEventManager(em);
				return (EM_OUT_OF_MEMORY);
			}
			
			while (days--)
			{
				dateTick(date);
			}
			
			EventManagerResult ret_val = emAddEventByDate(em,event_name,date,event_id);
			dateDestroy(date);
			
			return (ret_val);
		}
		return (EM_INVALID_DATE);
	}
	return (EM_NULL_ARGUMENT);
}
/******************************************************************************/

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
	if (em && 0 <= event_id)
	{
		Event event = eventFromUID(em, event_id);
		if (event)
		{
			unlinkMembersfromEvent(em,event);
			pqRemoveElement(em->events, event);

			return (EM_SUCCESS);
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
				Event tmp_event = createEventEM(NULL, new_date, event_id);
				if (!tmp_event)
				{
					destroyEventManager(em);
					return (EM_OUT_OF_MEMORY);
				}
		
				if (pqContains(em->events,tmp_event))
				{
					freeEvent(tmp_event);
					Event event = eventFromUID(em, event_id);
					if (!eventExistInDate(em, event, new_date))
					{
						Event new_event = createEvent(eventGetInfo(event),new_date,eventGetPQ(event));
						if (!new_event)
						{
							destroyEventManager(em);
							return (EM_OUT_OF_MEMORY);
						}

						Date old_date = eventGetDate(event);
						PriorityQueueResult ret_val = pqChangePriority(em->events,new_event,old_date,new_date);
						if (PQ_OUT_OF_MEMORY == ret_val)
						{
							destroyEventManager(em);
						}
						
						freeEvent(new_event);
						return (PQ_SUCCESS == ret_val ? EM_SUCCESS : EM_OUT_OF_MEMORY);
					}
					return (EM_EVENT_ALREADY_EXISTS);
				}
				freeEvent(tmp_event);
				return (EM_EVENT_ID_NOT_EXISTS);
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
			unsigned int event_count = 0;
			Info info = createInfo(member_name, member_id, event_count);
			if (!info)
			{
				destroyEventManager(em);
			}

			if (!pqContains(em->mem_list, info))
			{
				MemDetails details = createMem(event_count, member_id);
				if (!details)
				{
					freeMemDetails(details);
					freeInfo(info);
					return (EM_OUT_OF_MEMORY);
				}
				
				PriorityQueueResult insert_ret = pqInsert(em->mem_list, info, details);
				if (PQ_OUT_OF_MEMORY == insert_ret)
				{
					destroyEventManager(em);
				}
				freeMemDetails(details);
				freeInfo(info);
				return (PQ_SUCCESS == insert_ret ? EM_SUCCESS : EM_OUT_OF_MEMORY);
			}
			freeInfo(info);
			return (EM_MEMBER_ID_ALREADY_EXISTS);
		}
		return (EM_INVALID_MEMBER_ID);
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
			Event event = eventFromUID(em, event_id);
			if (event)
			{
				int count_events = 0;
				Info info = createInfo(NULL, member_id, count_events);
				if (!info)
				{
					destroyEventManager(em);
					return (EM_OUT_OF_MEMORY);
				}
				
				if (pqContains(em->mem_list, info))
				{
					freeInfo(info);
					PriorityQueue mem_list = eventGetPQ(event);
					if (!pqContains(mem_list,&member_id))
					{
						Info member_info = infoFromUID(em, member_id);
						Info cpy_mem = copyInfoElem(member_info);
						if (!cpy_mem)
						{
							destroyEventManager(em);
							return (EM_OUT_OF_MEMORY);
						}

						int member_info_count = infoGetCount(cpy_mem);
						infoSetCount(cpy_mem, member_info_count + 1);
						
						MemDetails new_prio = createMem(member_info_count + 1, member_id);
						MemDetails old_prio = createMem(member_info_count, member_id);
						if (!new_prio || !old_prio)
						{
							freeMemDetails(new_prio);
							freeMemDetails(old_prio);
							destroyEventManager(em);

							return (EM_OUT_OF_MEMORY);
						}

						bool insert_ok = (PQ_SUCCESS == pqInsert(mem_list, &member_id, &member_id));
						bool change_ok = (PQ_SUCCESS == pqChangePriority(em->mem_list,cpy_mem,old_prio,new_prio));
						if (!insert_ok || !change_ok)	// if out_of_memory
						{
							destroyEventManager(em);
						}

						freeMemDetails(new_prio);
						freeMemDetails(old_prio);
						freeInfoElem(cpy_mem);
		
						return ((insert_ok && change_ok) ? EM_SUCCESS : EM_OUT_OF_MEMORY);
					}
					return (EM_EVENT_AND_MEMBER_ALREADY_LINKED);
				}
				freeInfo(info);
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
			Event event = eventFromUID(em, event_id);
			if (event)
			{
				int count_events = 0;
				Info info = createInfo(NULL, member_id, count_events);
				if (!info)
				{
					destroyEventManager(em);
					return (EM_OUT_OF_MEMORY);
				}
				
				if (pqContains(em->mem_list, info))
				{
					freeInfo(info);
					PriorityQueue event_mem_list = eventGetPQ(event);
					if (pqContains(event_mem_list,&member_id))
					{
						Info member_info = infoFromUID(em, member_id);
						Info cpy_mem = copyInfoElem(member_info);
						if (!cpy_mem)
						{
							destroyEventManager(em);
							return (EM_OUT_OF_MEMORY);
						}

						int member_info_count = infoGetCount(cpy_mem);
						infoSetCount(cpy_mem, member_info_count - 1);
						
						MemDetails new_prio = createMem(member_info_count - 1, member_id);
						MemDetails old_prio = createMem(member_info_count, member_id);
						if (!new_prio || !old_prio)
						{
							freeMemDetails(new_prio);
							freeMemDetails(old_prio);
							destroyEventManager(em);

							return (EM_OUT_OF_MEMORY);
						}
						
						pqRemoveElement(event_mem_list, &member_id);
						PriorityQueueResult ret_change = pqChangePriority(em->mem_list,cpy_mem,old_prio,new_prio);
										
						if (PQ_OUT_OF_MEMORY ==  ret_change)
						{
							destroyEventManager(em);
						}
						freeMemDetails(new_prio);
						freeMemDetails(old_prio);
						freeInfoElem(cpy_mem);
						
						return ((PQ_SUCCESS == ret_change) ? EM_SUCCESS : EM_OUT_OF_MEMORY);
					}
					return (EM_EVENT_AND_MEMBER_NOT_LINKED);
				}
				freeInfo(info);
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
			
			Event event = pqGetFirst(em->events);
			Date event_date = eventGetDate(event);

			while (event && 0 > dateCompare(event_date,em->sys_date))
			{
				unlinkMembersfromEvent(em, event);
				pqRemove(em->events);
				event = pqGetFirst(em->events);
				if (event)
				{
					event_date = eventGetDate(event);
				}
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
	return (em ? pqGetSize(em->events) : -1);
}
/******************************************************************************/

char* emGetNextEvent(EventManager em)
{
	if (em)
	{
		Event event = pqGetFirst(em->events);
		if (event)
		{
			return (eventGetName(event));
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
		PQ_FOREACH(Event, event_runner, em->events)
		{
			printEvent(em, event_runner, file);
		}
	}

	fclose(file);
}

/******************************************************************************/

void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
	FILE* file = fopen(file_name, "w+");
	if (em && file_name && file)
	{
		PQ_FOREACH(Info, mem_info, em->mem_list)
		{
			unsigned int event_count = infoGetCount(mem_info);
			if (event_count > 0)
			{
				char* mem_name = infoGetName(mem_info);
				fprintf(file,"%s,%d\n", mem_name, event_count);
			}
		}	
	}

	fclose(file);
}

/******************************************************************************/
// write line of event details into file
static void printEvent(EventManager em, Event event, FILE* file)
{
	char* event_name = eventGetName(event);
	Date event_date = eventGetDate(event);
	PriorityQueue event_mem_list = eventGetPQ(event);

	fprintf(file, "%s,",event_name);
	printDate(event_date, file);
	printMembers(em, event_mem_list, file);
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

static void printMembers(EventManager em, PriorityQueue mem_list, FILE* file)
{
	PQ_FOREACH(int*, uid, mem_list)
	{
		Info mem_info = infoFromUID(em,*uid);
		char* mem_name = infoGetName(mem_info);
		fprintf(file, ",%s",mem_name);
	}
}
/******************************************************************************/
static Event eventFromUID(EventManager em, int event_id)
{
	PQ_FOREACH(Event, event_runner, em->events)
	{
		if (eventGetUID(event_runner) == event_id)
		{
			return (event_runner);
		}
	}

	return (NULL);
}
static Info infoFromUID(EventManager em, int member_id)
{
	PQ_FOREACH(Info, info_runner, em->mem_list)
	{
		if (infoGetUID(info_runner) == member_id)
		{
			return (info_runner);
		}
	}

	return (NULL);
}
static bool eventExistInDate(EventManager em, Event event, Date date)
{
	int date_cmp_ret = 0;
	Event runner = pqGetFirst(em->events);
	
	while (runner && 0 >= date_cmp_ret)
	{
		date_cmp_ret = dateCompare(eventGetDate(runner), date);
		if (0 == date_cmp_ret && 0 == strcmp(eventGetName(runner), eventGetName(event)))
		{
			return (true);
		}
		
		runner = pqGetNext(em->events);
	}

	return (false);
}
/******************************************************************************/
static void unlinkMembersfromEvent(EventManager em, Event event)
{
	PriorityQueue mem_list = eventGetPQ(event);
	int uid = eventGetUID(event);

	int* member_uid = pqGetFirst(mem_list);
	
	while (member_uid)
	{
		emRemoveMemberFromEvent(em,*member_uid,uid);

		member_uid = pqGetFirst(mem_list);
	}
}

/******************************************************************************/

static Event createEventEM(char* event_name, Date date, int event_id)
{
	int count_events = 0;
	Info event_info = createInfo(event_name, event_id, count_events);
	PriorityQueue event_mem_list = pqCreate(copyIDElem,freeIDElem,isEqualIDElem,
									copyIDElem,freeIDElem,compareIDPrio);
	if (!event_info || !event_mem_list)
	{
		freeInfo(event_info);
		pqDestroy(event_mem_list);
		
		return (NULL);
	}

	Event event = createEvent(event_info, date, event_mem_list);
	freeInfo(event_info);
	pqDestroy(event_mem_list);
	if (!event)
	{
		return (NULL);
	}

	return (event);
}

/******************************************************************************/

/******************************************************************************
 * 	System Member List PQ Functions
 *****************************************************************************/
PQElement copyInfoElem(PQElement elem)
{
	Info cpy_info = (Info)elem;

	return (copyInfo(cpy_info));
}

void freeInfoElem(PQElement elem)
{
	Info info = (Info)elem;
	// char* name = infoGetName(info);
	// free(name);
	freeInfo(info);
}

bool equalInfoElem(PQElement elem1, PQElement elem2)
{
	Info info1 = (Info)elem1;
	Info info2 = (Info)elem2;

	return (equalInfo(info1,info2));
}

PQElementPriority copyMemPrio(PQElementPriority priority)
{
	MemDetails memdet = (MemDetails)priority;
	return (copyMemDetails(memdet));
}
void freeMemPrio(PQElementPriority priority)
{
	MemDetails memdet = (MemDetails)priority;
	freeMemDetails(memdet);
}
int compareMemPrio(PQElementPriority prio1, PQElementPriority prio2)
{
	MemDetails details1 = prio1;
	MemDetails details2 = prio2;

	return (compareMemDetails(details1,details2));
}
/******************************************************************************
 * Event's PQ Functions
 *****************************************************************************/
PQElement copyEventElem(PQElement elem)
{
	Event cpy_event = (Event)elem;
	return (copyEvent(cpy_event));
}
void freeEventElem(PQElement elem)
{
	Event event = (Event)elem;
	freeEvent(event);
}
bool equalEventElem(PQElement event1, PQElement event2)
{
	Event event_1 = (Event)event1;
	Event event_2 = (Event)event2;

	return (equalEvent(event_1,event_2));
}

PQElementPriority copyDatePrio(PQElementPriority prio)
{
	return (dateCopy((Date)prio));
}

void freeDatePrio(PQElementPriority prio)
{
	dateDestroy((Date)prio);
}

int cmpDatePrio(PQElementPriority prio1, PQElementPriority prio2)
{
	return (-dateCompare((Date)prio1, (Date)prio2));
}
/******************************************************************************
 * 	Event's Member List PQ Functions
 *****************************************************************************/
PQElement copyIDElem(PQElement element)
{
	int* uid = malloc(sizeof(int));
	*uid = *(int*)element;

	return (uid);
}
void freeIDElem(PQElement element)
{
	free(element);
}
bool isEqualIDElem(PQElement elem1,PQElement elem2)
{
	return (*(int*)elem1 == *(int*)elem2);
}
int compareIDPrio(PQElementPriority prio1, PQElementPriority prio2)
{
	return (*(int*)prio2 - *(int*)prio1);
}