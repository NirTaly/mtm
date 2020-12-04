/******************************************************************************
                                 Data Structures
                                Doubly Linked List
                               Functions Definition                  
                                    Nir Tali
                                    20.11.18
 *****************************************************************************/

#include <stdlib.h> /* malloc */
#include <stdio.h>  /* printf */
#include <assert.h> /* assert */

#include "dlist.h"  /* Doubly Linked List Library */

static DListNode DListCreateNode(Data data, DListNode *prev, DListNode *next);
static void DListFreeNode(DListNode iter);


struct DListNode_t
{
	DListNode next;
    DListNode prev;

	Data* data;
};


struct DList_t 
{
	struct DListNode_t start;
	struct DListNode_t end;
};


DList DListCreate(void)
{
	DList dlist = NULL;

	dlist = malloc(sizeof(struct DList_t));
	if (dlist)
	{
		dlist->start.prev = NULL;
		dlist->start.next = &dlist->end;
		dlist->start.data = NULL;

		dlist->end.prev = &dlist->start;
		dlist->end.next = NULL;
		dlist->end.data = NULL;
	}

	return (dlist);
}

void DListDestroy(DList dlist)
{
	DListNode iter = NULL;
	DListNode place_keeper = NULL;

	assert(dlist);

	iter = DListBegin(dlist);
	place_keeper = iter->next;

	for ( ; iter != DListEnd(dlist); place_keeper = place_keeper->next)
	{
		DListFreeNode(iter);
		iter = place_keeper;
	}
	
	free(dlist);
	dlist = NULL;
}

size_t DListCount(const DList dlist)
{
	DListNode iter = NULL;
	size_t count = 0;

	assert(dlist);

	iter = DListBegin(dlist);

	for ( ; iter != DListEnd(dlist); iter = DListNext(iter))
	{
		count++;
	}

	return (count);
}

int DListIsEmpty(const DList dlist)
{
	assert(dlist);
	return (DListBegin(dlist) == DListEnd(dlist));
}

DListNode DListBegin(const DList dlist)
{
	assert(dlist);
	return (dlist->start.next);
}

DListNode DListEnd(const DList dlist)
{
	assert(dlist);
	return ((DListNode)&(dlist->end));
}

DListNode DListNext(DListNode iter)
{
	assert(iter);
	return (iter->next);
}

DListNode DListPrev(DListNode iter)
{
	assert(iter);
	return (iter->prev);
}

void *DListGetData(const DListNode iter)
{

	if (!DListIsBadIter(iter))
	{
		return (iter->data);
	}

	return (NULL);
}

int DListIsBadIter(DListNode iter)
{
	if(NULL == iter || NULL == iter->next || NULL == iter->prev)
	{
		return (1);
	}

	return (0);
}

DListNode DListInsert(DListNode where, Data data)
{
	DListNode node = NULL;

	assert(where);

	node = DListCreateNode(data, DListPrev(where), where);
	if (NULL == node)
	{
		return (NULL);
	}

	where->prev->next = node;
	where->prev = node;

	return (node);
}

DListNode DListErase(DListNode where, void **data)
{	
	DListNode where_next = NULL;

	if (!DListIsBadIter(where))
	{
		where_next = DListNext(where);
		where->prev->next = where->next;
		where->next->prev = where->prev;

		if (NULL != data)
		{
			*data = DListGetData(where);
		}

		DListFreeNode(where);
	}

	return (where_next);
}

int DListPushFront(DList dlist, void *data)
{
	DListNode node = NULL;
	assert(dlist);

	
	node = DListInsert(DListBegin(dlist),data);
	if (NULL == node)
	{
		return (1);
	}

	return (0);
}

int DListPushBack(DList dlist, void *data)
{
	DListNode node = NULL;
	assert(dlist);

	node = DListInsert(DListEnd(dlist),data);
	if (NULL == node)
	{
		return (1);
	}

	return (0);
}

void *DListPopFront(DList dlist)
{
	void *data_popd = NULL;

	assert(dlist);

	DListErase(DListBegin(dlist),&data_popd);

	return (data_popd);
}

void *DListPopBack(DList dlist)
{
	assert(dlist);
	void *data_popd = NULL;

	DListErase(DListPrev(DListEnd(dlist)),&data_popd);
	
	return (data_popd);
}

void DListSpliceBefore(DListNode dest, 
                       DListNode src_begin, DListNode src_end)
{
	DListNode last_copy = NULL;

	assert(dest);
	assert(src_end);
	assert(src_begin);

	if (src_begin != src_end)
	{
	    last_copy = src_end->prev;
	    /* isolate src_star to src_end from thier list */
	    src_begin->prev->next = src_end;
	    src_end->prev = src_begin->prev;

   	    /* connecting src_begin to dest's list */
	    dest->prev->next = src_begin;
	    src_begin->prev = dest->prev;
	
	    /* connecting src_end to dest's list */
	    last_copy->next = dest;
	    dest->prev = last_copy;
	}
}

DListNode DListForEach(DListNode from, DListNode to, action_func func,
			  	 		  void *param)
{
	DListNode runner = from;

	assert(from);
	assert(to);
	assert(func);

	for ( ; runner != to; runner = DListNext(runner))
	{
		if(0 != func(DListGetData(runner), param))
		{
			break;
		}
	}

	return (runner);
}			  	 

DListNode DListFind(DListNode from, DListNode to, int uid
{
	DListNode runner = from;

	assert(from);
	assert(to);
	assert(func);

	for ( ; runner != to; runner = DListNext(runner))
	{
		if(0 == func(DListGetData(runner), param))
		{
			break;
		}
	}

	return (runner);
}                                                      

static DListNode DListCreateNode(Data data, DListNode *prev, DListNode *next)
{
	DListNode node = NULL;

	node = malloc(sizeof(struct DListNode_t));
	if (node)
	{	
		node->data->list = DListCreate();
		if (!node->data->list)
		{
			DListFreeNode(node);
			return (NULL);
		}
		
		node->data->name = data.name;
		node->data->uid = data.uid;
		
		node->next = next;
		node->prev = prev;
	}

	return (node);
}

static void DListFreeNode(DListNode iter)
{
	iter->next = NULL;
	iter->prev = NULL;
	DListDestroy(iter->data->list);

	free(iter);
	iter = NULL;
}