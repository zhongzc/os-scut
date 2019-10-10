.PHONY: all clean

CC = clang

all: main
	./main

main: main.c reader writer
	${CC} main.c -o main -lpthread

reader: reader.c
	${CC} reader.c -o reader -lpthread

writer: writer.c
	${CC} writer.c -o writer -lpthread

clean:
	rm main reader writer