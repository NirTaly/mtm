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

// Commitee member list - PQPriority
typedef struct MemPrio_t
{
	unsigned int event_count;
	int uid;
}MemPrio;

// Commitee member list - PQElement, 
// also part of event element
struct Information_t
{
	char* name;
	int uid;
	unsigned int event_count;
};
typedef struct Information_t* Info;

// events list - PQElement

typedef struct Event_t
{
	Info info;
	Date date;
	PriorityQueue mem_list;
}Event;

// THE EVENT MANAGER!!!
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
static void destroyEvent(Event event);
static Info infoFromUID(EventManager em, int member_id);
static void printEvent(EventManager em, Event* event, FILE* file);
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
		em = NULL;
	}
}

/******************************************************************************/
EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
	Event event;
	struct Information_t info;
	info.name = event_name;
	info.uid = event_id;
	info.event_count = 0;
	event.info = &info;
	
	EventManagerResult create_ret = createEvent(em, event_name, date, event_id, &event);
	if (em && EM_SUCCESS == create_ret)
	{
		EventManagerResult can_insert_ret = canInsert(em,event);
		if (EM_SUCCESS == can_insert_ret)
		{
			PriorityQueueResult insert_ret = pqInsert(em->events,&event,date);
			if (PQ_OUT_OF_MEMORY == insert_ret)
			{
				destroyEventManager(em);
			}
			destroyEvent(event);
			return (insert_ret);
		}
		destroyEvent(event);
		return (can_insert_ret);
	}
	return (!em ? EM_NULL_ARGUMENT : create_ret);
}
/******************************************************************************/
EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
	if (em && event_name)
	{
		if (days >= 0)
		{
			Date date = dateCopy(em->sys_date);

			while (days--)
			{
				dateTick(date);
			}
			
			EventManagerResult retval = emAddEventByDate(em,event_name,date,event_id);
			dateDestroy(date);
			
			return (retval);
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
		Event* event = eventFromUID(em, event_id);
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
				Event cmp_event;
				struct Information_t info = {NULL,event_id,0};
				cmp_event.info = &info;
				if (pqContains(em->events,&cmp_event))
				{
					Event* event = eventFromUID(em, event_id);
					if (!eventExistInDate(em, event, new_date))
					{
						EventManagerResult retval = EM_OUT_OF_MEMORY;
						Event* cpy_event = copyEventElem(event);
						if (cpy_event)
						{
							dateDestroy(cpy_event->date);
							cpy_event->date = dateCopy(new_date);
							if (!cpy_event->date)
							{
								freeEventElem(cpy_event);							
								destroyEventManager(em);
								return (EM_OUT_OF_MEMORY);
							}
							retval = pqChangePriority(em->events,cpy_event,event->date,new_date);
							freeEventElem(cpy_event);
							
							return (retval);
						}

						return (EM_OUT_OF_MEMORY);
					}
					
					return (EM_EVENT_ALREADY_EXISTS);
				}
				
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
			struct Information_t cmp_info = {member_name, member_id, 0};

			if (!pqContains(em->mem_list, &cmp_info))
			{
				int event_count = 0;
				MemPrio prio = {event_count, member_id};

				return (pqInsert(em->mem_list, &cmp_info, &prio));
			}
			
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
			Event* event = eventFromUID(em, event_id);
			if (event)
			{
				struct Information_t cmp_info;
				cmp_info.uid = member_id;
				if (pqContains(em->mem_list, &cmp_info))
				{
					if (!pqContains(event->mem_list,&member_id))
					{
						Info member_info = infoFromUID(em, member_id);
						Info cpy_mem = copyInfoElem(member_info);
						if (!cpy_mem)
						{
							return (EM_OUT_OF_MEMORY);
						}

						MemPrio new_prio = {member_info->event_count + 1, member_id};
						MemPrio old_prio = {member_info->event_count, member_id};
						
						cpy_mem->event_count += 1;

						if (PQ_SUCCESS == pqInsert(event->mem_list, &member_id, &member_id) &&
							PQ_SUCCESS == pqChangePriority(em->mem_list,cpy_mem,&old_prio,&new_prio))
						{
							freeInfoElem(cpy_mem);
							return (EM_SUCCESS);
						}
						freeInfoElem(cpy_mem);
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
				struct Information_t cmp_info;
				cmp_info.uid = member_id;	
				if (pqContains(em->mem_list, &cmp_info))
				{
					if (pqContains(event->mem_list,&member_id))
					{
						Info member_info = infoFromUID(em, member_id);
						Info cpy_mem = copyInfoElem(member_info);
						if (!cpy_mem)
						{
							return (EM_OUT_OF_MEMORY);
						}
						
						MemPrio new_prio = {member_info->event_count - 1, member_id};
						MemPrio old_prio = {member_info->event_count, member_id};
						
						cpy_mem->event_count -= 1;
						int* cpy_mem_id = copyIDElem(&member_id);
						if (!cpy_mem_id)
						{
							return (EM_OUT_OF_MEMORY);
						}

						bool op_success =  (PQ_SUCCESS == pqRemoveElement(event->mem_list, cpy_mem_id) &&
								PQ_SUCCESS == pqChangePriority(em->mem_list,cpy_mem,&old_prio,&new_prio));
						
						freeIDElem(cpy_mem_id);
						freeInfoElem(cpy_mem);
						
						return (op_success ? EM_SUCCESS : EM_OUT_OF_MEMORY);
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
			if (mem_info->event_count > 0)
			{
				fprintf(file,"%s,%d\n", mem_info->name, mem_info->event_count);
			}
		}	
	}

	fclose(file);
}

/******************************************************************************/
// write line of event details into file
static void printEvent(EventManager em, Event* event, FILE* file)
{
	fprintf(file, "%s,",event->info->name);
	printDate(event->date, file);
	printMembers(em, event->mem_list, file);
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
		fprintf(file, ",%s",mem_info->name);
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
	PQ_FOREACH(Info, info_runner, em->mem_list)
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
	
	while (runner && 0 >= date_cmp_ret)
	{
		date_cmp_ret = dateCompare(runner->date, date);
		if (0 == date_cmp_ret && 0 == strcmp(runner->info->name, event->info->name))
		{
			return (true);
		}
		
		runner = pqGetNext(em->events);
	}

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
		if (!eventExistInDate(em, &event, event.date))
		{
			if (!pqContains(em->events,&event))
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
	if (em && event_name && date)
	{
		if (0 <= dateCompare(date,em->sys_date))
		{
			if (event_id >= 0)
			{
				event->mem_list = pqCreate(copyIDElem,freeIDElem,isEqualIDElem,
											copyIDElem,freeIDElem,compareIDPrio);	
				event->date = dateCopy(date);
				if (!event->date || !event->mem_list)
				{
					pqDestroy(event->mem_list);
					dateDestroy(event->date);

					return (EM_OUT_OF_MEMORY);
				}
				
				return (EM_SUCCESS);
			}
			return (EM_INVALID_EVENT_ID);
		}
		return (EM_INVALID_DATE);
	}
	return (EM_NULL_ARGUMENT);	
}

static void destroyEvent(Event event)
{
	pqDestroy(event.mem_list);
	dateDestroy(event.date);
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
		info->name = malloc(strlen(cpy_info->name) + 1);
		if (!info->name)
		{
			return (NULL);
		}
		strcpy(info->name, cpy_info->name);
	}
	
	return (info);
}

void freeInfoElem(PQElement elem)
{
	Info info = (Info)elem;
	free(info->name);
	free(info);
}

bool equalInfoElem(PQElement elem1, PQElement elem2)
{
	Info info1 = (Info)elem1;
	Info info2 = (Info)elem2;

	return (info1->uid == info2->uid);
}

PQElementPriority copyMemPrio(PQElementPriority priority)
{
	MemPrio* prio = malloc(sizeof(struct MemPrio_t));
	if (prio)
	{
		prio->event_count = ((MemPrio*)(priority))->event_count;
		prio->uid = ((MemPrio*)(priority))->uid;
	}

	return (prio);
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

	free(event);
}
bool equalEventElem(PQElement event1, PQElement event2)
{
	int uid1 = ((Event*)event1)->info->uid;
	int uid2 = ((Event*)event2)->info->uid;

	return (uid1 == uid2);
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