#makefile variable/macros
objects = priority_queue.o priority_queue_test.o

CC=gcc
CFLAGS=-std=c99 -pedantic-errors -Wall -Wextra -g -DNDEBUG
LDFLAGS=

#rule : (prerequisites)
#   recipe....

priority_queue : $(objects)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY : all
all :
	gcc -std=c99 -pedantic-errors -Wall -Wextra -g -DNDEBUG *.o *.c -o priority_queue

.PHONY : clean
clean : cleanO
	rm -f priority_queue 

.PHONY : cleanO
cleanO :
	rm -f $(objects)

.PHONY : run
run :
	./priority_queue

.PHONY : run
runVLG :
	valgrind --leak-check=yes --track-origins=yes ./priority_queue

%.o: %.c
	@echo Compile $< to $@
	@$(CC) $(CFLAGS) -c $< -o $@