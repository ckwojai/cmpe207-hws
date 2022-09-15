CC=gcc
CFLAGS=-I.

hw3: main.o libactive.a
	$(CC) -o hw3 main.o libactive.a

main.o: main.c active.h
	$(CC) -c main.c
     # $(CC) -I . -c main.c

activeSocket.o: activeSocket.c activeSocket.h errexit.h
	$(CC) -c activeSocket.c

errexit.o: errexit.c errexit.h
	$(CC) -c errexit.c

activeTCP.o: activeTCP.c activeSocket.h
	$(CC) -c activeTCP.c

libactive.a: active.h activeTCP.o activeSocket.o errexit.o
	ar rcs libactive.a activeTCP.o activeSocket.o errexit.o

clean:
	rm -f

libs: libactive.a
