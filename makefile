CFLAGS=-Wall -ggdb3 -pedantic
CC=gcc

all: parameters.out

parameters.out: parameters.c
	$(CC) $(CFLAGS) $< -o $@