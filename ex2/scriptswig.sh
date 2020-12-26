#!/bin/sh

swig -python event_manager.i
gcc -std=c99 -fPIC -c event_manager_wrap.c -I/usr/local/include/python3.6m
ld -shared event_manager.o event_manager_wrap.o date.o priority_queue.o -L/usr/local/include/python3.6m/ -o _event_manager.so
