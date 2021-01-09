#include "dv.h"
#include "utils.h"

using namespace mtm;

int main()
{
	DynamicVector<size_t> dv(1);

	dv[1] = true; 
	dv[1] = false;

	DynamicVector<size_t> copy_dv(dv);

  	TEST_IS_TRUE(!dv.last());

	TEST_IS_TRUE(copy_dv.isEmpty());

	for (size_t i = 1; i <= 20000; i++)
	{
		dv[i] = true;
	}
	
	TEST_IS_TRUE(20000 == dv.length());
	TEST_IS_TRUE(!dv.isEmpty());
	
	bool isin = true;
	for (size_t i = 1; i < 20000; i++)
	{
		if (!dv.isIn(i))
		{
			isin = false;
		}
	}
	TEST_IS_TRUE(isin)


}