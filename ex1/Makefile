#makefile variable/macros
pq_objects = priority_queue.o tests/priority_queue_test.o
em_objects = event_manager_test.o event_manager.o date.o 
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

.PHONY : all
all :
	gcc -std=c99 -pedantic-errors -Wall -Wextra -g -DNDEBUG *.o *.c -o priority_queue

.PHONY : clean
clean : cleanO
	rm -f $(bin) 

.PHONY : cleanO
cleanO :
	rm -f $(pq_objects) $(em_objects)

.PHONY : run
run :
	./event_manager
.PHONY : runVLG
runVLG :
	valgrind --leak-check=yes --track-origins=yes ./event_manager

%.o: %.c
	@echo Compile $< to $@
	@$(CC) $(CFLAGS) -c $< -o $@