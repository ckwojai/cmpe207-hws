# multiconn-server.py

import sys
import socket
import selectors
import types

sel = selectors.DefaultSelector()

host, port = "127.0.0.1", 6969
lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
lsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
lsock.bind((host, port))
lsock.listen()
print(f"Listening on {(host, port)}")
lsock.setblocking(False)
sel.register(lsock, selectors.EVENT_READ, data=None)

usersockmap = {}
waitingsocks = []

def accept_wrapper(sock):
    conn, addr = sock.accept()  # Should be ready to read
    print(f"Accepted connection from {addr}")
    conn.setblocking(False)
    data = types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
    events = selectors.EVENT_READ | selectors.EVENT_WRITE
    sel.register(conn, events, data=data)

    if waitingsocks:
        socka = waitingsocks.pop()
        sockb = conn
        usersockmap[socka] = sockb
        usersockmap[sockb] = socka
        socka.send(b"Connection established with user. Happy chatting.")
    else:
        waitingsocks.append(conn)


def service_connection(key, mask):
    sock = key.fileobj
    data = key.data
    if mask & selectors.EVENT_READ:
        recv_data = sock.recv(1024)  # Should be ready to read
        sockb = usersockmap[sock]
        if recv_data:
            datab = sel.get_key(sockb).data
            datab.outb += recv_data
        else:
            print(f"Closing connection to {data.addr}")
            usersockmap.pop(sock)
            sel.unregister(sock)
            sock.close()
    if mask & selectors.EVENT_WRITE:
        if data.outb:
            print(f"Sending message to the other socket: {sock.fileno()}")
            sent = sock.send(data.outb)  # Should be ready to write
            data.outb = data.outb[sent:]


try:
    while True:
        events = sel.select(timeout=None)
        for key, mask in events:
            if key.data is None: # master socket
                accept_wrapper(key.fileobj)
            else:
                if key.fileobj in usersockmap:
                    service_connection(key, mask)
except KeyboardInterrupt:
    print("Caught keyboard interrupt, exiting")
finally:
    sel.close()
