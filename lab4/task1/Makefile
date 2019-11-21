.PHONY: all clean

CC = gcc

all: main

main: doit.o
	${CC} doit.o -o doit

doit.o: doit.c
	${CC} doit.c -c

clean:
	rm doit *.o