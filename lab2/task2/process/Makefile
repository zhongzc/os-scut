.PHONY: all clean

CC = clang

all: main
	./main

main: main.c barber consumer
	${CC} main.c -o main -lpthread

barber: barber.c
	${CC} barber.c -o barber -lpthread

consumer: consumer.c
	${CC} consumer.c -o consumer -lpthread

clean:
	rm main barber consumer