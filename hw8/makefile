CC=gcc
CFLAGS=-I.

hw8: main.o
	$(CC) -o hw3 main.o

main.o: main.c unp.h
	$(CC) -c main.c

clean:
	rm -f hw8 *.o
