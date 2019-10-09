.PHONY: all clean

CC = clang

all: main
	./main

main: sem_chld
	${CC} main.c -o main -lpthread

sem_chld:
	${CC} sem_chld.c -o sem_chld -lpthread

clean:
	rm main sem_chld