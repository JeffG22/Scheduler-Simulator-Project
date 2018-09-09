#NOT WORKING!!! COMPILE MANUALLY!!!

CFLAGS=-Wall -ggdb3 -pedantic
CC=gcc
LIBS=-pthread

all: simulator

simulator: simulator.o listmaker.o not_preemptive.o preemptive.o
	$(CC) -o $@ simulator.o listmaker.o not_preemptive.o preemptive.o $(LIBS)

simulator.o: main.c listmaker.h not_preemptive.h preemptive.h
	$(CC) $(CFLAGS) -c main.c -o $@	

listmaker.o: listmaker.c listmaker.h
	$(CC) $(CFLAGS) -c listmaker.c -o $@	

preemptive.o: preemptive.c preemptive.h listmaker.h
	$(CC) $(CFLAGS) -c preemptive.c -o $@

not_preemptive.o: not_preemptive.c not_preemptive.h listmaker.h
	$(CC) $(CFLAGS) -c not_preemptive.c -o $@