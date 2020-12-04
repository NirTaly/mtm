/******************************************************************************
                                Doubly Linked List
                                    Nir Tali
                                    04.12.20
 *****************************************************************************/


#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h>      /* size_t */ 

typedef enum DListResult_t {
    DL_SUCCESS,
    DL_OUT_OF_MEMORY,
    DL_NULL_ARGUMENT,
    DL_INVALID_DATE,
    DL_INVALID_EVENT_ID,
    DL_EVENT_ALREADY_EXISTS,
    DL_EVENT_ID_ALREADY_EXISTS,
    DL_EVENT_NOT_EXISTS,
    DL_EVENT_ID_NOT_EXISTS,
    DL_INVALID_MEMBER_ID,
    DL_MEMBER_ID_ALREADY_EXISTS,
    DL_MEMBER_ID_NOT_EXISTS,
    DL_EVENT_AND_MEMBER_ALREADY_LINKED,
    DL_EVENT_AND_MEMBER_NOT_LINKED,
    DL_ERROR
} DListResult;


typedef struct DListNode_t* DListNode;
typedef struct DList_t* DList;

typedef struct Info_t
{
	char* name;
	int uid;
}Info;

typedef struct Element_t
{
	DList list;
	Info member;
}Element;


/* 
 * Create empty list
 * Return NULL if malloc failed
 */
DList DListCreate(void); 									

void DListDestroy(DList dlist);								

/* Complexity O(n) */
size_t DListCount(const DList dlist);					

/*
 * Return value: 0 - not empty, !0 - empty
 * Complexity O(1)
 */
int DListIsEmpty(const DList dlist);

/* When dlist is empty - returns End(), complexity O(1) */
DListNode DListBegin(const DList dlist);

/* Returns iterator to the last (stub) element , complexity O(1) */
DListNode DListEnd(const DList dlist);

/* Undefined behavior when iter = End(), complexity O(1) */
DListNode DListNext(DListNode iter);

/* Undefined behavior when iter = Begin(), complexity O(1) */
DListNode DListPrev(DListNode iter);

/* Return NULL when iter = End(), complexity O(1) */
Element* DListGetData(const DListNode iter);


/* Returns iterator to following iter from the erased
 * Data will be point to the data of the erased - if isn't - NULL
 * Complexity O(1)              
 */
DListResult DListErase(DListNode where, void **data);


/*
 * Push data before End
 * Return value: 0 = success, !0 = failure
 * Complexity O(1) 
 */
DListResult DListPushBack(DList dlist, void *data);


/*
 * Pop the element before End and return data 
 * Return NULL if dlist is empty
 * Complexity O(1) 
 */
DListResult DListPopBack(DList dlist);

                       
/* Return iterator to the one who found,
 * The iteration doesn't include "to" 
 * Return "to" if havn't found
 */
DListNode DListFind(DListNode from, DListNode to, int uid);

#endif     /* __DLIST_H__ */

