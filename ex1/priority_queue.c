/******************************************************************************
                                  	PQ                  
                         Roee Latzres , Nir Tali
                                    22.11.18
 *****************************************************************************/
#include	<stddef.h>	// NULL
#include	<stdlib.h>  // MALLOC

#include	"priority_queue.h"


/******************************************************************************/
typedef struct PQNode_t* PQNode;
struct PQNode_t
{
	PQElement element;
	PQElementPriority priority;
	PQNode next; 
};

struct PriorityQueue_t
{
	CopyPQElement copy_element;
	FreePQElement free_element;
	EqualPQElements equal_elements;
	CopyPQElementPriority copy_priority;
	FreePQElementPriority free_priority;
	ComparePQElementPriorities compare_priorities;
	PQNode start;
	PQNode iter;
	size_t length;
};

static PQNode createNode(PQElement element, PQElementPriority priority);
static PQNode InsertAfter(PriorityQueue pq, PQNode curr_node, PQElement elem, PQElementPriority prio);
static bool pqIsEmpty(PriorityQueue pq);

/******************************************************************************/

PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities)
{
	if (copy_element && free_element && equal_elements && copy_priority && free_priority && compare_priorities)
	{
		PriorityQueue pq = malloc(sizeof(struct PriorityQueue_t));
		if (pq)
		{
			pq->copy_element = copy_element;
			pq->free_element = free_element;
			pq->equal_elements = equal_elements;
			pq->copy_priority = copy_priority;
			pq->free_priority = free_priority;
			pq->compare_priorities = compare_priorities;
			pq->start = NULL;
			pq->iter = NULL;
			pq->length = 0;

			return (pq);	
		}
	}

	return (NULL);
}					   
/******************************************************************************/

void pqDestroy(PriorityQueue queue)
{
	if (pqClear(queue))
	{
		free(queue);

		queue = NULL;
	}
}
/******************************************************************************/

PriorityQueue pqCopy(PriorityQueue queue)
{
	PriorityQueue pq_copy = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements,
		queue->copy_priority, queue->free_priority, queue->compare_priorities);
	if (!pq_copy)
	{
		return (NULL);
	}

	if (!pqIsEmpty(queue))
	{		
		PQ_FOREACH(PQNode, runner, queue)
		{
			pq_copy->iter = InsertAfter(pq_copy,pq_copy->iter,runner->element,runner->priority);
			if (!pq_copy->iter)
			{
				pqDestroy(pq_copy);
				return (NULL);
			}
		}
	}

	pq_copy->iter = NULL; // Now it's undefined
	return pq_copy;
}
/******************************************************************************/

int pqGetSize(PriorityQueue queue)
{
	return (queue ? queue->length : -1);
}
/******************************************************************************/

bool pqContains(PriorityQueue queue, PQElement element)
{
	if (queue && element)
	{
		PQ_FOREACH(PQNode, runner, queue)
		{
			if(queue->equal_elements(runner->element,element))
			{
				return (true);
			}
		}
	}

	return (false);
}
/******************************************************************************/
PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
	if (queue && element && priority)
	{
		PQ_FOREACH(PQNode, runner, queue)
		{
			if ()
		}
	}
}
/******************************************************************************/


/******************************************************************************/

PriorityQueueResult pqClear(PriorityQueue queue)
{
	if (queue)
	{
		while (pqRemove(queue) != PQ_NULL_ARGUMENT) { 
		}

		return (PQ_SUCCESS);
	}

	return (PQ_NULL_ARGUMENT);
}

/******************************************************************************/
/******************************************************************************/
static PQNode createNode(PQElement element, PQElementPriority priority)
{
	PQNode node = malloc(sizeof(struct PQNode_t));
	if (!node)
	{
		return (NULL);
	}
	
	node->element = element;
	node->priority = priority;
}

// Create new node and insert it after curr_node, if curr_node is NULL then insert in beginning of pq
// return new node in success, NULL else
static PQNode InsertAfter(PriorityQueue pq, PQNode curr_node, PQElement elem, PQElementPriority prio)
{
	PQNode new_node = createNode(elem, prio);

	if (!new_node)
	{
		return (NULL);
	}
	
	if (!curr_node)
	{
		pq->start = new_node;
		new_node->next = NULL;
	}
	else
	{
		new_node->next = curr_node->next;
		curr_node->next = new_node;
	}
		
	return (new_node);	
}

static bool pqIsEmpty(PriorityQueue pq)
{
	return (pq->start == NULL);
}