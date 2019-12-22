.PHONY: all clean

CC = gcc

all: main
	./main

main: main.o
	${CC} main.o -o main -lpthread

main.o: main.c producer consumer
	${CC} main.c -c

producer: producer.o
	${CC} producer.o -o producer -lpthread

consumer: consumer.o
	${CC} consumer.o -o consumer -lpthread

producer.o: producer.c common.h
	${CC} producer.c -c

consumer.o: consumer.c common.h
	${CC} consumer.c -c

clean:
	rm main producer consumer *.o