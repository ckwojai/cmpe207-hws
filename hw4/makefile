CC=gcc
CFLAGS=-I.

client: client.o libactive.a
	$(CC) -o client client.o libactive.a

server: server.o libpassive.a
	$(CC) -o server server.o libpassive.a

client.o: client.c active.h
	$(CC) -c client.c

server.o: server.c passive.h
	$(CC) -c server.c

errexit.o: errexit.c errexit.h
	$(CC) -c errexit.c

libactive.a: active.h activeUDP.o activeTCP.o activeSocket.o errexit.o
	ar rcs libactive.a activeUDP.o activeTCP.o activeSocket.o errexit.o

activeSocket.o: activeSocket.c activeSocket.h errexit.h
	$(CC) -c activeSocket.c

activeTCP.o: activeTCP.c activeSocket.h
	$(CC) -c activeTCP.c

activeUDP.o: activeUDP.c activeSocket.h
	$(CC) -c activeUDP.c

libpassive.a: passive.h passiveUDP.o passiveSocket.o errexit.o
	ar rcs libpassive.a passiveUDP.o passiveSocket.o errexit.o

passiveSocket.o: passiveSocket.c passiveSocket.h errexit.h
	$(CC) -c passiveSocket.c

passiveUDP.o: passiveUDP.c passiveSocket.h
	$(CC) -c passiveUDP.c

clean:
	rm -f server client *.o *.a

libs: libactive.a libpassive.a
