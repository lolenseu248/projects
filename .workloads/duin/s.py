import socket


soc=socket.socket()
soc.connect((str('54.255.134.29'),int(8024)))

def send(msg):
    sendmsg = msg.encode(format)
    msglen = len(sendmsg)
    sendlen = str(msglen).encode(format)
    sendlen += b' ' * (1024 - len(sendlen))
    soc.send((sendlen))
    soc.send((sendmsg))
    print(soc.recv(1024).decode(format))

while True:
    msgs = input("Send Someting: ")
    send(msgs)
    if msgs == "exit":
        send("disconnected")
        exit()