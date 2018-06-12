#NOT WORKING!!! COMPILE MANUALLY!!!

CFLAGS=-Wall -ggdb3 -pedantic
CC=gcc

all: listmaker main.out

main.out: main.c
	$(CC) $(CFLAGS) $< -o $@

listmaker: listmaker.c
	$(CC) $(CFLAGS) $<

preemptive: preemptive.c
	$(CC) $(CFLAGS) $<

not_preemptive: not_preemptive.c
	$(CC) $(CFLAGS) $<