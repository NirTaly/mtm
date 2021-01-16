#include "date_wrap.h"
#include "exceptions.h"
#include <iostream>

#include "../../utils.h"

using namespace mtm;
using std::cout;
using std::endl;

template <class T> void print(const T& x) { cout << x << endl; }

void test1()
{
    LOG_CYAN("Test Start:")

    bool catch_exp = false;
    try  {
        DateWrap date(0, 11, 2020);
    } catch(const InvalidDate&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    DateWrap date(1, 1, 2000);
    int day = date.day() , month = date.month(), year = date.year();
    TEST_IS_TRUE(((day == 1) && (month == 1) && (year == 2000)))

    DateWrap date2(1, 1, 2000);

    TEST_IS_TRUE((date2 == date))
    TEST_IS_TRUE(!(date2 < date))
    TEST_IS_TRUE(!(date2 > date))
    TEST_IS_TRUE(date2 >= date)
    TEST_IS_TRUE(date2 <= date)
    TEST_IS_TRUE(!(date2 != date))

    DateWrap date3(2, 1, 2000);

    TEST_IS_TRUE(!(date3 == date))
    TEST_IS_TRUE(!(date3 < date))
    TEST_IS_TRUE((date3 > date))
    TEST_IS_TRUE(date3 >= date)
    TEST_IS_TRUE(!(date3 <= date))
    TEST_IS_TRUE((date3 != date))

    TEST_IS_TRUE((date3++) == DateWrap(2,1,2000))
    DateWrap check(1,1,0);
    

    catch_exp = false;
    try  {
        date2 += -1;
    } catch(const NegativeDays&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    date2 += 0;
    
    catch_exp = false;
    try {
        date = date + (-3);
    } catch (const NegativeDays&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    DateWrap d(1,1,0);
    DateWrap d1(1,1,0);

    d = 5 + ((d1 += 1)++) + 3;

    TEST_IS_TRUE(d1 == DateWrap(3,1,0))
    TEST_IS_TRUE(d  == DateWrap(10,1,0))

    DateWrap a(1,1,0);
    a += 150;
    TEST_IS_TRUE(a  == DateWrap(1,6,0))

    cout << "14/12/1995 == " << DateWrap(14,12,1995) << endl;
    cout << "1/1/-1 == " << DateWrap(1,1,-1) << endl;

    LOG_CYAN("Test End")
}

int main() {
    LOG_YELLOW("PartA Tests Begin:")

    test1();
    
    LOG_YELLOW("PartA Tests End")

    CHECK_VLG
    return 0;
}
