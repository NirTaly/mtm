#include "date_wrap.h"
#include "exceptions.h"
#include <iostream>
using mtm::DateWrap;
using std::cout;
using std::endl;

template <class T> void print(const T& x) { cout << x << endl; }

int main() {
    print("My Tests:");
    try {
        DateWrap bad_date(0, 0, 2020);  // InvalidDate
        DateWrap bad_date2(bad_date);  // InvalidDate
    } catch(const mtm::InvalidDate&) {
        print("InvalidDate");
    }
    
    DateWrap datedate(1, 1, 2020);
    DateWrap date1(datedate);
    date1 = (datedate = (1 + datedate + 1))++;          // date = 3/1/20 ... date1 = 4/1/20

    DateWrap check_date(3,1,2020);
    print (datedate);
    print (date1);
    cout << "op correct? " << std::boolalpha << ((date1 == check_date) && (datedate == (check_date+1))) << endl;
    print("Finish My Tests\n");

    DateWrap date(30, 11, 2020);
    print(date);
    print(date + 4);
    print(3 + date);
    date++;
    print(date);
    date += 7;
    print(date);
    print(date > DateWrap(29, 11, 2020));
    print(date <= DateWrap(29, 11, 2020));
    print(date == DateWrap(30, 11, 2020));
    print(date == DateWrap(8, 12, 2020));
    try {
        date += (-3);
    } catch (mtm::NegativeDays) {
        print("NegativeDays");
    }
    try {
        date = date + (-3);
    } catch (mtm::NegativeDays) {
        print("NegativeDays");
    }
    return 0;
}
