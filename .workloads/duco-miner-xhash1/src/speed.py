
from time import sleep, perf_counter
from threading import Thread


def task1():
    count = 111111
    while True:
        
        if count == 222221:
            break
        count += 1

def task2():
    count = 222222
    while True:
        
        if count == 333332:
            break
        count += 1

def task3():
    count = 11111111
    while True:
        if count == 56844357:
            print("ok")
        
        elif count == 99999999:
            break
        count += 1


start_time = perf_counter()

# create two new threads
t1 = Thread(target=task1)
t2 = Thread(target=task2)
t3 = Thread(target=task3)

# start the threads
t1.start()
t2.start()
t3.start()

# wait for the threads to complete
t1.join()
t2.join()
t3.join()

end_time = perf_counter()

print(f'It took {end_time- start_time: 0.2f} second(s) to complete.')