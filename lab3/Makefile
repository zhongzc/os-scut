.PHONY: all clean

CC = gcc

all: myfind

myfind: myfind.o parser.o option.o runner.o
	${CC} myfind.o parser.o option.o runner.o -o myfind

parser.o: parser.c
	${CC} parser.c -c

option.o: option.c
	${CC} option.c -c

myfind.o: myfind.c
	${CC} myfind.c -c

runner.o: runner.c
	${CC} runner.c -c

clean:
	rm myfind *.o
