import os
import time
import json
import socket
import threading
import urllib.request


## use this if you don' know your ip
#ipv4= json.loads(urllib.request.urlopen('https://api.ipify.org?format=json').read())['ip']
#ipv6 = json.loads(urllib.request.urlopen('https://api64.ipify.org?format=json').read())['ip']

localip = socket.gethostbyname(socket.gethostname()) ## use this if the server are the same ip
#ip = str(input("Enter the IP: "))

port = 8024 ## default
#port = int(input("Enter the Port: "))

#addr = (ipv4, port) ## use this if the server in public ipv4
#addr = (ipv6, port) ## use this if the server in public ipv6
addr = (localip, port) ## use this if the server are the same ip
#addr = (ip, port)



header = 2048
format = 'utf-8'
disconnected = "!disconnected"

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(addr)

def client(conn, addr):
    print(f"-[new connection] {addr} connected.")

    connected = True
    while connected:
        msglen = conn.recv(header).decode(format)
        if msglen:
            msglen = int(msglen)
            msg = conn.recv(msglen).decode(format)
            print(f"-[{addr}] {msg}")
            if msg == disconnected:
                connected = False
            elif msg[:4] == "cli ":
                climsg = msg[4:]
                os.system(climsg)

            conn.send("msg received".encode(format))

    conn.close()

def start():
    server.listen()
    print(f"-[listening] server is listening on {server}")
    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=client, args=(conn, addr))
        thread.start()
        print(f"-[active connection] {threading.activeCount() - 1}")


print("-[starting] server is starting...")
start()
