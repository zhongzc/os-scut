.PHONY: all clean

CC = gcc

all: main
	./main

main: main.o barber.o common.o customer.o
	${CC} main.o barber.o customer.o common.o -o main -lpthread

main.o: main.c
	${CC} main.c -c

barber.o: barber.c
	${CC} barber.c -c

customer.o: customer.c
	${CC} customer.c -c

common.o: common.c
	${CC} common.c -c

clean:
	rm main *.o