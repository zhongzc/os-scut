.PHONY: all clean

CC = gcc

all: main
	./main

main: main.o barber consumer
	${CC} main.o -o main -lpthread

main.o: main.c
	${CC} main.c -c

barber: barber.o
	${CC} barber.o -o barber -lpthread

barber.o: barber.c
	${CC} barber.c -c

consumer: consumer.o
	${CC} consumer.o -o consumer -lpthread

consumer.o: consumer.c
	${CC} consumer.c -c

clean:
	rm main barber consumer *.o