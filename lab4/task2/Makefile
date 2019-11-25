.PHONY: all clean

CC = gcc

all: main
	command -v rlwrap >/dev/null 2>&1 && rlwrap -c ./main || echo WARNING: install rlwrap for the better experience; ./main

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
