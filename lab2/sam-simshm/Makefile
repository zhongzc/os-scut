.PHONY: all clean

CC = clang

all: main
	./main

main: reader writer main.c
	${CC} main.c -o main

reader: shm-reader.c
	${CC} shm-reader.c -o reader

writer: shm-writer.c
	${CC} shm-writer.c -o writer

clean:
	rm main reader writer