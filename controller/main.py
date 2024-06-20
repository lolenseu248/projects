

# import --------------------
import _thread
import time
import sys


# file import --------------------



# variables --------------------
running = True


# fuctions --------------------
# initboot ----------  
def initboot():
    print("\nbooting ...\n") 
    time.sleep(.500)
     
# debug ----------
def debug():
    print(f"Cpu Usage\ncpu0: {core0_end}ms\ncpu1: {core1_elapsed}ms")


# core0 --------------------
def core0_task():
    global core0_end
    
    while running:
        core0_start=time.ticks_ms()
        time.sleep(1)
        core0_end=time.ticks_ms()-core0_start
        
        


# core1 --------------------
def core1_task():
    global core1_elapsed
    
    while running:
        core1_start=time.ticks_ms()
        
        
        
        core1_elapsed=time.ticks_ms()-core1_start
        # debug ----------
        if debug_time>=200:
            debug_time=time.ticks_ms()
            debug()
        
        
# setup --------------------
def main():
    global debug_time
    
    # initboot
    initboot()
    
    # sleep startup
    time.sleep(.200)
    debug_time=time.ticks_ms()
    
    # threads
    _thread.start_new_thread(core0_task, ())
    _thread.start_new_thread(core1_task, ())



# startup --------------------
if __name__ == '__main__':
    main()
    