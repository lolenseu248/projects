#import machine
import network
import time
import json
import urllib.request
import socket
import hashlib
import random


WIFI_SSID = "lolenseu"
WIFI_PASSWORD = "@lolenseu24!"

server_ip='103.253.43.245'
server_port=8455

thread=20 # Recommended is 10

username="soup"
key="0x00"

# Hashrate
#hashr=12000
#hashr=6000
#hashr=4500
#hashr=1000
#hashr=520
hashr=370

# Difficultry
#diff="ESP32"
#diff="ESP8266H"
#diff="ESP8266"
#diff="DUE"
#diff="ARM"
diff="MEGA"

# Board
#miner="Custom Duino ASIC Miner"
#miner="Official ESP32 Miner"
#miner="Official ESP8266 Miner"
#miner="Official ARM Miner"
miner="Official AVR Miner"

# Version
#version="1.0"
version="3.5"

sockets=[]

def map_value(var,in_min,in_max,out_min,out_max):
    return (var-in_min)*(out_max-out_min)/(in_max-in_min)+out_min

def connect_to_wifi():
    wlan=network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print("Connecting to Wi-Fi...")
        wlan.connect(WIFI_SSID, WIFI_PASSWORD)
        while not wlan.isconnected():
            time.sleep(1)
    print("Connected to Wi-Fi")
    print("IP address:", wlan.ifconfig()[0])

def get_server_info():
    print("\nConnecting to Server")
    get_server=False
    while not get_server:
        try:
            serverip=("https://server.duinocoin.com/getPool")
            server=json.loads(urllib.request.urlopen(serverip).read())
            global server_ip,server_port,server_info,server_name,server_connection
            server_ip=server['ip']
            server_port=server['port']
            server_info=server['server']
            server_name=server['name']
            server_connection=server['success']
            get_server=True
        except:
            print("\nConnetion Failed!, Retrying...")
            time.sleep(2)
            continue

    print("\nServer Details")
    print(f"IP: {server_ip}")
    print(f"Port: {server_port}")
    print(f"Info: {server_info}")
    print(f"Name: {server_name}")
    print(f"Server Alive: {server_connection}")
    
def main():
    try:
        print("\nEstablish Threads")
        for i in range(thread):
            try:  
                soc=socket.socket()
                soc.connect((str(server_ip),int(server_port)))
                sockets.append(soc)
                soc.recv(100).decode()
                print(f"[  Thread No.{i}\t] Connected successfully.")
            except ConnectionRefusedError:
                print(f"[  Thread No.{i}\t] Unable connect to the server.")
        
        print("\nStart Mining")
        accepted=0
        rejected=0

        while True:
            works=[]
            target_hash=[]
            main_hash_storage=[]
            temp_hash_storage=[]
            total_hash=[]

            for soc in sockets:
                soc.send(bytes(f"JOB,{username},{diff},{key}",encoding="ascii"))
                hashes=soc.recv(1024).decode().rstrip("\n").split(",")
                works.append(hashes[0])
                target_hash.append(hashes[1])

            for i in range(thread):
                main_hash=hashlib.sha1(str(works[i]).encode('ascii'))
                temp_hash=None
                main_hash_storage.append(main_hash) 
                temp_hash_storage.append(temp_hash)

            thread_count=list(range(thread))
            random_sequence=random.sample(thread_count,len(thread_count))
            for i in random_sequence:
                solve_hash=works[i]
                tohash=0
                while True:
                    if tohash>=100*hashr:
                        tohash=0
                    else:
                        tohash+=1

                    solve_hash=main_hash_storage[i].copy()
                    solve_hash.update(str(tohash).encode('ascii'))
                    final_hash=solve_hash.hexdigest()
                    if final_hash==target_hash[i]:
                        total_hash.append(tohash)
                        break
                
                time.sleep(map_value(thread,1,10,1,.1)) # Don't remove this delay!
                sockets[i].send(f"{tohash},{hashr},{miner} {version},Chip {i},{i}".encode('ascii'))
                feedback=sockets[i].recv(1024).decode().rstrip("\n").split(",")
                if feedback[0]=='GOOD':
                    accepted+=1
                    print(f"[  Thread No.{i}\t] Hash: {final_hash}, ( {accepted} Accepted! )")
                elif feedback[0]=='BAD':
                    rejected+=1
                    print(f"[  Thread No.{i}\t] Hash: {final_hash}, ( {rejected} Rejected! )")


    except Exception as e:
        for i in range(thread):
            soc[i].close()
            
        print("An error occurred:", e)
        print("Reconnecting in 3s...")
        time.sleep(3)


if __name__ == '__main__':
    while True:
        #connect_to_wifi()
        get_server_info()
        main()
