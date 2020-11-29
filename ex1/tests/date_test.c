
#include "../date.h"
#include "../../utils.h"

/******************************************************************************/
/*							Function Declaration							  */

void CreateDestroyTest(void);
void GetTest(void);
void CompareTest(void);
void TickTest(void);
/******************************************************************************/

int main(void)
{
	CreateDestroyTest();

	GetTest();

	CompareTest();

	TickTest();

	LOG_CYAN("\t\t\tCheck ValGrind")

	return 0;
}



/******************************************************************************/

void CreateDestroyTest(void)
{
	Date date1 = dateCreate(1,1,2000);
	Date date2 = dateCreate(32,1,2000);

	dateDestroy(date1);
	dateDestroy(date2);
}

void GetTest(void)
{
	Date date1 = dateCreate(1,1,2000);
	Date date2 = dateCreate(29,2,2020);
	Date date3 = dateCreate(14,12,1995);
	Date bad_date = dateCreate(31,4,1999);

	int day,month,year;
	TEST(1, dateGet(date1,&day,&month,&year), true, "\tdateGet() - 1\t")
	TEST(2, dateGet(date2,&day,&month,&year), true, "\tdateGet() - 2\t")
	TEST(3, dateGet(date3,&day,&month,&year), true, "\tdateGet() - 3\t")
	NOT_EQUAL_TEST(4, dateGet(bad_date,&day,&month,&year), true, "\tdateGet(bad_date) - 4")

	dateDestroy(date1);
	dateDestroy(date2);
	dateDestroy(date3);
	dateDestroy(bad_date);

}

void CompareTest(void)
{
	Date date1 = dateCreate(1,1,2000);
	Date date2 = dateCreate(29,2,2020);
	Date date3 = dateCreate(14,12,1995);
	Date bad_date = dateCreate(31,4,1999);

	TEST_INT(5, dateCompare(date1,date2), -20,	"\tdateCompare() - 1")
	TEST_INT(6, dateCompare(date1,date3), 5,	"\tdateCompare() - 2")
	TEST_INT(7, dateCompare(date3,date3), 0,	"\tdateCompare(same) - 3")
	TEST_INT(8, dateCompare(date3,bad_date), 0, "\tdateCompare(bad) - 4")
	
	dateDestroy(date1);
	dateDestroy(date2);
	dateDestroy(date3);
	dateDestroy(bad_date);
}

void TickTest(void)
{
	Date date = dateCreate(1,1,2000);
	Date copy = dateCopy(date);

	for (int i = 1; i <= 360; i++)
	{
		dateTick(date);
	}
	
	int d,m,y;
	TEST_INT(9, dateCompare(date,copy), 1, "\tdateTick() - 1\t")
	TEST(10, dateGet(date,&d,&m,&y), true, "\tdateGet() - 2\t")
	TEST_INT(11, d, 1, "\tdateTick() - 2\t")
	TEST_INT(12, m, 1, "\tdateTick() - 3\t")
	TEST_INT(13, y, 2001, "\tdateTick() - 4\t")

	dateDestroy(date);
	dateDestroy(copy);
}