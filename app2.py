import time as time
import threading

def start():
    print("hello")

def main():
    print("hello2")

while True:
    start()
    time.sleep(3)
    main()
    time.sleep(1)
