#include <stddef.h>	// NULL
#include <stdlib.h> // MALLOC
#include <string.h>	// strcmp

#include "event.h"

struct Event_t
{
	Info info;
	Date date;
	PriorityQueue mem_list;
};

Event createEvent(Info info, Date date, PriorityQueue pq)
{
	Event event = malloc(sizeof(struct Event_t));
	if (event)
	{
		if (!eventSetAll(event, info, date, pq))
		{
			freeEvent(event);
			return (NULL);
		}
	}
	return (event);
}

Event copyEvent(Event event)
{
	return (createEvent(event->info, event->date, event->mem_list));
}

void freeEvent(Event event)
{
	if (event)
	{
		freeInfo(event->info);
		pqDestroy(event->mem_list);
		dateDestroy(event->date);

		free(event);
	}
}

bool equalEvent(Event event1, Event event2)
{
	int uid1 = infoGetUID(event1->info);
	int uid2 = infoGetUID(event2->info);

	return (uid1 == uid2);
}

Info eventGetInfo(Event event)
{
	return (event->info);
}
bool eventSetInfo(Event event, Info info)
{
	event->info = copyInfo(info);
	
	return (event->info);
}

Date eventGetDate(Event event)
{
	return (event->date);
}
bool eventSetDate(Event event, Date date)
{
	event->date = dateCopy(date);
	
	return (event->date);
}

PriorityQueue eventGetPQ(Event event)
{
	return (event->mem_list);
}
bool eventSetPQ(Event event, PriorityQueue pq)
{
	event->mem_list = pqCopy(pq);

	return event->mem_list;
}

bool eventSetAll(Event event, Info info, Date date, PriorityQueue pq)
{
	return (eventSetInfo(event, info) && eventSetDate(event, date) && eventSetPQ(event, pq));
}

int eventGetUID(Event event)
{
	return (infoGetUID(event->info));
}
bool eventSetUID(Event event, int uid)
{
	return infoSetUID(event->info, uid);
}

char* eventGetName(Event event)
{
	return (infoGetName(event->info));
}
bool eventSetName(Event event, char* name)
{
	return infoSetName(event->info, name);
}