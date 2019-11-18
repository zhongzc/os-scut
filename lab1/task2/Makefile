.PHONY: all clean

CC = gcc

all: main
	./main

main: race.o
	${CC} race.o -o main -lpthread

main.o: race.c
	${CC} race.c -c

clean:
	rm main race.o