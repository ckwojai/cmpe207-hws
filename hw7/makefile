CC=gcc
CFLAGS=-I.

client: client.o libactive.a
	$(CC) -o client client.o libactive.a

server: server.o svfuncs.o libpassive.a
	$(CC) -pthread -o server server.o libpassive.a

svfuncs.o: svfuncs.c svfuncs.h
	$(CC) -c svfuncs.c

client.o: superclient.c active.h
	$(CC) -c superclient.c -o client.o

server.o: superserver.c passive.h
	$(CC) -c superserver.c -o server.o

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

libpassive.a: passive.h passiveTCP.o passiveUDP.o passiveSocket.o errexit.o
	ar rcs libpassive.a passiveTCP.o passiveUDP.o passiveSocket.o errexit.o

passiveSocket.o: passiveSocket.c passiveSocket.h errexit.h
	$(CC) -c passiveSocket.c

passiveUDP.o: passiveUDP.c passiveSocket.h
	$(CC) -c passiveUDP.c

passiveTCP.o: passiveTCP.c passiveSocket.h
	$(CC) -c passiveTCP.c

clean:
	rm -f server client *.o *.a *.txt

libs: libactive.a libpassive.a
