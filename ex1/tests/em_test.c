#include "test_utilities.h"
#include "../event_manager.h"
#include <stdlib.h>
#include <string.h>

#define REMOVE true

void createExpectedOutput(char* file_name, char* data)
{
    FILE* file = fopen(file_name, "w");
    fprintf(file,"%s",data);
    fclose(file);
}

bool compareFiles(char* file_name1,char* file_name2)
{
    FILE* file1 = fopen(file_name1,"r");
    FILE* file2 = fopen(file_name2,"r");
    
    if(!file1 || !file2)
    {
        return false;
    }

    char ch1 = getc(file1);
    char ch2 = getc(file2);

    while(ch1!=EOF && ch2!=EOF)
    {
        if(ch1!=ch2)
        {
            fclose(file1);
            fclose(file2);
            return false;
        }
        ch1 = getc(file1);
        ch2 = getc(file2);
    }
    fclose(file1);
    fclose(file2);
    return true;
}



bool testEventManagerCreateDestroy() {
    bool result = true;
    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    ASSERT_TEST(em != NULL, destroyEventManagerCreateDestroy);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyEventManagerCreateDestroy);
    ASSERT_TEST(emGetNextEvent(em) == NULL, destroyEventManagerCreateDestroy);

destroyEventManagerCreateDestroy:
    destroyEventManager(em);
    dateDestroy(start_date);
    return result;

}

bool testAddEventByDiffAndSize() {
    bool result = true;

    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    char* event_name = "event1";
    ASSERT_TEST(emAddEventByDiff(em, event_name, 2, 1) == EM_SUCCESS, destroyAddEventByDiffAndSize);
    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyAddEventByDiffAndSize);
    ASSERT_TEST(strcmp(event_name, emGetNextEvent(em)) == 0, destroyAddEventByDiffAndSize);

destroyAddEventByDiffAndSize:
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}

bool testEMTick() {
bool result = true;

    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    char* event_name = "event1";
    ASSERT_TEST(emAddEventByDiff(em, event_name, 1, 1) == EM_SUCCESS, destroyEMTick);

    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyEMTick);
    ASSERT_TEST(emTick(em, 2) == EM_SUCCESS, destroyEMTick);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyEMTick);
destroyEMTick:
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}


bool createAndDestroyEM()
{
    bool result = true;
    Date start_date = dateCreate(1,1,2000);
    ASSERT_TEST(start_date != NULL,destroyCreateDestroyDate); //check date creation success
    EventManager em = createEventManager(start_date);
    ASSERT_TEST(em != NULL ,destroyCreateDestroy); // check creation of event manager success

    destroyCreateDestroy:
    dateDestroy(start_date);
    destroyEventManager(em);
    destroyCreateDestroyDate:
    return result;
}

bool addEventByDateBasic()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    Date date2 = dateCreate(1,1,1999);
    EventManager em = createEventManager(date);
    dateTick(date);
    ASSERT_TEST(emAddEventByDate(em,"Party",date,1)==EM_SUCCESS,destroyEventByDate);
    ASSERT_TEST(emAddEventByDate(em,"Bar Mitzva",date,2)==EM_SUCCESS,destroyEventByDate);
    ASSERT_TEST(emGetEventsAmount(em)==2,destroyEventByDate);
    ASSERT_TEST(emAddEventByDate(em,"Bar Mitzva",date,3)==EM_EVENT_ALREADY_EXISTS,destroyEventByDate);
    ASSERT_TEST(emAddEventByDate(em,"Fun in the sun",date,2)==EM_EVENT_ID_ALREADY_EXISTS,destroyEventByDate);


    ASSERT_TEST(emAddEventByDate(em,"Fun in the sun",date2,3)==EM_INVALID_DATE,destroyEventByDate); // date is before the current date
    ASSERT_TEST(emAddEventByDate(em,"Fun in the sun",NULL,3)==EM_NULL_ARGUMENT,destroyEventByDate); // name is null
    ASSERT_TEST(emAddEventByDate(em,NULL,date2,3)==EM_NULL_ARGUMENT,destroyEventByDate); // date is null
    ASSERT_TEST(emAddEventByDate(em,"Fun in the sun",date,-1)==EM_INVALID_EVENT_ID,destroyEventByDate); // id is invalid

    destroyEventByDate:
    destroyEventManager(em);
    dateDestroy(date);
    dateDestroy(date2);
    return result;
    
}

bool addEventByDiffBasic()
{
    bool result = true;
    Date date = dateCreate(30,1,2000);
    EventManager em = createEventManager(date);
    dateTick(date);

    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"New years",1,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"ROFLCOPTER",1,3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Party",1,4)==EM_EVENT_ALREADY_EXISTS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"FUNFUNFUN",3,1)==EM_EVENT_ID_ALREADY_EXISTS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Fun in the sun",0,5)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"hello",1,-1)==EM_INVALID_EVENT_ID,destroy);
    ASSERT_TEST(emAddEventByDiff(em,NULL,1,-1)==EM_NULL_ARGUMENT,destroy);
    ASSERT_TEST(emAddEventByDiff(NULL,"NofitRules",1,6)==EM_NULL_ARGUMENT,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"ID=0",1,0)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"NofitCity Party",-2,7)==EM_INVALID_DATE,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Shalom",33,8)==EM_SUCCESS,destroy);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;

}

bool emRemoveEventTester()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    EventManager em = createEventManager(date);
    ASSERT_TEST(emRemoveEvent(em,5)==EM_EVENT_NOT_EXISTS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"New years",1,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emRemoveEvent(em,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emRemoveEvent(em,1)==EM_EVENT_NOT_EXISTS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Fun in the sun",2,3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emRemoveEvent(em,3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emRemoveEvent(NULL,1)==EM_NULL_ARGUMENT,destroy);
    ASSERT_TEST(emRemoveEvent(em,-1)==EM_INVALID_EVENT_ID,destroy);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;

}

bool ChangeEventDateTester()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    Date date2 = dateCreate(1,1,1999);
    EventManager em = createEventManager(date);
    dateTick(date);
    dateTick(date);

    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"New years",1,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emChangeEventDate(em,1,date)==EM_SUCCESS,destroy);
    for(int i = 0;i<10;i++)
    {
        dateTick(date);
    }
    ASSERT_TEST(emChangeEventDate(em,2,date)==EM_SUCCESS,destroy);
    ASSERT_TEST(emChangeEventDate(em,2,date)==EM_EVENT_ALREADY_EXISTS,destroy);
    ASSERT_TEST(emChangeEventDate(em,-2,date)==EM_INVALID_EVENT_ID,destroy);
    ASSERT_TEST(emChangeEventDate(NULL,2,date)==EM_NULL_ARGUMENT,destroy);
    ASSERT_TEST(emChangeEventDate(em,2,date2)==EM_INVALID_DATE,destroy);
    ASSERT_TEST(emChangeEventDate(em,5,date)==EM_EVENT_ID_NOT_EXISTS,destroy);
    dateTick(date);

    ASSERT_TEST(emAddEventByDate(em,"New years",date,3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emChangeEventDate(em,2,date)==EM_EVENT_ALREADY_EXISTS,destroy);

    destroy:
    dateDestroy(date);
    dateDestroy(date2);
    destroyEventManager(em);
    return result;
}

bool addMemberTests()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    EventManager em = createEventManager(date);

    ASSERT_TEST(emAddMember(em,"Oren",1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Tomer",2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Yossi",1)==EM_MEMBER_ID_ALREADY_EXISTS,destroy);
    ASSERT_TEST(emAddMember(em,"Oren",3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Avi Nimni",-1)==EM_INVALID_MEMBER_ID,destroy);
    ASSERT_TEST(emAddMember(em,"Avi Nimni",0)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(NULL,"Yaniv Kattan",4)==EM_NULL_ARGUMENT,destroy);
    ASSERT_TEST(emAddMember(em,"",4)==EM_SUCCESS,destroy); //empty name(valid?)
    ASSERT_TEST(emAddMember(em,NULL,0)==EM_NULL_ARGUMENT,destroy);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool addMemberToEventTester()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    EventManager em = createEventManager(date);
    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"PoolParty",3,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Oren",1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Tomer",2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,2,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(NULL,2,2)==EM_NULL_ARGUMENT,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,-2,2)==EM_INVALID_MEMBER_ID,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,2,-2)==EM_INVALID_EVENT_ID,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,2,7)==EM_EVENT_ID_NOT_EXISTS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,7,2)==EM_MEMBER_ID_NOT_EXISTS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,2,2)==EM_EVENT_AND_MEMBER_ALREADY_LINKED,destroy);
    ASSERT_TEST(emAddMember(em,"Shalom Assayag",3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,3,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,7,7)==EM_EVENT_ID_NOT_EXISTS,destroy); // check if the first error occures first

    createExpectedOutput("Expected_addMemberToEventEvents.txt","Party,2.1.2000,Oren,Shalom Assayag\nPoolParty,4.1.2000,Tomer\n");
    createExpectedOutput("Expected_addMemberToEventMember.txt","Oren,1\nTomer,1\nShalom Assayag,1\n");
    emPrintAllEvents(em,"addMemberToEventEvents.txt");
    emPrintAllResponsibleMembers(em,"addMemberToEventMember.txt");
    ASSERT_TEST(compareFiles("Expected_addMemberToEventEvents.txt","addMemberToEventEvents.txt"),destroy);
    ASSERT_TEST(compareFiles("Expected_addMemberToEventMember.txt","addMemberToEventMember.txt"),destroy);


    destroy:
    if(REMOVE)
    {
        remove("Expected_addMemberToEventEvents.txt");
        remove("Expected_addMemberToEventMember.txt");
        remove("addMemberToEventEvents.txt");
        remove("addMemberToEventMember.txt");
    }
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool removeMemberFromEventTester()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    EventManager em = createEventManager(date);

    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"PoolParty",3,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Oren",1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Tomer",2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Nissim",3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,2,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,3,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,3,2)==EM_SUCCESS,destroy);

    createExpectedOutput("Expected_removeMemberFromEventMembers1.txt","Nissim,2\nOren,1\nTomer,1\n");
    emPrintAllResponsibleMembers(em,"removeMemberFromEventMembers1.txt");
    ASSERT_TEST(compareFiles("Expected_removeMemberFromEventMembers1.txt","removeMemberFromEventMembers1.txt"),destroy);

    ASSERT_TEST(emRemoveMemberFromEvent(em,1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(em,1,1)==EM_EVENT_AND_MEMBER_NOT_LINKED,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(em,2,1)==EM_EVENT_AND_MEMBER_NOT_LINKED,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(em,2,-1)==EM_INVALID_EVENT_ID,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(em,-1,2)==EM_INVALID_MEMBER_ID,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(NULL,2,2)==EM_NULL_ARGUMENT,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(em,2,4)==EM_EVENT_ID_NOT_EXISTS,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(em,4,2)==EM_MEMBER_ID_NOT_EXISTS,destroy);

    createExpectedOutput("Expected_removeMemberFromEventEvents.txt","Party,2.1.2000,Nissim\nPoolParty,4.1.2000,Tomer,Nissim\n");
    createExpectedOutput("Expected_removeMemberFromEventMembers2.txt","Nissim,2\nTomer,1\n");
    emPrintAllEvents(em,"removeMemberFromEventEvents.txt");
    emPrintAllResponsibleMembers(em,"removeMemberFromEventMembers2.txt");

    ASSERT_TEST(compareFiles("Expected_removeMemberFromEventEvents.txt","removeMemberFromEventEvents.txt"),destroy);
    ASSERT_TEST(compareFiles("Expected_removeMemberFromEventMembers2.txt","removeMemberFromEventMembers2.txt"),destroy);


    destroy:
    if(REMOVE)
    {
        remove("Expected_removeMemberFromEventEvents.txt");
        remove("Expected_removeMemberFromEventMembers2.txt");
        remove("removeMemberFromEventEvents.txt");
        remove("removeMemberFromEventMembers2.txt");
        remove("Expected_removeMemberFromEventMembers1.txt");
        remove("removeMemberFromEventMembers1.txt");
    }
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool advancedCombinations()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    EventManager em = createEventManager(date);

    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"PoolParty",3,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Oren",1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Tomer",2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Nissim",3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,2,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,3,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,3,2)==EM_SUCCESS,destroy);

    ASSERT_TEST(emRemoveEvent(em,1)==EM_SUCCESS,destroy);
    createExpectedOutput("Expected_removeAdvancedEventEvents1.txt","PoolParty,4.1.2000,Tomer,Nissim\n");
    createExpectedOutput("Expected_removeAdvancedEventMembers1.txt","Tomer,1\nNissim,1\n");
    emPrintAllEvents(em,"removeAdvancedEventEvents1.txt");
    emPrintAllResponsibleMembers(em,"removeAdvancedEventMembers1.txt");

    ASSERT_TEST(compareFiles("Expected_removeAdvancedEventEvents1.txt","removeAdvancedEventEvents1.txt"),destroy);
    ASSERT_TEST(compareFiles("Expected_removeAdvancedEventMembers1.txt","removeAdvancedEventMembers1.txt"),destroy);


    ASSERT_TEST(emAddMemberToEvent(em,1,1)==EM_EVENT_ID_NOT_EXISTS,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(em,1,1)==EM_EVENT_ID_NOT_EXISTS,destroy);

    ASSERT_TEST(emAddMemberToEvent(em,1,1)==EM_EVENT_ID_NOT_EXISTS,destroy);
    ASSERT_TEST(emRemoveMemberFromEvent(em,1,1)==EM_EVENT_ID_NOT_EXISTS,destroy);

    ASSERT_TEST(emRemoveEvent(em,2)==EM_SUCCESS,destroy);

    createExpectedOutput("Expected_removeAdvancedEventEvents2.txt","");
    createExpectedOutput("Expected_removeAdvancedEventMembers2.txt","");
    emPrintAllEvents(em,"removeAdvancedEventEvents2.txt");
    emPrintAllResponsibleMembers(em,"removeAdvancedEventMembers2.txt");

    ASSERT_TEST(compareFiles("Expected_removeAdvancedEventEvents2.txt","removeAdvancedEventEvents2.txt"),destroy);
    ASSERT_TEST(compareFiles("Expected_removeAdvancedEventMembers2.txt","removeAdvancedEventMembers2.txt"),destroy);
    
    destroy:
    if(REMOVE)
    {
        remove("Expected_removeAdvancedEventEvents1.txt");
        remove("Expected_removeAdvancedEventMembers1.txt");
        remove("removeAdvancedEventEvents1.txt");
        remove("removeAdvancedEventMembers1.txt");
        remove("Expected_removeAdvancedEventEvents2.txt");
        remove("Expected_removeAdvancedEventMembers2.txt");
        remove("removeAdvancedEventEvents2.txt");
        remove("removeAdvancedEventMembers2.txt");
    }
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool emTickTester()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    EventManager em = createEventManager(date);

    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"PoolParty",3,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Oren",1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Tomer",2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMember(em,"Nissim",3)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,2,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,3,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emAddMemberToEvent(em,3,2)==EM_SUCCESS,destroy);

    ASSERT_TEST(emTick(em,1)==EM_SUCCESS,destroy); // Party should still be Active
    createExpectedOutput("Expected_emTickEvents1.txt","Party,2.1.2000,Oren,Nissim\nPoolParty,4.1.2000,Tomer,Nissim\n");
    createExpectedOutput("Expected_emTickMembers1.txt","Nissim,2\nOren,1\nTomer,1\n");
    emPrintAllEvents(em,"emTickEvents1.txt");
    emPrintAllResponsibleMembers(em,"emTickMembers1.txt");

    ASSERT_TEST(compareFiles("Expected_emTickEvents1.txt","emTickEvents1.txt"),destroy);
    ASSERT_TEST(compareFiles("Expected_emTickMembers1.txt","emTickMembers1.txt"),destroy);

    ASSERT_TEST(emTick(em,1)==EM_SUCCESS,destroy);

    createExpectedOutput("Expected_emTickEvents2.txt","PoolParty,4.1.2000,Tomer,Nissim\n");
    createExpectedOutput("Expected_emTickMembers2.txt","Tomer,1\nNissim,1\n");
    emPrintAllEvents(em,"emTickEvents2.txt");
    emPrintAllResponsibleMembers(em,"emTickMembers2.txt");

    ASSERT_TEST(compareFiles("Expected_emTickEvents2.txt","emTickEvents2.txt"),destroy);
    ASSERT_TEST(compareFiles("Expected_emTickMembers2.txt","emTickMembers2.txt"),destroy);

    ASSERT_TEST(emTick(NULL,1)==EM_NULL_ARGUMENT,destroy);
    ASSERT_TEST(emTick(em,0)==EM_INVALID_DATE,destroy); // 0 should fail too
    ASSERT_TEST(emTick(em,-1)==EM_INVALID_DATE,destroy);

    ASSERT_TEST(emAddEventByDiff(em,"BirthDay",30,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emTick(em,30)==EM_SUCCESS,destroy);

    createExpectedOutput("Expected_emTickEvents3.txt","BirthDay,3.2.2000\n");
    createExpectedOutput("Expected_emTickMembers3.txt","");
    emPrintAllEvents(em,"emTickEvents3.txt");
    emPrintAllResponsibleMembers(em,"emTickMembers3.txt");

    ASSERT_TEST(compareFiles("Expected_emTickEvents3.txt","emTickEvents3.txt"),destroy);
    ASSERT_TEST(compareFiles("Expected_emTickMembers3.txt","emTickMembers3.txt"),destroy);
    
    ASSERT_TEST(emTick(em,1)==EM_SUCCESS,destroy); // BirthDay should still be Active
    
    createExpectedOutput("Expected_emTickEvents4.txt","");
    createExpectedOutput("Expected_emTickMembers4.txt","");
    emPrintAllEvents(em,"emTickEvents4.txt");
    emPrintAllResponsibleMembers(em,"emTickMembers4.txt");

    ASSERT_TEST(compareFiles("Expected_emTickEvents4.txt","emTickEvents4.txt"),destroy);
    ASSERT_TEST(compareFiles("Expected_emTickMembers4.txt","emTickMembers4.txt"),destroy);

    destroy:
    if(REMOVE)
    {
        remove("Expected_emTickEvents1.txt");
        remove("Expected_emTickMembers1.txt");
        remove("emTickEvents1.txt");
        remove("emTickMembers1.txt");
        remove("Expected_emTickEvents2.txt");
        remove("Expected_emTickMembers2.txt");
        remove("emTickEvents2.txt");
        remove("emTickMembers2.txt");
        remove("Expected_emTickEvents3.txt");
        remove("Expected_emTickMembers3.txt");
        remove("emTickEvents3.txt");
        remove("emTickMembers3.txt");
        remove("Expected_emTickEvents4.txt");
        remove("Expected_emTickMembers4.txt");
        remove("emTickEvents4.txt");
        remove("emTickMembers4.txt");
    }
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool getEventsAmount()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    EventManager em = createEventManager(date);

    ASSERT_TEST(emGetEventsAmount(em)==0,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(emGetEventsAmount(em)==1,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"PoolParty",3,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emGetEventsAmount(em)==2,destroy);
    ASSERT_TEST(emRemoveEvent(em,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(emGetEventsAmount(em)==1,destroy);
    ASSERT_TEST(emTick(em,10)==EM_SUCCESS,destroy);
    ASSERT_TEST(emGetEventsAmount(em)==0,destroy);
    ASSERT_TEST(emGetEventsAmount(NULL)==-1,destroy);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool getNextEventTester()
{
    bool result = true;
    Date date = dateCreate(1,1,2000);
    EventManager em = createEventManager(date);

    ASSERT_TEST(emGetNextEvent(em)==NULL,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(strcmp(emGetNextEvent(em),"Party")==0,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"PoolParty",1,2)==EM_SUCCESS,destroy);
    ASSERT_TEST(strcmp(emGetNextEvent(em),"Party")==0,destroy);
    ASSERT_TEST(emRemoveEvent(em,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(strcmp(emGetNextEvent(em),"PoolParty")==0,destroy);
    ASSERT_TEST(emAddEventByDiff(em,"Party",1,1)==EM_SUCCESS,destroy);
    ASSERT_TEST(strcmp(emGetNextEvent(em),"PoolParty")==0,destroy);
    ASSERT_TEST(emTick(em,10)==EM_SUCCESS,destroy);
    ASSERT_TEST(emGetNextEvent(em)==NULL,destroy);
    

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool (*tests[]) (void) = {
        testEventManagerCreateDestroy,
        testAddEventByDiffAndSize,
        testEMTick,
        createAndDestroyEM,
        addEventByDateBasic,
        addEventByDiffBasic,
        emRemoveEventTester,
        ChangeEventDateTester,
        addMemberTests,
        addMemberToEventTester,
        removeMemberFromEventTester,
        advancedCombinations,
        emTickTester,
        getEventsAmount,
        getNextEventTester
};

const char* testNames[] = {
        "testEventManagerCreateDestroy",
        "testAddEventByDiffAndSize",
        "testEMTick",
        "CreateAndDestroyEM",
        "addEventByDateBasic",
        "addEventByDiffBasic",
        "emRemoveEventTester",
        "ChangeEventDateTester",
        "addMemberTests",
        "addMemberToEventTester",
        "removeMemberFromEventTester",
        "advancedCombinations",
        "emTickTester",
        "getEventsAmount",
        "getNextEventTester"
};

#define NUMBER_TESTS 15

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: event_manager_tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
