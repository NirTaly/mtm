#include <iostream>

#include "pq.h"
#include "../utils.h"
#include "../partA/date_wrap.h"

using namespace mtm;

struct DateCompare
{
	bool operator()(const DateWrap& a, const DateWrap& b) const
	{
		return a < b;
	}
};

int main()
{
	PriorityQueue<DateWrap, DateCompare> pq;

	DateWrap dates[] = {DateWrap(3,1,2000), DateWrap(2,1,2000), DateWrap(1,1,2000), DateWrap(1,1,2005),
						DateWrap(1,1,1999), DateWrap(1,1,1998), DateWrap(1,1,1997)};

	TEST_IS_TRUE(pq.isEmpty()) 
	TEST_IS_TRUE(0 == pq.size());
	TEST_IS_TRUE(false == pq.isIn(DateWrap(14,12,1995)));

	try
	{
        pq.pop();
	} catch(const PQEmpty& e)	
	{std::cout << "PQEmpty Success" << std::endl; }

	for (auto date : dates)
	{
		pq.push(date);
	}
	
	TEST_IS_TRUE(false == pq.isEmpty()) 
	TEST_IS_TRUE(7 == pq.size());
	
	for (int i = 0; i < 7; i++)
	{
		TEST_IS_TRUE(pq.isIn(dates[i]));
	}
	
	for (const DateWrap* i = pq.begin(); i != pq.end(); i= pq.next())
	{
		std::cout << *i << std::endl;
	}

	for (int i = 0; i < 7; i++)
	{
		pq.remove(dates[i]);
	}

	TEST_IS_TRUE(pq.isEmpty()) 
	
	for (const DateWrap* i = pq.begin(); i != pq.end(); i= pq.next())
	{
		std::cout << *i << std::endl;
	}
}
