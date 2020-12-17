#makefile variable/macros
pq_objects = priority_queue.o tests/priority_queue_tests.o
em_objects =  event_manager.o date.o tests/event_manager_tests.o event.o info.o
bin = priority_queue event_manager
CC=gcc
CFLAGS=-std=c99 -pedantic-errors -Wall -Wextra -g -DNDEBUG
LDFLAGS= -L. -lpriority_queue

#rule : (prerequisites)
#   recipe....

event_manager : $(em_objects)
	$(CC) $^ $(LDFLAGS) -o $@ 

priority_queue : $(pq_objects)
	$(CC) -o $@ $^

.PHONY : clean cleanO run runVLG

clean : cleanO
	rm -f $(bin) 

cleanO :
	rm -f $(pq_objects) $(em_objects)

run :
	./event_manager

runVLG :
	valgrind --leak-check=yes --track-origins=yes ./event_manager

%.o: %.c
	@echo Compile $< to $@
	@$(CC) $(CFLAGS) -c $< -o $@
