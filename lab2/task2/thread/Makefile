.PHONY: all clean

CC = clang

all: main
	./main

main: main.c
	${CC} main.c -o main -lpthread

clean:
	rm main