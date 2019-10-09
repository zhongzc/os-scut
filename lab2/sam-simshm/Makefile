.PHONY: all clean

CC = clang

all: main
	./main

main: reader writer
	${CC} main.c -o main

reader:
	${CC} shm-reader.c -o reader

writer:
	${CC} shm-writer.c -o writer

clean:
	rm main reader writer