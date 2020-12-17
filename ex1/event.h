#ifndef EVENT_H
#define EVENT_H

#include "priority_queue.h"
#include "date.h"
#include "info.h"

typedef struct Event_t* Event;

/**
 * @brief Create a Event object
 * 
 * @param info 
 * @param date 
 * @param pq 
 * @return Event 
 */
Event createEvent(Info info, Date date, PriorityQueue pq);

/**
 * @brief copy Event object
 * 
 * @param event 
 * @return Event 
 */
Event copyEvent(Event event);

/**
 * @brief frees event 
 * 
 * @param event 
 */
void freeEvent(Event event);

/**
* identify equal elements.
* This function should return:
* 		true if they're equal;
*		false otherwise
 */
bool equalEvent(Event event1, Event event2);

/**
 * @brief return event->UID
 * 
 * @param event 
 * @return int 
 */
int eventGetUID(Event event);

/**
 * @brief set event->UID as <uid>
 * 
 * @param event 
 * @param uid 
 * @return true 
 * @return false 
 */
bool eventSetUID(Event event, int uid);


/**
 * @brief return event->info
 * 
 * @param event 
 * @return Info 
 */
Info eventGetInfo(Event event);

/**
 * @brief sets event->info as <info>
 * 
 * @param event 
 * @param info 
 * @return true 
 * @return false 
 */
bool eventSetInfo(Event event, Info info);


/**
 * @brief return event->date
 * 
 * @param event 
 * @return Date 
 */
Date eventGetDate(Event event);

/**
 * @brief sets event->date as <date>
 * 
 * @param event 
 * @param date 
 * @return true 
 * @return false 
 */
bool eventSetDate(Event event, Date date);


/**
 * @brief return event->mem_list
 * 
 * @param event 
 * @return PriorityQueue 
 */
PriorityQueue eventGetPQ(Event event);

/**
 * @brief sets event->mem_list as <pq>
 * 
 * @param event 
 * @param pq 
 * @return true 
 * @return false 
 */
bool eventSetPQ(Event event, PriorityQueue pq);

/**
 * @brief return event->name
 * 
 * @param event 
 * @return char* 
 */
char* eventGetName(Event event);

/**
 * @brief sets event->name as <name>
 * 
 * @param event 
 * @param name 
 * @return true 
 * @return false 
 */
bool eventSetName(Event event, char* name);

/**
 * @brief set all event's arguments
 * 
 * @param event 
 * @param info 
 * @param date 
 * @param pq 
 * @return true 
 * @return false 
 */
bool eventSetAll(Event event, Info info, Date date, PriorityQueue pq);

#endif //EVENT_H
