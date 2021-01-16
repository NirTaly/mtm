#include <cstdlib>
#include <iostream>
#include <functional>
#include <fstream>      // std::ofstream

#include "base_event.h"
#include "closed_event.h"
#include "custom_event.h"
#include "date_wrap.h"
#include "event_container.h"
#include "exceptions.h"
#include "festival.h"
#include "one_time_event.h"
#include "open_event.h"
#include "recurring_event.h"


#include "../../utils.h"

typedef mtm::EventContainer::EventIterator Iter;


struct StudentFilter {
    bool operator()(int student) {
        return student == 1 || student == 3 || student == 20000;
    }
};

std::function<bool(int)> canRegister = [](int student) { return student == 15;} ;

void printEventsShort(mtm::EventContainer& events, std::ostream& os) {
    for (Iter iter = events.begin(); iter != events.end(); ++iter) {
        mtm::BaseEvent& event = *iter;

        event.printShort(os);
    }
}
/********************************************************************************/
void CustomTest()
{
    LOG_CYAN("CustomEvent Test Start:")

    mtm::BaseEvent* custom1 = new mtm::CustomEvent<StudentFilter>(mtm::DateWrap(1,1,2020), "Custom-Functor");
    mtm::BaseEvent* custom2 = new mtm::CustomEvent<std::function<bool(int)>>(mtm::DateWrap(1,1,2020), "Custom-function",canRegister);

    bool catch_exp = false;
    try  {
        custom1->registerParticipant(0);
    } catch(const mtm::InvalidStudent& e) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    catch_exp = false;
    try  {
        custom1->registerParticipant(1234567891);
    } catch(const mtm::InvalidStudent& e) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    
    catch_exp = false;
    try  {
        custom1->registerParticipant(666);
    } catch(const mtm::RegistrationBlocked& e) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    catch_exp = false;
    try  {
        custom2->registerParticipant(14);
    } catch(const mtm::RegistrationBlocked& e) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    
    catch_exp = false;
    try  {
        custom2->unregisterParticipant(123);
    } catch(const mtm::NotRegistered&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    
    custom2->registerParticipant(15);
    catch_exp = false;
    try  {
        custom2->registerParticipant(15);
    } catch(const mtm::AlreadyRegistered&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    custom1->registerParticipant(1);
    custom1->registerParticipant(3);

    std::ofstream file;
    file.open ("TEST1_OUTPUT.txt");
    
    custom1->printShort(file);
    custom1->printLong(file);
    mtm::BaseEvent* clone = custom1->clone();
    clone->printShort(file);
    clone->printLong(file);
    custom2->printShort(file);
    custom2->printLong(file);

    delete custom1; delete custom2; delete clone;
    
    LOG_CYAN("CustomEvent Test End")
}

void EventTest()
{
    LOG_CYAN("EventTest Test Start:")

    mtm::OpenEvent open(mtm::DateWrap(1,1,2020), "Open");
    mtm::ClosedEvent close(mtm::DateWrap(1,1,2020), "Closed");

    bool catch_exp = false;
    try  {
        open.registerParticipant(0);
    } catch(const mtm::InvalidStudent&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    catch_exp = false;
    try  {
        open.registerParticipant(1234567891);
    } catch(const mtm::InvalidStudent&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    catch_exp = false;
    try  {
        close.registerParticipant(0);
    } catch(const mtm::InvalidStudent&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    catch_exp = false;
    try  {
        close.registerParticipant(1234567891);
    } catch(const mtm::InvalidStudent&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    
    catch_exp = false;
    try  {
        close.registerParticipant(666);
    } catch(const mtm::RegistrationBlocked&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    catch_exp = false;
    try  {
        close.unregisterParticipant(10);
    } catch(const mtm::NotRegistered&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    for (int i = 1; i <= 200; i++)
    {
        close.addInvitee(i);
    }
    close.addInvitee(1234567890);
    
    close.registerParticipant(1234567890);
    for (int i = 30; i >= 10; i--)
    {
        close.registerParticipant(i);
    }

    open.registerParticipant(1234567890);
    open.registerParticipant(15);
    open.registerParticipant(555);
    open.unregisterParticipant(555);

    mtm::ClosedEvent* clone = close.clone();
    catch_exp = false;
    try  {
        clone->registerParticipant(10);
    } catch(const mtm::AlreadyRegistered&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    catch_exp = false;
    try  {
        clone->addInvitee(200);
    } catch(const mtm::AlreadyInvited&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    
    close.addInvitee(1234567889);
    std::ofstream file;
    file.open ("TEST2_OUTPUT.txt");

    close.printShort(file);
    close.printLong(file);
    clone->printShort(file);
    clone->printLong(file);
    open.printShort(file);
    open.printLong(file);

    LOG_CYAN("EventTest Test End")

    delete clone;
}

void ContainerTest()
{
    LOG_CYAN("ContainerTest Test Start:")
    std::ofstream file;
    file.open ("TEST3_OUTPUT.txt");

    mtm::Festival festival(mtm::DateWrap(21, 10, 2020));

    bool catch_exp = false;
    try  {
        festival.add(mtm::OpenEvent(mtm::DateWrap(1, 1, 2020), "VeryBad"));
    } catch(const mtm::DateMismatch&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    festival.add(mtm::OpenEvent(mtm::DateWrap(21, 10, 2020), "Open1"));
    
    mtm::ClosedEvent closed(mtm::DateWrap(21, 10, 2020), "Closed2");
    closed.addInvitee(1); closed.addInvitee(15); closed.addInvitee(150);
    festival.add(closed);

    mtm::CustomEvent<StudentFilter> custom1(mtm::DateWrap(21,10,2020), "Custom3");
    mtm::CustomEvent<std::function<bool(int)>> custom2(mtm::DateWrap(21,10,2020), "Custom4",canRegister);

    festival.add(custom1);
    festival.add(custom2);

    printEventsShort(festival,file);

    catch_exp = false;
    try  {
        mtm::RecurringEvent<mtm::CustomEvent<StudentFilter>> recurring(mtm::DateWrap(1, 1, 2021),"BadNum", 0, 7);
    } catch(const mtm::InvalidNumber&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)
    catch_exp = false;
    try  {
        mtm::RecurringEvent<mtm::CustomEvent<StudentFilter>> recurring(mtm::DateWrap(1, 1, 2021),"BadInterval", 13, 0);
    } catch(const mtm::InvalidInterval&) { catch_exp = true; }
    TEST_IS_TRUE(catch_exp)

    mtm::RecurringEvent<mtm::CustomEvent<StudentFilter>> recurring1(mtm::DateWrap(1, 1, 2021),"CustomRecurring", 5, 7);
    printEventsShort(recurring1,file);

    mtm::OneTimeEvent<mtm::CustomEvent<StudentFilter>> one_time(mtm::DateWrap(2, 3, 80), "OneTimeCustom");
    printEventsShort(one_time,file);

    LOG_CYAN("ContainerTest Test End")
}

int main() 
{
    LOG_YELLOW("PartB Tests Begin:")

    CustomTest();
    EventTest();
    ContainerTest();

    CHECK_VLG

    LOG_YELLOW("PartB Tests End")

    return 0;
}
