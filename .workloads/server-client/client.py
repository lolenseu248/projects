import os
import time
import socket
import threading


#localip = socket.gethostbyname(socket.gethostname()) ## use this if the server are the same ip
ip = str(input("Enter the IP: "))

port = 8024 ## default
#port = int(input("Enter the Port: "))

#addr = (localip, port) ## use this if the server are the same ip
addr = (ip, port)



header = 1024
format = 'utf-8'
disconnected = "!disconnected"

connected = False
if connected == False:
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(addr)
        connected = True
    except Exception as e:
        print("Error occured: " + str(e))
        exit()

def send(msg):
    sendmsg = msg.encode(format)
    msglen = len(sendmsg)
    sendlen = str(msglen).encode(format)
    sendlen += b' ' * (header - len(sendlen))
    client.send(sendlen)
    client.send(sendmsg)
    print(client.recv(header).decode(format))

while True:
    msgs = input("Send Someting: ")
    send(msgs)
    if msgs == "exit":
        send(disconnected)
        exit()
