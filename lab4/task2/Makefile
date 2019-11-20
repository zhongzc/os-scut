.PHONY: all clean

CC = gcc

all: main
	./main

main: main.o
	${CC} shell.o -o main

main.o: shell.c
	${CC} shell.c -c

clean:
	rm main *.o