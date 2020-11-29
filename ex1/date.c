/******************************************************************************
                                  	Date                  
                         Roee Latzres , Nir Tali
                                    22.11.18
 *****************************************************************************/
#include	<stddef.h>	// NULL
#include	<stdlib.h>  // MALLOC

#include	"date.h"


/******************************************************************************/
struct Date_t
{
	int day;
	int month;
	int year;
};

static bool dateIsLegal(int day, int month, int year);

/******************************************************************************/
Date dateCreate(int day, int month, int year)
{
	if (dateIsLegal(day,month,year))
	{
		Date date = malloc(sizeof(struct Date_t));
		if (!date)
		{
			return (NULL);
		}

		date->day = day;
		date->month = month;
		date->year = year;
		
		return (date);
	}

	return (NULL);
}
/******************************************************************************/

void dateDestroy(Date date)
{
	if (date)
	{
		free(date);
		date = NULL;
	}
	
}
/******************************************************************************/

Date dateCopy(Date date)
{
	Date copy = NULL;
	
	if (date)
	{
		copy = dateCreate(date->day,date->month,date->year);
	}
	
	return (copy);
}
/******************************************************************************/

bool dateGet(Date date, int* day, int* month, int* year)
{
	if (day && month && year && date)
	{
		*day = date->day;
		*month = date->month;
		*year = date->year;

		return (true);
	}

	return (false);
}
/******************************************************************************/

int dateCompare(Date date1, Date date2)
{
	if (date1 && date2)
	{
		if (date1->year == date2->year)
		{
			if (date1->month == date2->month)
			{
				if (date1->day == date2->day)
				{
					return (0);
				}
				else
				{
					return (date1->day - date2->day);
				}
			}
			else
			{
				return (date1->month - date2->month);
			}
		}
		else
		{
			return (date1->year - date2->year);
		}
	}

	return (0);
}
/******************************************************************************/

void dateTick(Date date)
{
	if (date)
	{
		if (dateIsLegal(date->day+1,date->month,date->year))
		{
			date->day += 1;
		}
		else if (dateIsLegal(1,date->month+1,date->year))
		{
			date->day = 1;
			date->month += 1;
		}
		else
		{
			date->day 	= 1;
			date->month = 1;
			date->year += 1;
		}
	}
}

/******************************************************************************/
/******************************************************************************/
static bool dateIsLegal(int day, int month, int year)
{
	return (1 <= month && month <= 12 && 1 <= day && day <= 30);
	(void)year;
}