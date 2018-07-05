#NOT WORKING!!! COMPILE MANUALLY!!!

CFLAGS=-Wall -ggdb3 -pedantic
CC=gcc
LDFLAGS=-lpthread
SOURCES = main.c not_preemptive.c listmaker.c
OBJECTS = $(SOURCES:.c=.out)
TARGET = main

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)