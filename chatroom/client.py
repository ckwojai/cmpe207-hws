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

username = args.name
while True:
        recv_data = sock.recv(1024)
        if recv_data:
            print(recv_data.decode())
        else:
            print(f"Closing sock: {sock}")
            sock.close()
        print("Me: ", end="")
        mes = input()
        sock.send(f"{username}: {mes}".encode())
