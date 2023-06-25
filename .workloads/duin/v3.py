import time
import socket
import hashlib
import multiprocessing
from datetime import datetime

ip = '103.253.43.245'
port = 9124

username = 'd'
key='dfds'
thread = 1


# Difficultry
diff='AVR'

# Hashrate
hashr=338

# Board
miner='Official AVR Miner'

# Version
version='3.5'



def main():

    soc=socket.socket()
    soc.connect((str('103.253.43.245'),int(9124)))
    soc.recv(100).decode()


    while True:
        soc.send(bytes(f'JOB,{username},{diff},{key}'.encode('ascii')))
        job=soc.recv(1024).decode().rstrip("\n").split(',')
        work_hash=job[0]
        target_hash=job[1]
        difficultry=job[2]

        result=0
        while True:
            result+=1
            find_hash=hashlib.sha1(str(work_hash[i]+str(result)).encode('ascii')).hexdigest()
            if find_hash==target_hash:

                soc.send(f'{result},{hashr},{miner} {version},,{None}'.encode('ascii'))
                feedback=soc.recv(1024).decode().rstrip('\n').split(',')
                if feedback[0]=='GOOD':
                    print(f"[  Thread No.\t] Hash: {find_hash}, ( Accepted! )")
                    break
                elif feedback[0]=='BAD':
                    rejected+=1
                    print(f"[  Thread No.\t] Hash: {find_hash}, ( Rejected! )")
                    break

processes = []

for x in range(int(thread)):
    p = multiprocessing.Process(target=main)
    if __name__ == '__main__':
        p.start()
        processes.append(p)


for p in processes:
    p.join()
