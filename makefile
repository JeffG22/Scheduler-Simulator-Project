CFLAGS=-Wall -ggdb3 -pedantic
CC=gcc

all: main.out

main.out: main.c
	$(CC) $(CFLAGS) $< -o $@