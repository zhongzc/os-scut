.PHONY: all clean

CC = gcc

all: main
	./main

main: main.o
	${CC} main.o -o main -lpthread

main.o: main.c
	${CC} main.c -c

clean:
	rm main *.o