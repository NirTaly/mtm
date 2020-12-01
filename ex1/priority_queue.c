/******************************************************************************
                                  	PQ                  
                         Roee Latzres , Nir Tali
                                    27.11.20
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
};

static PQNode createNode(PriorityQueue pq, PQElement element, PQElementPriority priority);
static void destroyNode(PriorityQueue pq, PQNode node);
static PQNode insertAfter(PriorityQueue pq, PQNode curr_node, PQElement elem, PQElementPriority prio);
static PQNode pqPush(PriorityQueue pq, PQElement elem, PQElementPriority prio);
static bool pqIsEmpty(PriorityQueue pq);
// static void swapNodes(PQNode curr, PQNode next_curr);

/******************************************************************************/

PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities)
{
	PriorityQueue pq = NULL;
	
	if (copy_element && free_element && equal_elements && copy_priority && free_priority && compare_priorities)
	{
		pq = malloc(sizeof(struct PriorityQueue_t));
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
		}
	}
	
	return (pq);	
}					   
/******************************************************************************/

void pqDestroy(PriorityQueue queue)
{
	pqClear(queue);
	
	free(queue);
	queue = NULL;
}
/******************************************************************************/

PriorityQueue pqCopy(PriorityQueue queue)
{
	PriorityQueue pq_copy = NULL;
	if (queue)
	{
		pq_copy = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements,
			queue->copy_priority, queue->free_priority, queue->compare_priorities);
		if (!pq_copy)
		{
			return (NULL);
		}

		for (PQNode runner = queue->start; runner; runner = runner->next)
		{
			pq_copy->iter = insertAfter(pq_copy,pq_copy->iter,runner->element,runner->priority);
			if (!pq_copy->iter)
			{
				pqDestroy(pq_copy);
				return (NULL);
			}
		}

		queue->iter = NULL;
		pq_copy->iter = NULL; // Now it's "undefined"
	}
	return pq_copy;
}
/******************************************************************************/

int pqGetSize(PriorityQueue queue)
{
	if (queue)
	{
		int count = 0;
		for (PQNode runner = queue->start; runner; runner = runner->next)
		{
			count++;
		}

		return (count);
	}

	return (-1);	
}
/******************************************************************************/

bool pqContains(PriorityQueue queue, PQElement element)
{
	if (queue && element)
	{
		for (PQNode runner = queue->start; runner; runner = runner->next)
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
		queue->iter = NULL;
		
		// incase <priority> is the best priority in queue or queue isEmpty
		if ((pqIsEmpty(queue)) ||
			queue->compare_priorities(priority,queue->start->priority) > 0)
		{
			PQNode new_node = pqPush(queue,element,priority);
			return (new_node ? PQ_SUCCESS : PQ_OUT_OF_MEMORY);
		}

		PQNode runner = NULL;
		for (runner = queue->start; runner->next; runner = runner->next)
		{	
			if (queue->compare_priorities(priority,runner->next->priority) > 0)
			{
				break;
			}
		}
		
		PQNode new_node = insertAfter(queue,runner,element,priority);
		return (new_node ? PQ_SUCCESS : PQ_OUT_OF_MEMORY);	
	}
	// NEED TO INVALIDATE ITER IF NOTHING HAPPEND
	if (queue)
	{
		queue->iter = NULL;
	}
	
	return (PQ_NULL_ARGUMENT);
}
/******************************************************************************/

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
	if (queue && element && old_priority && new_priority)
	{
		if (queue->start)
		{
			if (queue->equal_elements(element, queue->start->element) &&
				0 == queue->compare_priorities(old_priority, queue->start->priority))
			{
				pqRemove(queue);
				return pqInsert(queue, element, new_priority);
			}
		}
		
		for (PQNode runner = queue->start; runner; runner = runner->next)
		{
			if (runner->next)
			{
				if (queue->equal_elements(element, runner->next->element) &&
					0 == queue->compare_priorities(old_priority, runner->next->priority))
				{
					PQNode found = runner->next;
					runner->next = found->next;
					destroyNode(queue,found);

					return pqInsert(queue, element, new_priority);
				}					
			}			
		}

		return (PQ_ELEMENT_DOES_NOT_EXISTS);
	}
	
	return (PQ_NULL_ARGUMENT);
}
/******************************************************************************/

PriorityQueueResult pqRemove(PriorityQueue queue)
{
	if (queue)
	{
		if (queue->start)
		{
			PQNode removed = queue->start;
			queue->start = removed->next;

			destroyNode(queue,removed);
		}		
		
		return (PQ_SUCCESS);
	}
	
	return (PQ_NULL_ARGUMENT);
}
/******************************************************************************/

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
	if (queue && element)
	{
		if (queue->start && queue->equal_elements(element,queue->start->element))
		{
			pqRemove(queue);
			return (PQ_SUCCESS);
		}
		
		for (PQNode runner = queue->start; runner; runner = runner->next)
		{
			if (runner->next && queue->equal_elements(element, runner->next->element))
			{
				PQNode removed = runner->next;
				runner->next = removed->next;

				destroyNode(queue,removed);

				return (PQ_SUCCESS);
			}
		}

		return (PQ_ELEMENT_DOES_NOT_EXISTS);
	}
	
	return (PQ_NULL_ARGUMENT);
}
/******************************************************************************/

PQElement pqGetFirst(PriorityQueue queue)
{
	if (queue && queue->start)
	{
		queue->iter = queue->start;

		return (queue->iter->element);
	}
	
	return (NULL);
}
/******************************************************************************/

PQElement pqGetNext(PriorityQueue queue)
{
	if (queue && queue->iter && queue->iter->next)
	{
		queue->iter = queue->iter->next;

		return (queue->iter->element);
	}
	
	return (NULL);
}
/******************************************************************************/
PriorityQueueResult pqClear(PriorityQueue queue)
{
	if (queue)
	{
		while (!pqIsEmpty(queue)) 
		{
			pqRemove(queue);
		}

		return (PQ_SUCCESS);
	}

	return (PQ_NULL_ARGUMENT);
}

/******************************************************************************/
/******************************************************************************/
static PQNode createNode(PriorityQueue pq, PQElement element, PQElementPriority priority)
{
	PQNode node = malloc(sizeof(struct PQNode_t));
	if (!node)
	{
		return (NULL);
	}
	
	node->element = pq->copy_element(element);
	node->priority = pq->copy_priority(priority);
	node->next = NULL;

	if (!node->element || !node->priority)
	{
		destroyNode(pq,node);
		node = NULL;
	}
	
	return (node);
}

static void destroyNode(PriorityQueue pq, PQNode node)
{
	pq->free_element(node->element);
	pq->free_priority(node->priority);
	
	free(node);
	node = NULL;

	pq->iter = NULL;
}

// Create new node and insert it after curr_node, if curr_node is NULL then insert in beginning of pq
// return new node in success, NULL else
static PQNode insertAfter(PriorityQueue pq, PQNode curr_node, PQElement elem, PQElementPriority prio)
{
	PQNode new_node = createNode(pq, elem, prio);
	if (new_node)
	{	
		if (!curr_node)
		{
			pq->start = new_node;
		}
		else
		{
			new_node->next = curr_node->next;
			curr_node->next = new_node;
		}
	}
		
	return (new_node);	
}


static PQNode pqPush(PriorityQueue pq, PQElement elem, PQElementPriority prio)
{
	PQNode new_node = createNode(pq, elem, prio);
	if (new_node)
	{
		if (pqIsEmpty(pq))
		{
			pq->start = new_node;
		}
		else
		{
			new_node->next = pq->start;
			pq->start = new_node;
		}
	}
		
	return (new_node);	
}
static bool pqIsEmpty(PriorityQueue pq)
{
	return (pq->start == NULL);
}