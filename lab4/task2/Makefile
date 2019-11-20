.PHONY: all clean

CC = clang

all: main
	./main

main: shell.o parser.o runner.o doit.o
	${CC} shell.o parser.o runner.o doit.o -o main

shell.o: shell.c
	${CC} shell.c -c

parser.o: parser.c
	${CC} parser.c -c

runner.o: runner.c
	${CC} runner.c -c

doit.o: doit.c
	${CC} doit.c -c

clean:
	rm main *.o
