.PHONY: all clean

CC = clang

all: main
	./main

main: main.c producer consumer
	${CC} main.c -o main -lpthread

producer: producer.c
	${CC} producer.c -o producer -lpthread

consumer: consumer.c
	${CC} consumer.c -o consumer -lpthread

clean:
	rm main producer consumer