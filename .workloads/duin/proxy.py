import socket

find_port=9999
while True:
    try:
        soc=socket.socket()
        soc.connect((str('103.253.43.245'),int(find_port)))
        break
    except:
        soc.close()
        find_port-=1
        print(find_port)
        continue

print(find_port)
