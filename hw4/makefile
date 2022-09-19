CC=gcc
CFLAGS=-I.

hw3: main.o libactive.a
	$(CC) -o hw3 main.o libactive.a

main.o: main.c active.h
	$(CC) -c main.c


libactive.a: active.h activeTCP.o activeSocket.o errexit.o
	ar rcs libactive.a activeTCP.o activeSocket.o errexit.o

activeSocket.o: activeSocket.c activeSocket.h errexit.h
	$(CC) -c activeSocket.c

errexit.o: errexit.c errexit.h
	$(CC) -c errexit.c

activeTCP.o: activeTCP.c activeSocket.h
	$(CC) -c activeTCP.c


libpassive.a: passive.h passiveUDP.o passiveSocket.o errexit.o
	ar rcs libpassive.a passiveUDP.o passiveSocket.o errexit.o

passiveSocket.o: passiveSocket.c passiveSocket.h errexit.h
	$(CC) -c passiveSocket.c

clean:
	rm -f hw4 main.o libactive.a activeSocket.o errexit.o activeTCP.o libpassive.a passiveSocket.o

libs: libactive.a libpassive.a
