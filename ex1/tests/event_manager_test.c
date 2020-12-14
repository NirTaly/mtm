/******************************************************************************
                                  Priority Queue                
                                    Nir Tali
                                    22.11.18
 *****************************************************************************/
#include <stdlib.h>
/**
#include "../utils.h"

#include "event_manager.h" 
#include "priority_queue.h"

/******************************************************************************
	Tasks:
	[] NullArgs				- all functions with every NULL arg
	[] CreateDestroy		- empty EM, normal case
	[] AddEvents			- empty EM, normal case, Invalid Date,Invalid ID, Event already exist, id exist
	[] RemoveEvent			- empty EM, normal case, Invalid ID, Event Dont exist
	[] ChangeDate			- empty EM, normal case, Invalid Date,Invalid ID, Event already exist, id exist, change to same date
	[] AddMemb&ToEvent		- normal case, Invalid ID, member dont exist, invalid event ID, event dont exist, already linked
	[] RemoveMemFromEvent	- normal case, dont exists, invalid ID's, not linked
	[] Tick					- noraml case, invalid date
	[] SizeANDGetNext		- empty EM, normal cases
	[] Prints				- !
******************************************************************************/
/*****************************************************************************
 * 
 * 
 * 

*****************************************************************************/

/* Helper Functions  */

/******************************************************************************/
/* Test Declarations */
void nullArgsTest();
void CreateDestroyTest();
void AddEventsTest();
void RemoveEventTest();
void ChangeDateTest();
void AddMembANDToEventTest();
void RemoveMemFromEventTest();
void TickTest();
void SizeANDGetNextTest();
void PrintsTest();

/* Global Variables */

char* mem_name[]	= {"Member0","Member1","Member2","Member3","Member4","Member5","Member6"};
char* event_name[]	= {"Event0","Event1","Event2","Event3","Event4","Event5","Event6"};
int mem_id[]		= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int event_id[]		= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

 /*****************************************************************************
 *                                M A I N                                    *
****************************************************************************/
int main(void)
{	
	nullArgsTest();
	CreateDestroyTest();
	// AddEventsTest();
	// RemoveEventTest();
	// ChangeDateTest();
	// AddMembANDToEventTest();
	// RemoveMemFromEventTest();
	// TickTest();
	// SizeANDGetNextTest();
	// PrintsTest();
	// ClearTest();

	CHECK_VLG()

    return (0);
}

// [] NullArgs		- all functions with every NULL arg
void nullArgsTest()
{
	LOG_CYAN("NULL Args Test Begin:")
	Date date = dateCreate(14,12,1995);
	TEST_IS_TRUE(date)

	EventManager em = createEventManager(date);
	EventManager em_null = createEventManager(NULL);
	
	TEST_IS_TRUE(em)
	TEST_IS_TRUE(!em_null)
	
	TEST(1, emAddEventByDate(em_null,event_name[0],date,event_id[0]), EM_NULL_ARGUMENT, "emAddEventByDate1\t")
	TEST(2, emAddEventByDate(em,NULL,date,event_id[0]), EM_NULL_ARGUMENT, "emAddEventByDate2\t")
	TEST(3, emAddEventByDate(em,event_name[0],NULL,event_id[0]), EM_NULL_ARGUMENT, "emAddEventByDate3\t")
	TEST(4, emAddEventByDiff(em_null,event_name[0],0,event_id[0]), EM_NULL_ARGUMENT, "emAddEventByDiff1\t")
	TEST(5, emAddEventByDiff(em,NULL,0,event_id[0]), EM_NULL_ARGUMENT, "emAddEventByDiff\t")
	TEST(6,	emGetEventsAmount(em_null), -1, "emGetEventsAmount\t")
	TEST(7,	emRemoveEvent(em_null,event_id[0]), EM_NULL_ARGUMENT, "emRemoveEvent\t")
	TEST(8,	emChangeEventDate(em_null,event_id[0],date), EM_NULL_ARGUMENT, "emChangeEventDate1")
	TEST(9,	emChangeEventDate(em,event_id[0],NULL), EM_NULL_ARGUMENT, "emChangeEventDate2")
	TEST(10,emAddMember(em_null,mem_name[0],mem_id[0]), EM_NULL_ARGUMENT, "emAddMember1\t")
	TEST(11,emAddMember(em,NULL,mem_id[0]), EM_NULL_ARGUMENT, "emAddMember2\t")
	TEST(12,emAddMemberToEvent(em_null,mem_id[0],event_id[0]), EM_NULL_ARGUMENT, "emAddMemberToEvent")
	TEST(13,emRemoveMemberFromEvent(em_null,mem_id[0],event_id[0]), EM_NULL_ARGUMENT, "emRemoveMemberFromEvent")
	TEST(14,emTick(em_null,1), EM_NULL_ARGUMENT, "emTick\t\t")
	TEST(15,emGetNextEvent(em_null), NULL, "emGetNextEvent\t")

	
	destroyEventManager(em_null);
	destroyEventManager(em);
	dateDestroy(date);
	LOG_CYAN("NULL Args Test END")
}

// [] CreateDestroy	- empty EM, normal case
void CreateDestroyTest()
{
	LOG_CYAN("Create Destroy Test Begin:")

	Date date = dateCreate(14,12,1995);
	TEST_IS_TRUE(date)

	EventManager em1 = createEventManager(date);
	destroyEventManager(em1);

	EventManager em2 = createEventManager(date);

	TEST_TRUE(emAddEventByDiff(em2,event_name[0],0,event_id[0]),EM_SUCCESS)
	TEST_TRUE(emAddMember(em2,mem_name[0],mem_id[0]),EM_SUCCESS)

	destroyEventManager(em2);
	dateDestroy(date);

	LOG_CYAN("Create Destroy Test END")
}

// // [] Copy	- empty pq, normal case
// void AddEventsTest()
// {
// 	LOG_CYAN("Copy Test Begin:")

// 	EventManager em1 = createEventManager(date);

// 	EventManager empty_pq = pqCopy(em1);

// 	TEST_INT(1,pqGetSize(empty_pq), 0, "Empty PQ\t")

// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(em1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
// 	}

// 	EventManager em2 = pqCopy(em1);
// 	TEST_INT(2,arePQEqual(em1,em2), 1, "Normal Case")
	
// 	pqDestroy(em1);
// 	pqDestroy(empty_pq);
// 	pqDestroy(em2);

// 	LOG_CYAN("Copy Test End")
// }

// // [x] Size	- empty pq, normal case
// void RemoveEventTest()
// {
// 	LOG_CYAN("Size Test Begin:")
	
// 	EventManager em1 = createEventManager(date);
// 	EventManager empty_pq = createEventManager(date);

// 	TEST_INT(1,pqGetSize(empty_pq),0,"Empty PQ\t")
// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(em1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
// 	}

// 	TEST_INT(2,pqGetSize(em1),21,"Normal Case")

// 	pqDestroy(em1);
// 	pqDestroy(empty_pq);

// 	LOG_CYAN("Size Test End")
// }

// // [x] Contains	- empty pq, elem not exist, elem exist
// void ChangeDateTest()
// {
// 	LOG_CYAN("Contain Test Begin:")
	
// 	EventManager empty_pq = createEventManager(date);
// 	TEST_INT(1,pqContains(empty_pq,0),0,"Empty PQ\t")

// 	EventManager em1 = createEventManager(date);
// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(em1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
// 	}

// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqContains(em1,ELEM(i)),1)
// 	}
	
// 	EventManager em2 = createEventManager(date);
// 	for (int data = 0; data < 20; data++)
// 	{
// 		for (int prio = 0; prio < 3; prio++)
// 		{
// 			TEST_TRUE(pqInsert(em2,ELEM(data),PRIO(prio)), PQ_SUCCESS)
// 		}
// 	}
// 	TEST_INT(2,pqContains(em2,ELEM(20)),0,"Elem Not Found")
	
// 	for (int data = 0; data < 20; data++)
// 	{
// 		for (int prio = 0; prio < 3; prio++)
// 		{
// 			TEST_TRUE(pqContains(em2,ELEM(data)),1)
// 			TEST_TRUE(pqRemoveElement(em2,ELEM(data)),PQ_SUCCESS)
// 		}
// 	}

// 	TEST_INT(3,pqGetSize(em2),0,"Duplicate Elems")

// 	pqDestroy(em1);
// 	pqDestroy(em2);
// 	pqDestroy(empty_pq);

// 	LOG_CYAN("Contain Test End")

// }

// // [x] Insert - Malloc fail, normal case
// // all inserted, all inserted by priority, all inserted by priority2, malloc failed
// void AddMembANDToEventTest()
// {
// 	LOG_CYAN("Insert Test Begin:")
	
// 	EventManager em1 = createEventManager(date);
// 	for (int i = 20; i >= 0; i--)
// 	{
// 		TEST_TRUE(pqInsert(em1,ELEM(i),PRIO(20-i)), PQ_SUCCESS) 
// 	}
// 	TEST_INT(1, pqGetSize(em1), 21, "Normal Case 1")
	
// 	int j = 0;
// 	bool good_test = true;
// 	PQ_FOREACH(int* , runner, em1)
// 	{
// 		if (*runner != j)
// 		{
// 			good_test = false;
// 		}
// 		TEST_TRUE(*runner, j++)
// 	}
// 	TEST_INT(2,good_test,1,"Normal Case 2")

// 	TEST_TRUE(pqClear(em1),PQ_SUCCESS)
// 	for (int prio = 0; prio < 20; prio++)
// 	{
// 		for (int data = 0; data < 20; data++)
// 		{
// 			TEST_TRUE(pqInsert(em1,ELEM(data),PRIO(prio)), PQ_SUCCESS)
// 		}
// 	}
// 	for (int prio = 0; prio < 20; prio++)
// 	{
// 		for (int data = 0; data < 20; data++)
// 		{
// 			TEST_TRUE(pqContains(em1,ELEM(data)),1)
// 			TEST_TRUE(pqRemoveElement(em1,ELEM(data)),PQ_SUCCESS)
// 		}
// 	}
// 	TEST_INT(3,pqGetSize(em1),0,"Normal Case 3")
// 	pqDestroy(em1);


// 	EventManager bad_malloc_pq = pqCreate(COPYBADMALLOCFUNC,FREEBADMALLOCFUNC,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	
// 	TEST_INT(4,pqInsert(bad_malloc_pq,ELEM(0),PRIO(0)),PQ_OUT_OF_MEMORY,"Malloc Fail")

// 	pqDestroy(bad_malloc_pq);
	
// 	LOG_CYAN("Insert Test End")
// }

// // [x] ChangePrio - FIRST elem changed, re-insert, Malloc fail, normal case
// void RemoveMemFromEventTest()
// {
// 	LOG_CYAN("ChangePrio Test Begin:")
	
// 	EventManager em = createEventManager(date);

// 	TEST_INT(1,pqChangePriority(pq, ELEM(0),PRIO(0), PRIO(1)),PQ_ELEMENT_DOES_NOT_EXISTS,"Empty PQ\t")
// /*****************************************************************************/

// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(pq,ELEM(i),PRIO(i)), PQ_SUCCESS) 
// 	}
// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqChangePriority(pq,ELEM(i),PRIO(i),PRIO(20-i)), PQ_SUCCESS) 
// 	}
// 	TEST_INT(2,pqGetSize(pq),21,"Normal Case")
	
// 	bool good_test = true;
// 	int j = 0;
// 	PQ_FOREACH(int*, runner, pq)
// 	{
// 		if (*runner != j++)
// 		{
// 			good_test = false;
// 		}
// 	}	
// 	TEST_INT(3,good_test,1,"Flip PQ\t")
// /*****************************************************************************/
// 	pqClear(pq);

// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(pq,ELEM(0),PRIO(i)), PQ_SUCCESS) 
// 	}
	
// 	EventManager em2 = pqCopy(pq);
	
// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqChangePriority(pq,ELEM(0),PRIO(i),PRIO(20-i)), PQ_SUCCESS) 
// 	}
	
// 	TEST_INT(4,arePQEqual(pq,em2),1, "dif prio\t")
// /*****************************************************************************/
// 	pqClear(pq);

// 	TEST_TRUE(pqInsert(pq,ELEM(0),PRIO(0)), PQ_SUCCESS) 
// 	TEST_TRUE(pqChangePriority(pq,ELEM(0),PRIO(0),PRIO(15)), PQ_SUCCESS) 
// 	TEST_INT(5,pqGetSize(pq),1,"Single elem")

// 	pqDestroy(pq);
// 	pqDestroy(em2);

// 	LOG_CYAN("ChangePrio Test End")
// }

// // [x] Remove - empty pq, first inserted
// void TickTest()
// {
// 	LOG_CYAN("Remove Test Begin:")
	
// 	EventManager em1 = createEventManager(date);

// 	TEST_INT(1,pqRemove(em1),PQ_SUCCESS,"Empty em1\t")
// 	TEST_INT(2,pqGetSize(em1),0,"Empty em2\t")

// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(em1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
// 	}

// 	int j = 20;
// 	while (pqGetFirst(em1))
// 	{
// 		TEST_TRUE(pqGetFirst(em1),ELEM(j))
// 		j--;

// 		pqRemove(em1);
// 	}

// 	TEST_INT(3, pqGetSize(em1), 0, "Normal Case")
	
// 	pqDestroy(em1);

// 	LOG_CYAN("Remove Test End")
// }

// // [x] RemoveElem - empty pq, FIRST elem changed, doesnt exist
// void SizeANDGetNextTest()
// {
// 	LOG_CYAN("RemoveElem Test Begin:")
	
// 	EventManager em1 = createEventManager(date);

// 	TEST_INT(1,pqRemoveElement(em1,ELEM(0)),PQ_ELEMENT_DOES_NOT_EXISTS,"Empty PQ\t")

// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(em1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
// 	}

// 	EventManager em2 = pqCopy(em1);

// 	for (int j = 0; j < 21; j++)
// 	{
// 		TEST_TRUE(pqGetFirst(em1),ELEM(20-j))

// 		TEST_TRUE(pqRemoveElement(em1,pqGetFirst(em1)),PQ_SUCCESS)
// 	}

// 	TEST_INT(2, pqGetSize(em1), 0, "Normal Case")
	
// 	TEST_TRUE(pqRemoveElement(em2,ELEM(6)),PQ_SUCCESS)
// 	TEST_INT(3, pqRemoveElement(em2,ELEM(6)),PQ_ELEMENT_DOES_NOT_EXISTS, "Elem Not Exist1")
// 	TEST_INT(4, pqGetSize(em2),20, "Elem Not Exist2")
	
// 	pqDestroy(em1);
// 	pqDestroy(em2);

// 	LOG_CYAN("RemoveElem Test End")
// }

// // [] PrintsTest	- empty pq, same priorities, next after invalid, invalid inside loop, nested loop
// void PrintsTest()
// {
// 	LOG_CYAN("Iterator Test Begin:")
	
// 	EventManager pq = createEventManager(date);

// 	TEST_PTR(1, pqGetFirst(pq), NULL, "Empty PQ-getfirst\t")
// 	TEST_PTR(2, pqGetNext(pq), NULL, "Empty PQ-getnext\t")

// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(pq,ELEM(i),PRIO(i)), PQ_SUCCESS) 
// 	}

// 	TEST_PTR(3, pqGetNext(pq), NULL, "Invalid iter\t")

// 	EventManager em2 = pqCopy(pq);
// 	TEST_PTR(4, pqGetNext(pq), NULL, "Invalid iter-copy1")
// 	TEST_PTR(5, pqGetNext(em2), NULL, "Invalid iter-copy2")

// 	pqGetFirst(em2);
// 	TEST_TRUE(pqChangePriority(em2,ELEM(5),PRIO(5),PRIO(6)), PQ_SUCCESS)
// 	TEST_PTR(6, pqGetNext(em2), NULL, "Invalid iter-changePrio")
	
// 	pqGetFirst(em2);
// 	TEST_TRUE(pqRemove(em2), PQ_SUCCESS)
// 	TEST_PTR(7, pqGetNext(em2), NULL, "Invalid iter-Remove")

// 	pqGetFirst(em2);
// 	TEST_TRUE(pqRemoveElement(em2,ELEM(5)), PQ_SUCCESS)
// 	TEST_PTR(8, pqGetNext(em2), NULL, "Invalid iter-RemoveElem")


// 	bool good_test = true;
// 	int j = 20;
// 	PQ_FOREACH(int*, runner, pq)
// 	{
// 		if (*runner != j--)
// 		{
// 			good_test = false;
// 		}
// 	}

// 	TEST_INT(9, good_test, true, "Iterating\t\t")
	
// 	pqDestroy(pq);
// 	pqDestroy(em2);

// 	LOG_CYAN("Iterator Test End")
// }

// // [] Clear	- empty pq, normal case
// void ClearTest()
// {
// 	LOG_CYAN("Clear Test Begin:")
	
// 	EventManager em1 = createEventManager(date);
// 	EventManager empty_pq = createEventManager(date);

// 	TEST_INT(1,pqGetSize(empty_pq),PQ_SUCCESS,"Empty PQ\t")
// 	for (int i = 0; i < 21; i++)
// 	{
// 		TEST_TRUE(pqInsert(em1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
// 	}

// 	TEST_INT(2,pqGetSize(empty_pq),PQ_SUCCESS,"Normal Case")

// 	pqDestroy(em1);
// 	pqDestroy(empty_pq);

// 	LOG_CYAN("Clear Test End")
// }
