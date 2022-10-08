CC=gcc
CFLAGS=-I.

client: client.o libactive.a
	$(CC) -o client client.o libactive.a

server: server.o libpassive.a
	$(CC) -o server server.o libpassive.a

client.o: fileclient.c active.h
	$(CC) -c fileclient.c -o client.o

server.o: fileserver.c passive.h
	$(CC) -c fileserver.c -o server.o

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

txtfiles:
	./file.sh 51200 1_client.txt
	./file.sh 51200 2_client.txt
	./file.sh 51200 3_client.txt
	./file.sh 51200 4_client.txt
	./file.sh 51200 5_client.txt


libs: libactive.a libpassive.a
