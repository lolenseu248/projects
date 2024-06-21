# Receiver
# @lolenseu
# https:github.com/lolenseu

# import ----------------------------------------
import _thread
import utime
import network
import espnow
import machine


# file import ----------------------------------------



# variables ----------------------------------------
# manual variable --------------------
# esp-now mymac and targetmac
my_mac = b'\x40\x22\xD8\x03\x2E\x50'
target_mac = b'\x40\x22\xD8\x08\xBB\x48'

# fix variable --------------------
# define pinout ----------
# serial pinout
RXD = 16
TXD = 17

# buzzer pinout
BUZZER = 22

# servo pinout
GPIO_trottle = 4
GPIO_yaw = 5
GPIO_pitch = 15
GPIO_roll = 21 
GPIO_mode = 23 

# define variables ----------
# uptime
time = 0

# counter
loop1 = 0
loop2 = 0

# cpuusage
core0_elapse = 0
core1_elapse = 0

# process data
trottle = 1500
yaw = 1500
pitch = 1500
roll = 1500
mode = 1540
mods = ''

# percent data
percent_speed = 0
percent_trottle = 0
percent_yaw = 0
percent_pitch = 0
percent_roll = 0

# time for ping
time1 = 1234567890

# buffer
buffer = 128

# send message
send_massage = {
    'rtime1': 1234567890,
    'rtime2': 9876543210,
    'rlen': 128,
    'rbuf': ''
}

# receivemessage
receive_massage = {
    'sthrottle': 1500,
    'syaw': 1500,
    'spitch': 1500,
    'sroll': 1500,
    'smode': 1500,
    'stime1': 1234567890,
    'stime2': 9876543210,
    'slen': 128,
    'sbuf': ''
}

# fuctions ----------------------------------------
# initboot ----------  
def initboot():
    print("\nbooting ...\n") 
    utime.sleep_ms(500)
    
# connection ----------
# init espnow
def init_espnow():
    global e
    
    # set wi-fi to station
    sta = network.WLAN(network.STA_IF)
    sta.active(True)
    sta.disconnect()
    
    # init espnow
    e = espnow.ESPNow()
    e.active(True)
    e.add_peer(target_mac)

# process ----------
# map mode
def map_mode(to_mode):
    global mods
    map_mode = map(to_mode, 1000, 2000, 1000, 2000)
    if map_mode > 1000 and map_mode < 1230: mods = "Stab"
    elif map_mode > 1231 and map_mode < 1360: mods = "PosH"
    elif map_mode > 1361 and map_mode < 1490: mods = "AltH"
    elif map_mode > 1491 and map_mode < 1621: mods = "Loit"
    elif map_mode > 1621 and map_mode < 1749: mods = "RTL "
    elif map_mode > 1750 and map_mode < 2000: mods = "Land"
    
def debug():
    pass
    
# loop ----------------------------------------
# core0 --------------------
def core0_task():
    # in global
    global loop1, time
    
    # out global
    global core0_elapse
    
    # start clock
    start_clock = utime.ticks_ms()
    
    while True:
        # core0 counter
        loop1 += 1
        if loop1 == 100: loop1 = 0
        
        # core0 load start
        core0_start = utime.ticks_ms()
        
        # clock
        current_time = utime.ticks_ms()
        if utime.ticks_diff(current_time, start_clock) >= 1000:
            start_clock = current_time
            time += 1
            
            
            
        
        # sleep prevent cpu load
        utime.sleep_ms(10)
        
        # core0 load end
        core0_elapse = utime.ticks_ms() - core0_start
        
    
# core1 --------------------
def core1_task():
     # in global
    global loop2
    
    # out global
    global core1_elapsed
    
    # start and debug time
    debug_time = utime.ticks_ms()
    
    while True:
        # core1 counter
        loop2 += 1
        if loop2 == 100: loop2 = 0
        
         # core1 load start
        core1_start = utime.ticks_ms()

        # debug ----------
        current_debug_time = utime.ticks_ms()
        if utime.ticks_diff(current_debug_time, debug_time) >= 250:
            debug_time = current_debug_time
            debug()
        
        # sleep prevent cpu load
        utime.sleep_ms(10)
        
        # core1 load end
        core1_elapsed = utime.ticks_ms() - core1_start
  
# setup ----------------------------------------
def main():
  
    #init espnow
    init_espnow()
    
    # initboot
    initboot()
    
    # sleep startup
    utime.sleep_ms(200)
    
    # threads
    _thread.start_new_thread(core0_task, ())
    core1_task()
    

# startup ----------------------------------------
if __name__ == '__main__':
    main()
    
