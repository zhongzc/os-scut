.PHONY: all clean

CC = gcc

all: main
	./main

main: main.o reader writer
	${CC} main.o -o main -lpthread

main.o: main.c
	${CC} main.c -c

reader: reader.o
	${CC} reader.o -o reader -lpthread

reader.o: reader.c
	${CC} reader.c -c

writer: writer.o
	${CC} writer.o -o writer -lpthread

writer.o: writer.c
	${CC} writer.c -c

clean:
	rm main reader writer *.o