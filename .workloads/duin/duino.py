import time
import requests
import json
import socket
import multiprocessing
import hashlib
import random


# ---------- Config ----------
# Manual Server
server_ip='103.253.43.245' # Singapore Server

# Uncomment only one
#target_port='LOW' # If low port betwen 4999 to 1000
target_port='HIGH' # If high port betwen 9999 to 5000


# Thread
thread=15# Below 12 is not allowed

# Wallet Username and key
username,key='0xcacdcabdccchbdd0','0xcacdca'


# Simulation Speed,Difficultry,Hashrate, and Miner (Device to Simulate)
# AVR
speed,diff,hashr,miner,version='SLOW','AVR',258,'AVR I2C','3.5'                              # AVR I2C
#speed,diff,hashr,miner,version='SLOW','AVR',338,'Official AVR Miner','3.5'                   # Official AVR Miner

# ESP
#speed,diff,hashr,miner,version='FAST','ESP8266N',12000,'Official ESP8266 Miner','3.5'        # Official ESP8266 Miner
#speed,diff,hashr,miner,version='FAST','ESP32',18000,'Official ESP32 Miner','3.5'             # Official ESP32 Miner
# ---------- Config ----------


hashr_min=hashr-int(hashr/100)
hashr_max=hashr+int(hashr/100)

def get_server_info():
    print("\nConnecting to Server")
    server_connection=False
    reconnect_time=3
    while not server_connection:
        try:
            get_server=('https://server.duinocoin.com/getPool')
            server=json.loads(requests.get(get_server).text)
            
            global server_ip,server_port,server_info,server_name
            server_ip=server['ip']
            server_port=server['port']
            server_info=server['server']
            server_name=server['name']
            server_connection=server['success']
            if server_connection==True:
                print("Connection Successful")
            
        except:
            if reconnect_time>15:
                print("\nUnable to reach the server!")
                reconnect_time=3
                continue

            print(f"Connetion Failed!, Retrying in {reconnect_time}s...")
            time.sleep(reconnect_time)
            reconnect_time+=3
            continue

    print("\nServer Details")
    print(f"IP: {server_ip}")
    print(f"Port: {server_port}")
    print(f"Info: {server_info}")
    print(f"Name: {server_name}")
    print(f"Server Alive: {server_connection}")

def find_port():
    if target_port=='LOW':
        find_port=4999
    elif target_port=='HIGH':
        find_port=9999

    print("\nFinding Server Port")
    print(f"IP: {server_ip}")
    while True:
        try:
            soc_port=socket.socket()
            soc_port.connect((str(server_ip),int(find_port)))
            break
        
        except:
            soc_port.close()
            if target_port=='LOW':
                if find_port<=1000:
                    find_port=4999
                else:
                    find_port-=1

            elif target_port=='HIGH':
                if find_port<=5000:
                    find_port=9999
                else:
                    find_port-=1
            
            continue

    soc_port.close()
    print(f"Port: {find_port}")
    return find_port

def solver():
    for result in range(100*int(difficulty[x])+1):
        find_hash=hashlib.sha1(str(work_hash[x]+str(result)).encode('ascii')).hexdigest()
        if find_hash==target_hash[x]:
            final_hash[int(x)]=str(find_hash)
            total_result[int(x)]=str(result)
            break
  
def map_value(var,in_min,in_max,out_min,out_max):
    return (var-in_min)*(out_max-out_min)/(in_max-in_min)+out_min
    
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
                print(f"[  Thread {i}\t] Connected successfully.")

            except ConnectionRefusedError:
                print(f"[  Thread {i}\t] Unable connect to the server.")
        
        accepted=0
        rejected=0
        print("\nStart Mining")
        while True:
            global work_hash,target_hash,difficulty
            work_hash=[]
            target_hash=[]
            difficulty=[]
            for soc in sockets:
                soc.send(bytes(f'JOB,{username},{diff},{key}'.encode('ascii')))
                job=soc.recv(1024).decode().rstrip("\n").split(',')
                work_hash.append(job[0])
                target_hash.append(job[1])
                difficulty.append(job[2])

            global final_hash,total_result,x
            start_calc=time.time()
            total_result=multiprocessing.Manager().dict()
            final_hash=multiprocessing.Manager().dict()
            if speed=='SLOW':
                for x in range(thread):
                    solver()
            elif speed=='FAST':
                processes=[]
                for x in range(thread):
                    p=multiprocessing.Process(target=solver)
                    p.start()
                    processes.append(p)

                for p in processes:
                    p.join()
            
            end_calc=time.time()
            calc_time=end_calc-start_calc
            #print(calc_time)
            if calc_time>.5:
                pass
            else:
                #time.sleep(.5-calc_time) # Uncomment if Thread is below 12 else sus
                pass

            thread_count=list(range(thread))
            random_sequence=random.sample(thread_count,len(thread_count))
            for i in random_sequence:
                hash_rate=random.uniform(hashr_min,hashr_max)
                sockets[i].send(f'{total_result[i]},{hash_rate},{miner} {version},,{None}'.encode('ascii'))
                feedback=sockets[i].recv(1024).decode().rstrip('\n').split(',')
                if feedback[0]=='GOOD':
                    accepted+=1
                    print(f"[  Thread {i}\t] Hash: {final_hash[i]}, {accepted} Accepted!")
                elif feedback[0]=='BAD':
                    rejected+=1
                    print(f"[  Thread {i}\t] Hash: {final_hash[i]}, {rejected} Rejected!")
    
    except Exception as e:
        for soc in sockets:
            soc.close()
    
        print("An error occurred:", e)
        print("Reconnecting in 3s...")
        time.sleep(3)
    

if __name__ == '__main__':
    if thread<12:
        print("\nThread is to Low!")
        exit()
    else:
        while True:
            # Uncomment only one
            #get_server_info() # For Auto Server
            server_port=find_port() # For Manual Server

            main()
