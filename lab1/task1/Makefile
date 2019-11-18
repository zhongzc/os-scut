.PHONY: all clean

CC = gcc

all: an_ch2_1a
	./an_ch2_1a

an_ch2_1a: an_ch2_1a.o
	${CC} an_ch2_1a.o -o an_ch2_1a

an_ch2_1a.o: an_ch2_1a.c an_ch2_1b
	${CC} an_ch2_1a.c -c

an_ch2_1b: an_ch2_1b.o
	${CC} an_ch2_1b.o -o an_ch2_1b

an_ch2_1b.o: an_ch2_1b.c
	${CC} an_ch2_1b.c -c

clean:
	rm an_ch2_1a an_ch2_1b *.o