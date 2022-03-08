import os
import sys
import time
import json
import thread
import socket
import hashlib
import urllib.request

soc = socket.socket()
soc.settimeout(None)

username = "test"
wallet = "test1"

def getpoolinfo():
    try:
        global address, port
        serverip = ("https://server.duinocoin.com/getPool")
        poolinfo = json.loads(urllib.request.urlopen(serverip).read())
        print(poolinfo) # DEBUG:
        address = poolinfo['ip']
        port = poolinfo['port']
    except:
        print("Retrieving ip and port")
        time.sleep(3)
        getpoolinfo()


getpoolinfo()
def start():
    connect = soc.connect((str(address), int(port)))
    soc.recv(3).decode()
    thread.thread(connect)

    print("ok") # DEBUG:

while True:
    start()
