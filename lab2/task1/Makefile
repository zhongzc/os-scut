.PHONY: all clean

CC = clang

all: main
	./main

main: producer consumer
	${CC} main.c -o main

producer:
	${CC} producer.c -o producer

consumer:
	${CC} consumer.c -o consumer

clean:
	rm main producer consumer