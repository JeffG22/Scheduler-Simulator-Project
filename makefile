CFLAGS=-Wall -ggdb3 -pedantic
CC=gcc

all: 
parameters: parameters.c
	$(CC) $(CFLAGS) $< -o $@