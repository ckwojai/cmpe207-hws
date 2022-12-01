import sys
import socket
import types

import argparse

parser = argparse.ArgumentParser()
parser.add_argument('name', type=str)
args = parser.parse_args()


host, port = "127.0.0.1", 6969
server_addr = (host, port)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(server_addr)

# sock.send((f"{args.name}").encode())
# sock.send(b"Kin")

while True:
        recv_data = sock.recv(1024)
        if recv_data:
            print(recv_data.decode())
        else:
            print(f"Closing sock: {sock}")
            sock.close()
        mes = input()
        sock.send(mes.encode())
