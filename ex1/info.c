#include <stddef.h>	// NULL
#include <stdlib.h> // MALLOC
#include <string.h>	// strcmp

#include "info.h"


struct MemDetails_t
{
	unsigned int event_count;
	int uid;
};

struct Information_t
{
	char* name;
	unsigned int event_count;
	int uid;
};

/*****************************************************************************/
Info createInfo(char* name, int uid, unsigned int count)
{
	Info info = malloc(sizeof(struct Information_t));
	if (info)
	{
		if (!infoSetAll(info,name,uid,count))
		{
			free(info);
			return (NULL);
		}
	}
	
	return (info);
}

Info copyInfo(Info info)
{
	return createInfo(info->name,info->uid,info->event_count);
}

void freeInfo(Info elem)
{
	Info info = (Info)elem;
	if (info)
	{
		if (info->name)
		{
			free(info->name);
		}
		
		free(info);
	}
	
}

bool equalInfo(Info elem1, Info elem2)
{
	Info info1 = (Info)elem1;
	Info info2 = (Info)elem2;

	return (info1->uid == info2->uid);
}
MemDetails createMem(unsigned int count, int mem_id)
{
	MemDetails prio = malloc(sizeof(MemDetails));
	if (prio)
	{
		prio->event_count = count;
		prio->uid = mem_id;
	}

	return (prio);
}
MemDetails copyMemDetails(MemDetails priority)
{
	return (createMem(priority->event_count, priority->uid));
}
void freeMemDetails(MemDetails priority)
{
	free (priority);
}
int compareMemDetails(MemDetails prio1, MemDetails prio2)
{
	MemDetails lhs = prio1;
	MemDetails rhs = prio2;

	int first_order = (lhs->event_count - rhs->event_count);

	if (0 == first_order)
	{
		return (rhs->uid - lhs->uid);
	}
	
	return (first_order);
}

char* infoGetName(Info info)
{
	return (info->name);
}

int infoGetUID(Info info)
{
	return (info->uid);
}

unsigned int infoGetCount(Info info)
{
	return (info->event_count);
	
}

bool infoSetName(Info info, char* name)
{
	if (name)
	{
		info->name = malloc(strlen(name) + 1);
		if (!info->name)
		{
			return (false);
		}
		strcpy(info->name, name);
	}
	else
	{
		info->name = NULL;
	}
	
	return (true);
}

bool infoSetUID(Info info, int uid)
{
	info->uid = uid;
	return true;
}

bool infoSetCount(Info info, unsigned int count)
{
	info->event_count = count;
	return true;
}

bool infoSetAll(Info info, char* name, int uid, unsigned int count)
{
	
	return (infoSetName(info, name) && infoSetUID(info, uid) && infoSetCount(info, count));
}