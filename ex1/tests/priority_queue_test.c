/******************************************************************************
                                  Priority Queue                
                                    Nir Tali
                                    22.11.18
 *****************************************************************************/
#include <stdlib.h>

#include "../utils.h"
#include "priority_queue.h"  

/******************************************************************************
	Tasks:
	[x] NullArgs		- all functions with every NULL arg
	[x] CreateDestroy	- empty pq, double destroy, normal case
	[x] Copy			- empty pq, normal case
	[x] Size			- empty pq, normal case
	[x] Contains		- empty pq, elem not exist, elem exist
	[x] Insert			- Malloc fail, normal case
	[x] ChangePrio		- FIRST elem changed, re-insert, Malloc fail, normal case
	[x] Remove			- empty pq, first inserted
	[x] RemoveElem		- empty pq, FIRST elem changed, doesnt exist
	[x] IteratorTests	- empty pq, same priorities, next after invalid, invalid inside loop, nested loop
	[x] Clear			- empty pq
******************************************************************************/
/*****************************************************************************
 * CreateDestroy		- Double Destroy?
 * Insert				- Bad Malloc - check user's copy functions?
 * 

*****************************************************************************/

/* Helper Functions  */
bool arePQEqual(PriorityQueue pq1, PriorityQueue pq2);
PQElement copyElemFunc(PQElement element);
PQElementPriority copyPrioFunc(PQElementPriority priority);
void freeElemFunc(PQElement element);
void freePrioFunc(PQElementPriority priority);
bool isEqualElemFunc(PQElement elem1,PQElement elem2);
int comparePrioFunc(PQElementPriority prio1, PQElementPriority prio2);
void FREEBADMALLOCFUNC(PQElement element);
PQElement COPYBADMALLOCFUNC(PQElement element);

/******************************************************************************/
/* Test Declarations */
void nullArgsTest();
void CreateDestroyTest();
void CopyTest();
void SizeTest();
void ContainsTest();
void InsertTest();
void ChangePrioTest();
void RemoveTest();
void RemoveElemTest();
void IteratorTests();
void ClearTest();

/* Global Variables */
int elements[]	= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int priorities[]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

#define ELEM(i) (&elements[i])
#define PRIO(i) (&priorities[i])
 /*****************************************************************************
 *                                M A I N                                    *
****************************************************************************/
int main(void)
{	
	nullArgsTest();
	CreateDestroyTest();
	CopyTest();
	SizeTest();
	ContainsTest();
	InsertTest();
	ChangePrioTest();
	RemoveTest();
	RemoveElemTest();
	IteratorTests();
	ClearTest();

	CHECK_VLG()

    return (0);
}

// [x] NullArgs		- all functions with every NULL arg
void nullArgsTest()
{
	LOG_CYAN("NULL Args Test Begin:")

	PriorityQueue pq = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	PriorityQueue pq_null = pqCreate(NULL,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	
	TEST_IS_TRUE(pq)
	TEST_IS_TRUE(!pq_null)

	TEST(1, pqCopy(pq_null), NULL, "pqCopy\t")
	TEST(2,	pqGetSize(pq_null), -1, "pqGetSize\t")
	TEST(3, pqContains(pq, NULL), false, "pqContains")
	TEST(4, pqContains(pq_null, ELEM(0)), false, "pqContains")
	TEST(5, pqInsert(pq, ELEM(0),NULL), PQ_NULL_ARGUMENT, "pqInsert\t")
	TEST(6, pqInsert(pq, NULL,PRIO(0)), PQ_NULL_ARGUMENT, "pqInsert\t")
	TEST(7, pqInsert(pq_null, ELEM(0),PRIO(0)), PQ_NULL_ARGUMENT, "pqInsert\t")
	TEST(8, pqChangePriority(pq_null, ELEM(0), PRIO(0), PRIO(1)), PQ_NULL_ARGUMENT, "pqChangePriority")
	TEST(9, pqChangePriority(pq, NULL, ELEM(0), PRIO(0)), PQ_NULL_ARGUMENT, "pqChangePriority")
	TEST(10,pqChangePriority(pq, ELEM(0), NULL, PRIO(0)), PQ_NULL_ARGUMENT, "pqChangePriority")
	TEST(11,pqChangePriority(pq, ELEM(0), PRIO(0), NULL), PQ_NULL_ARGUMENT, "pqChangePriority")
	TEST(12,pqRemove(pq_null), PQ_NULL_ARGUMENT, "pqRemove\t")
	TEST(13,pqRemoveElement(pq, NULL), PQ_NULL_ARGUMENT, "pqRemoveElement")
	TEST(14,pqRemoveElement(pq_null, ELEM(0)), PQ_NULL_ARGUMENT, "pqRemoveElement")
	TEST(15,pqGetFirst(pq_null), NULL, "pqGetFirst")
	TEST(16,pqGetNext(pq_null), NULL, "pqGetNext\t")
	TEST(17,pqClear(pq_null), PQ_NULL_ARGUMENT, "pqClear\t")
	
	pqDestroy(pq_null);
	pqDestroy(pq);

	LOG_CYAN("NULL Args Test END")
}

// [x] CreateDestroy	- empty pq, double destroy, normal case
// DOUBLE DESTROY DONT WORK!!!!!!!!!
void CreateDestroyTest()
{
	LOG_CYAN("Create Destroy Test Begin:")

	PriorityQueue pq1 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	PriorityQueue pq2 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	PriorityQueue null_pq = pqCreate(NULL,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);

	pqDestroy(null_pq);

	pqDestroy(pq1);
	// pqDestroy(pq1);

	for (int i = 0; i < 20; i++)
	{
		TEST_TRUE(pqInsert(pq2,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}

	pqDestroy(pq2);
	// pqDestroy(pq2);


	LOG_CYAN("Create Destroy Test END")
}

// [x] Copy	- empty pq, normal case
void CopyTest()
{
	LOG_CYAN("Copy Test Begin:")

	PriorityQueue pq1 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);

	PriorityQueue empty_pq = pqCopy(pq1);

	TEST_INT(1,pqGetSize(empty_pq), 0, "Empty PQ\t")

	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}

	PriorityQueue pq2 = pqCopy(pq1);
	TEST_INT(2,arePQEqual(pq1,pq2), 1, "Normal Case")
	
	pqDestroy(pq1);
	pqDestroy(empty_pq);
	pqDestroy(pq2);

	LOG_CYAN("Copy Test End")
}

// [x] Size	- empty pq, normal case
void SizeTest()
{
	LOG_CYAN("Size Test Begin:")
	
	PriorityQueue pq1 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	PriorityQueue empty_pq = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);

	TEST_INT(1,pqGetSize(empty_pq),0,"Empty PQ\t")
	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}

	TEST_INT(2,pqGetSize(pq1),21,"Normal Case")

	pqDestroy(pq1);
	pqDestroy(empty_pq);

	LOG_CYAN("Size Test End")
}

// [x] Contains	- empty pq, elem not exist, elem exist
void ContainsTest()
{
	LOG_CYAN("Contain Test Begin:")
	
	PriorityQueue empty_pq = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	TEST_INT(1,pqContains(empty_pq,0),0,"Empty PQ\t")

	PriorityQueue pq1 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}

	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqContains(pq1,ELEM(i)),1)
	}
	
	PriorityQueue pq2 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	for (int data = 0; data < 20; data++)
	{
		for (int prio = 0; prio < 3; prio++)
		{
			TEST_TRUE(pqInsert(pq2,ELEM(data),PRIO(prio)), PQ_SUCCESS)
		}
	}
	TEST_INT(2,pqContains(pq2,ELEM(20)),0,"Elem Not Found")
	
	for (int data = 0; data < 20; data++)
	{
		for (int prio = 0; prio < 3; prio++)
		{
			TEST_TRUE(pqContains(pq2,ELEM(data)),1)
			TEST_TRUE(pqRemoveElement(pq2,ELEM(data)),PQ_SUCCESS)
		}
	}

	TEST_INT(3,pqGetSize(pq2),0,"Duplicate Elems")

	pqDestroy(pq1);
	pqDestroy(pq2);
	pqDestroy(empty_pq);

	LOG_CYAN("Contain Test End")

}

// [x] Insert - Malloc fail, normal case
// all inserted, all inserted by priority, all inserted by priority2, malloc failed
void InsertTest()
{
	LOG_CYAN("Insert Test Begin:")
	
	PriorityQueue pq1 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	for (int i = 20; i >= 0; i--)
	{
		TEST_TRUE(pqInsert(pq1,ELEM(i),PRIO(20-i)), PQ_SUCCESS) 
	}
	TEST_INT(1, pqGetSize(pq1), 21, "Normal Case 1")
	
	int j = 0;
	bool good_test = true;
	PQ_FOREACH(int* , runner, pq1)
	{
		if (*runner != j)
		{
			good_test = false;
		}
		TEST_TRUE(*runner, j++)
	}
	TEST_INT(2,good_test,1,"Normal Case 2")

	TEST_TRUE(pqClear(pq1),PQ_SUCCESS)
	for (int prio = 0; prio < 20; prio++)
	{
		for (int data = 0; data < 20; data++)
		{
			TEST_TRUE(pqInsert(pq1,ELEM(data),PRIO(prio)), PQ_SUCCESS)
		}
	}
	for (int prio = 0; prio < 20; prio++)
	{
		for (int data = 0; data < 20; data++)
		{
			TEST_TRUE(pqContains(pq1,ELEM(data)),1)
			TEST_TRUE(pqRemoveElement(pq1,ELEM(data)),PQ_SUCCESS)
		}
	}
	TEST_INT(3,pqGetSize(pq1),0,"Normal Case 3")
	pqDestroy(pq1);


	PriorityQueue bad_malloc_pq = pqCreate(COPYBADMALLOCFUNC,FREEBADMALLOCFUNC,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	
	TEST_INT(4,pqInsert(bad_malloc_pq,ELEM(0),PRIO(0)),PQ_OUT_OF_MEMORY,"Malloc Fail")

	pqDestroy(bad_malloc_pq);
	
	LOG_CYAN("Insert Test End")
}

// [x] ChangePrio - FIRST elem changed, re-insert, Malloc fail, normal case
void ChangePrioTest()
{
	LOG_CYAN("ChangePrio Test Begin:")
	
	PriorityQueue pq = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);

	TEST_INT(1,pqChangePriority(pq, ELEM(0),PRIO(0), PRIO(1)),PQ_ELEMENT_DOES_NOT_EXISTS,"Empty PQ\t")
/*****************************************************************************/

	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}
	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqChangePriority(pq,ELEM(i),PRIO(i),PRIO(20-i)), PQ_SUCCESS) 
	}
	TEST_INT(2,pqGetSize(pq),21,"Normal Case")
	
	bool good_test = true;
	int j = 0;
	PQ_FOREACH(int*, runner, pq)
	{
		if (*runner != j++)
		{
			good_test = false;
		}
	}	
	TEST_INT(3,good_test,1,"Flip PQ\t")
/*****************************************************************************/
	pqClear(pq);

	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq,ELEM(0),PRIO(i)), PQ_SUCCESS) 
	}
	
	PriorityQueue pq2 = pqCopy(pq);
	
	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqChangePriority(pq,ELEM(0),PRIO(i),PRIO(20-i)), PQ_SUCCESS) 
	}
	
	TEST_INT(4,arePQEqual(pq,pq2),1, "dif prio\t")
/*****************************************************************************/
	pqClear(pq);

	TEST_TRUE(pqInsert(pq,ELEM(0),PRIO(0)), PQ_SUCCESS) 
	TEST_TRUE(pqChangePriority(pq,ELEM(0),PRIO(0),PRIO(15)), PQ_SUCCESS) 
	TEST_INT(5,pqGetSize(pq),1,"Single elem")

	pqDestroy(pq);
	pqDestroy(pq2);

	LOG_CYAN("ChangePrio Test End")
}

// [x] Remove - empty pq, first inserted
void RemoveTest()
{
	LOG_CYAN("Remove Test Begin:")
	
	PriorityQueue pq1 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);

	TEST_INT(1,pqRemove(pq1),PQ_SUCCESS,"Empty PQ1\t")
	TEST_INT(2,pqGetSize(pq1),0,"Empty PQ2\t")

	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}

	int j = 20;
	while (pqGetFirst(pq1))
	{
		TEST_TRUE(pqGetFirst(pq1),ELEM(j))
		j--;

		pqRemove(pq1);
	}

	TEST_INT(3, pqGetSize(pq1), 0, "Normal Case")
	
	pqDestroy(pq1);

	LOG_CYAN("Remove Test End")
}

// [x] RemoveElem - empty pq, FIRST elem changed, doesnt exist
void RemoveElemTest()
{
	LOG_CYAN("RemoveElem Test Begin:")
	
	PriorityQueue pq1 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);

	TEST_INT(1,pqRemoveElement(pq1,ELEM(0)),PQ_ELEMENT_DOES_NOT_EXISTS,"Empty PQ\t")

	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}

	PriorityQueue pq2 = pqCopy(pq1);

	for (int j = 0; j < 21; j++)
	{
		TEST_TRUE(pqGetFirst(pq1),ELEM(20-j))

		TEST_TRUE(pqRemoveElement(pq1,pqGetFirst(pq1)),PQ_SUCCESS)
	}

	TEST_INT(2, pqGetSize(pq1), 0, "Normal Case")
	
	TEST_TRUE(pqRemoveElement(pq2,ELEM(6)),PQ_SUCCESS)
	TEST_INT(3, pqRemoveElement(pq2,ELEM(6)),PQ_ELEMENT_DOES_NOT_EXISTS, "Elem Not Exist1")
	TEST_INT(4, pqGetSize(pq2),20, "Elem Not Exist2")
	
	pqDestroy(pq1);
	pqDestroy(pq2);

	LOG_CYAN("RemoveElem Test End")
}

// [] IteratorTests	- empty pq, same priorities, next after invalid, invalid inside loop, nested loop
void IteratorTests()
{
	LOG_CYAN("Iterator Test Begin:")
	
	PriorityQueue pq = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);

	TEST_PTR(1, pqGetFirst(pq), NULL, "Empty PQ-getfirst\t")
	TEST_PTR(2, pqGetNext(pq), NULL, "Empty PQ-getnext\t")

	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}

	TEST_PTR(3, pqGetNext(pq), NULL, "Invalid iter\t")

	PriorityQueue pq2 = pqCopy(pq);
	TEST_PTR(4, pqGetNext(pq), NULL, "Invalid iter-copy1")
	TEST_PTR(5, pqGetNext(pq2), NULL, "Invalid iter-copy2")

	pqGetFirst(pq2);
	TEST_TRUE(pqChangePriority(pq2,ELEM(5),PRIO(5),PRIO(6)), PQ_SUCCESS)
	TEST_PTR(6, pqGetNext(pq2), NULL, "Invalid iter-changePrio")
	
	pqGetFirst(pq2);
	TEST_TRUE(pqRemove(pq2), PQ_SUCCESS)
	TEST_PTR(7, pqGetNext(pq2), NULL, "Invalid iter-Remove")

	pqGetFirst(pq2);
	TEST_TRUE(pqRemoveElement(pq2,ELEM(5)), PQ_SUCCESS)
	TEST_PTR(8, pqGetNext(pq2), NULL, "Invalid iter-RemoveElem")


	bool good_test = true;
	int j = 20;
	PQ_FOREACH(int*, runner, pq)
	{
		if (*runner != j--)
		{
			good_test = false;
		}
	}

	TEST_INT(9, good_test, true, "Iterating\t\t")
	
	pqDestroy(pq);
	pqDestroy(pq2);

	LOG_CYAN("Iterator Test End")
}

// [] Clear	- empty pq, normal case
void ClearTest()
{
	LOG_CYAN("Clear Test Begin:")
	
	PriorityQueue pq1 = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);
	PriorityQueue empty_pq = pqCreate(copyElemFunc,freeElemFunc,isEqualElemFunc,copyPrioFunc,freePrioFunc,comparePrioFunc);

	TEST_INT(1,pqGetSize(empty_pq),PQ_SUCCESS,"Empty PQ\t")
	for (int i = 0; i < 21; i++)
	{
		TEST_TRUE(pqInsert(pq1,ELEM(i),PRIO(i)), PQ_SUCCESS) 
	}

	TEST_INT(2,pqGetSize(empty_pq),PQ_SUCCESS,"Normal Case")

	pqDestroy(pq1);
	pqDestroy(empty_pq);

	LOG_CYAN("Clear Test End")
}





/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool arePQEqual(PriorityQueue pq1, PriorityQueue pq2)
{
	while (pqGetFirst(pq1) && pqGetFirst(pq1) == pqGetFirst(pq2))
	{
		TEST_TRUE(pqGetFirst(pq1),pqGetFirst(pq2))
		TEST_TRUE(pqRemove(pq1),PQ_SUCCESS)
		TEST_TRUE(pqRemove(pq2),PQ_SUCCESS)
	}

	return (pqGetFirst(pq1) == NULL && pqGetFirst(pq2) == NULL);
}

PQElement copyElemFunc(PQElement element)
{
	return (element);
}
PQElementPriority copyPrioFunc(PQElementPriority priority)
{
	return (priority);
}
void freeElemFunc(PQElement element)
{
	(void)element;
}
void freePrioFunc(PQElementPriority priority)
{
	(void)priority;
}
bool isEqualElemFunc(PQElement elem1,PQElement elem2)
{
	return (*(int*)elem1 == *(int*)elem2);
}
int comparePrioFunc(PQElementPriority prio1, PQElementPriority prio2)
{
	return (*(int*)prio1 - *(int*)prio2);
}


PQElement COPYBADMALLOCFUNC(PQElement element)
{
	(void)element;
	return malloc(__SIZE_MAX__);
}
void FREEBADMALLOCFUNC(PQElement element)
{
	free(element);
}