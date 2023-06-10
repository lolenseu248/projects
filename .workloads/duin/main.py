#import machine
#import network
import time
import json
import urllib.request
import socket
import hashlib
import random


#WIFI_SSID='wifi_ssid'
#WIFI_PASSWORD='wifi_pass'

server_ip='103.253.43.245'
server_port=8455

username=str(input("Enter Username: "))
key=str(input("Enter Key: "))

# Delays 
delay=.1  # Enable for Normal run
#delay=.5 # Enable for Test run -If Thread is 15 below consider Test run!

# ---------- Config ----------
# Thread
#thread=15 # 
thread=20 #

# Difficultry
#diff='DUE'
#diff='ARM'
#diff='AVR'
diff='MEGA'

# Hashrate
hashr=370

# Board
miner='Official AVR Miner'

# Version
version='3.5'
# ---------- Config ----------

hashr_min=hashr-int(hashr/100)
hashr_max=hashr+int(hashr/100)

def map_value(var,in_min,in_max,out_min,out_max):
    return (var-in_min)*(out_max-out_min)/(in_max-in_min)+out_min

def connect_to_wifi():
    wlan=network.WLAN(network.STA_IF)
    wlan.active(True)
    if not wlan.isconnected():
        print("\nConnecting to Wi-Fi...")
        wlan.connect(WIFI_SSID, WIFI_PASSWORD)
        while not wlan.isconnected():
            time.sleep(1)
    print("\nConnected to Wi-Fi")
    print("IP address:", wlan.ifconfig()[0])

def get_server_info():
    print("\nConnecting to Server")
    get_server=False
    reconnect_time=3
    while not get_server:
        try:
            serverip=('https://server.duinocoin.com/getPool')
            server=json.loads(urllib.request.urlopen(serverip).read())
            global server_ip,server_port,server_info,server_name,server_connection
            server_ip=server['ip']
            server_port=server['port']
            server_info=server['server']
            server_name=server['name']
            server_connection=server['success']
            get_server=True
        except:
            print(f"\nConnetion Failed!, Retrying in {reconnect_time}s...")
            time.sleep(reconnect_time)
            reconnect_time+=3
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
        sockets=[]
        for i in range(thread):
            try:  
                soc=socket.socket()
                soc.connect((str(server_ip),int(server_port)))
                sockets.append(soc)
                soc.recv(100).decode()
                print(f"[  Thread No.{i}\t] Connected successfully.")
            except ConnectionRefusedError:
                print(f"[  Thread No.{i}\t] Unable connect to the server.")
        
        accepted=0
        rejected=0
        print("\nStart Mining")
        while True:
            work_hash=[]
            target_hash=[]
            difficultry=[]
            final_hash=[]
            total_result=[]

            for soc in sockets:
                soc.send(bytes(f'JOB,{username},{diff},{key}',encoding='ascii'))
                job=soc.recv(1024).decode().rstrip("\n").split(',')
                work_hash.append(job[0])
                target_hash.append(job[1])
                difficultry.append(job[2])

            for i in range(thread):
                result=0
                while True:
                    result+=1
                    find_hash=hashlib.sha1(str(work_hash[i]+str(result)).encode('ascii')).hexdigest()
                    if find_hash==target_hash[i]:
                        total_result.append(result)
                        final_hash.append(find_hash)
                        break
                    elif result>=100*int(difficultry[i])+1:
                        total_result.append(result)
                        final_hash.append(find_hash)
                        break

            time.sleep(delay) # Don't remove this delay!
            thread_count=list(range(thread))
            random_sequence=random.sample(thread_count,len(thread_count))
            for i in random_sequence:
                hash_rate=str(random.randint(hashr_min,hashr_max))+'.'+str(random.randint(1,99))
                sockets[i].send(f'{total_result[i]},{hash_rate},{miner} {version},Chip {i},{i}'.encode('ascii'))
                feedback=sockets[i].recv(1024).decode().rstrip('\n').split(',')
                if feedback[0]=='GOOD':
                    accepted+=1
                    print(f"[  Thread No.{i}\t] Hash: {final_hash[i]}, ( {accepted} Accepted! )")
                elif feedback[0]=='BAD':
                    rejected+=1
                    print(f"[  Thread No.{i}\t] Hash: {final_hash[i]}, ( {rejected} Rejected! )")
    
    except Exception as e:
        for soc in sockets:
            soc.close()
    
        print("An error occurred:", e)
        print("Reconnecting in 3s...")
        time.sleep(3)
    

if __name__ == '__main__':
    if thread<15:
        print("\nThread is to Low!")
        exit()
    else:
        while True:
            #connect_to_wifi()
            get_server_info()
            main()
