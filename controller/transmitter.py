# Transmitter
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
my_mac = b'\x40\x22\xD8\x08\xBB\x48'
target_mac = b'\x40\x22\xD8\x03\x2E\x50'

# fix variable --------------------
# define pinout ----------
pin_toggle_switch1 = 19 
pin_toggle_switch2 = 18
pin_toggle_switch3 = 2
pin_toggle_switch4 = 15

# joystick pinout
# joystick1
pin_joystick_switch1 = 25
pin_joystick_x1 = 33
pin_joystick_y1 = 32

# joystick2
pin_joystick_switch2 = 26
pin_joystick_x2 = 34
pin_joystick_y2 = 35

# potentiometer pinout
pin_potentiometer1 = 36
pin_potentiometer2 = 39

# define variables ----------
# uptime
time = 0

# counter
loop1 = 0
loop2 = 0

# cpuusage
core0_elapse = 0
core1_elapse = 0

# raw data
# toggle inputs
toggle_switch1_state = 0
toggle_switch2_state = 0
toggle_switch3_state = 0
toggle_switch4_state = 0

# joystic input
joystick_switch1_state = 0
joystick_switch1_state = 0
joystick_x1_position = 0
joystick_y1_position = 0
joystick_x2_position = 0
joystick_y2_position = 0

# joystic prefix
joystick_x1_positions = 0
joystick_y1_positions = 0
joystick_x2_positions = 0
joystick_y2_positions = 0

# potentiometer inputs
potentiometer1_position = 0
potentiometer2_position = 0

# potentiometer prefix
potentiometer1_positions = 0
potentiometer2_positions = 0

# mapped data
# joystic map
joystick_x1_positionss = 0
joystick_y1_positionss = 0
joystick_x2_positionss = 0
joystick_y2_positionss = 0

# potentiometer map
potentiometer1_positionss = 0
potentiometer2_positionss = 0

# joystic2 speed ajust
calc_low = 1500
calc_high = 1500

# capture trottle
capture_trottle = 1500

# current trottle
current_trottle = 1720

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

# receive message
receive_massage = {
    'rtime1': 1234567890,
    'rtime2': 9876543210,
    'rlen': 128,
    'rbuf': ''
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
# mapper
def map(x, in_min, in_max, out_min, out_max):
    mapped = int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
    return mapped

# to map value
def set_map(to_map):
    sub_map = map(to_map, 0, 1023, 0, 225) # fix the joystic input because joystic is not centerd to 511
    map_value = map(to_map,0, 1023, 1000, 2225 - sub_map)
    
    # default mapping if the joystic centerd to 2048
    #map_value = map(to_map, 0, 1023, 1000, 2000)
    return map_value

# map speed
def map_speed(to_speed):
    global calc_low, calc_high
    calc_low = 1500 - map(to_speed, 1000,2000, 0, 500)
    calc_high = 1500 + map(to_speed, 1000, 2000, 0, 500)
     
# to set official  data
# set trottle
def set_trottle(to_trottle):
    global trottle 
    if to_trottle <= 1200: trottle -= 5
    if to_trottle >= 1800: trottle += 5
    if trottle <= 1000: trottle = 1000
    if trottle >= 2000: trottle = 1700
    return trottle

# set trottle in mode
def set_trottle_in_mode(to_trottle):
    global trottle
    trottle=1500
    if to_trottle <= 1200: trottle = 1350
    if to_trottle >= 1800: trottle = 1650
    return trottle

# set yaw
def set_yaw(to_yaw):
    global yaw
    if to_yaw == 1500 or (1450 <= to_yaw <= 1550): yaw = 1500
    if to_yaw <= 1200: yaw = calc_low
    if to_yaw >= 1800: yaw = calc_high
    return yaw

# set pitch
def set_pitch(to_pitch):
    global pitch
    if to_pitch == 1500 or (1450 <= to_pitch <= 1550): pitch = 1500
    if to_pitch <= 1200: pitch = calc_low
    if to_pitch >= 1800: pitch = calc_high
    return pitch

# set roll
def set_roll(to_roll):
    global roll
    if to_roll == 1500 or (1450 <= to_roll <= 1550): roll = 1500
    if to_roll <= 1200: roll = calc_low
    if to_roll >= 1800: roll = calc_high
    return roll

# map to percent
def map_percent(to_map_percent):
    map_value_percent = map(to_map_percent, 1000, 2000, 0, 100)
    return map_value_percent

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
    
# espnow callback ----------
def on_data_sent(target_mac, data):
    e.send(target_mac, data)

def on_data_recv(target_mac, data):pass


# debug ----------
def debug():
    print("\n")
    print("-------------------- debug --------------------")
    print(f"ESP-NOW\nCom Status: {1}\nping: {1}ms")
    print("\n")
    #print(f"Raw Data\nJoystick no.1 X= {joystick_x1_positions}, Y= {joystick_y1_positions}, Sw= {joystick_switch1_state}\nJoystick no.2 X= {joystick_x2_positions}, Y= {joystick_y2_positions}, Sw= {joystick_switch2_state}\nPotentiometer no.1= {potentiometer1_positions}\nPotentiometer no.2= {potentiometer2_positions}")
    #print("\n")
    #print(f"Mapped Data\nJoystick no.1 X= {joystick_x1_positionss}, Y= {joystick_y1_positionss}, Sw= {joystick_switch1_state}\nJoystick no.2 X= {joystick_x2_positionss}, Y= {joystick_y2_positionss}, Sw= {joystick_switch2_state}\nPotentiometer no.1= {potentiometer1_positionss}\nPotentiometer no.2= {potentiometer2_positionss}")
    #print("\n")
    #print(f"Switch\nJoystic no. 1= {joystick_switch1_state}\nJoystic no. 2= {joystick_switch2_state}\nToggle no. 1= {toggle_switch1_state}\nToggle no. 2= {toggle_switch2_state}\nToggle no. 3= {toggle_switch3_state}\nToggle no. 4= {toggle_switch4_state}\n")
    #print("\n")
    print(f"Official Data\nSpeed: {percent_speed}%\nTrottle: {percent_trottle}%\nYaw: {percent_yaw}%\nPitch: {percent_pitch}%\nRoll: {percent_roll}%\nMode: {mods}")
    print("\n")
    print(f"Cpu Usage\ncpu0: {core0_elapse}ms\ncore0: {core1_elapsed}ms")
    print("\n")
    print(f"Uptime: {time}sec")
    print("-------------------- debug --------------------")
    print("\n")
    
# loop ----------------------------------------
# core0 --------------------
def core0_task():
    # in global
    global loop1, time
    global toggle_switch1, toggle_switch2, toggle_switch3, toggle_switch4
    global joystick_switch1, joystick_switch2
    global joystick_x1_position, joystick_y1_position, joystick_x2_position, joystick_y2_position
    global potentiometer1_position, potentiometer2_position
    global current_trottle, capture_trottle
    
    # out global
    global core0_elapse
    global toggle_switch1_state, toggle_switch2_state, toggle_switch3_state, toggle_switch4_state
    global joystick_switch1_state, joystick_switch2_state
    global joystick_x1_positions, joystick_y1_positions, joystick_x2_positions, joystick_y2_positions
    global potentiometer1_positions, potentiometer2_positions
    global joystick_x1_positionss, joystick_y1_positionss, joystick_x2_positionss, joystick_y2_positionss
    global potentiometer1_positionss, potentiometer2_positionss
    global trottle, yaw, pitch, roll, mode
    global percent_speed, percent_trottle, percent_yaw, percent_pitch, percent_roll
     
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
        
        # procces ----------
        # raw data
        # read toglle input value
        toggle_switch1_state = toggle_switch1.value()
        toggle_switch2_state = toggle_switch2.value()
        toggle_switch3_state = toggle_switch3.value()
        toggle_switch4_state = toggle_switch4.value()
        
        # read X,Y and SW analog values of joystic no.1
        joystick_switch1_state = joystick_switch1.value()
        joystick_x1_positions = joystick_x1_position.read()
        joystick_y1_positions = joystick_y1_position.read()
        
        # read X,Y and SW analog values of joystic no.2
        joystick_switch2_state = joystick_switch2.value()
        joystick_x2_positions = joystick_x2_position.read()
        joystick_y2_positions = joystick_y2_position.read()
        
        # read potentiometer analog values
        potentiometer1_positions = potentiometer1_position.read()
        potentiometer2_positions = potentiometer2_position.read()
        
        # mapped data
        # mapped joystic values of joystic no.1
        joystick_x1_positionss = set_map(joystick_x1_positions)
        joystick_y1_positionss = set_map(joystick_y1_positions)
        
        # mapped joystic values of joystic no.2
        joystick_x2_positionss = set_map(joystick_x2_positions)
        joystick_y2_positionss = set_map(joystick_y2_positions)
        
        # mapped potentiometer
        potentiometer1_positionss = set_map(potentiometer1_positions)
        potentiometer2_positionss = set_map(potentiometer2_positions)
        
        # map mode to string
        map_speed(potentiometer2_positionss);
        
        #  prepare for send message
        if toggle_switch2_state == True:
            yaw = set_yaw(joystick_y1_positionss)
            pitch = set_pitch(joystick_x2_positionss)
            roll = set_roll(joystick_y2_positionss)
            
            # for the modes
            if toggle_switch1_state == True:
                if toggle_switch4_state == True:
                    trottle = set_trottle_in_mode(joystick_x1_positionss)
                    mode = 1550 # loiter
                
                elif toggle_switch4_state == False:
                    trottle = set_trottle_in_mode(joystick_x1_positionss)
                    mode = 1400 # alt hold
                
            elif toggle_switch1_state == False:
                if toggle_switch4_state == True:
                    trottle = set_trottle_in_mode(joystick_x1_positionss)
                    mode = 1820 # land
                    
                elif toggle_switch4_state == False:
                    trottle = current_trottle
                    
                    # return trottle
                    if joystick_switch1_state == False:
                        trottle = capture_trottle
                        
                    # capture trottle
                    if joystick_switch2_state == False:
                        capture_trottle = set_trottle(joystick_x1_positionss)
                        
                    current_trottle = set_trottle(joystick_x1_positionss) # set trottle only on knob or stab
                    mode = potentiometer1_positionss # fix by knob 
        
        elif toggle_switch2_state == False:
            current_trottle = joystick_x1_positionss
            trottle = current_trottle
            yaw = joystick_y1_positionss
            pitch = joystick_x2_positionss
            roll = joystick_y2_positionss
            
            if toggle_switch1_state == True:
                if toggle_switch4_state == True:
                    mode = 1550 # loiter
                    
                elif toggle_switch4_state == False:
                    mode = 1400 # alt hold
                    
            elif toggle_switch1_state == False:
                if toggle_switch4_state == True:
                    mode = 1820 # land
                    
                elif toggle_switch4_state == False:
                    mode = 1100 # fix stabilize
            
        # fix yaw position 
        yaw = map(yaw, 1000, 2000, 2000, 1000);
        
        # rcv msg
        #e.on_recv(on_data_recv())
        
        # rcv ping
        
        
        # ping from uav
        if toggle_switch3_state == True: pass
        elif toggle_switch3_state == False: pass
        
        # ping
        time1 = utime.ticks_ms()
        
        # snd msg
        data_to_send = {
            'sthrottle': trottle,
            'syaw': yaw,
            'spitch': pitch,
            'sroll': roll,
            'smode': mode,
            'stime1': time1,
            'stime2': 9876543210,
            'slen': 20,
            'sbuf': ''
        }
            
        # percent data
        percent_speed = map_percent(potentiometer2_positionss)
        percent_trottle = map_percent(trottle)
        percent_yaw = map_percent(yaw)
        percent_pitch = map_percent(pitch)
        percent_roll = map_percent(roll)
        map_mode(mode)
        
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
    # in global
    global pin_toggle_switch1, pin_toggle_switch2, pin_toggle_switch3, pin_toggle_switch4
    global pin_joystick_switch1, pin_joystick_switch2
    global pin_joystick_x1, pin_joystick_y1, pin_joystick_x2, pin_joystick_y2
    global pin_potentiometer1, pin_potentiometer2
    
    # out global
    global toggle_switch1, toggle_switch2, toggle_switch3, toggle_switch4
    global joystick_switch1, joystick_switch2
    global joystick_x1_position, joystick_y1_position, joystick_x2_position, joystick_y2_position
    global potentiometer1_position, potentiometer2_position
    
    # init toggle switch
    toggle_switch1 = machine.Pin(pin_toggle_switch1, machine.Pin.IN, machine.Pin.PULL_UP)
    toggle_switch2 = machine.Pin(pin_toggle_switch2, machine.Pin.IN, machine.Pin.PULL_UP)
    toggle_switch3 = machine.Pin(pin_toggle_switch3, machine.Pin.IN, machine.Pin.PULL_UP)
    toggle_switch4 = machine.Pin(pin_toggle_switch4, machine.Pin.IN, machine.Pin.PULL_UP)
    
    # init joystick switch
    joystick_switch1 = machine.Pin(pin_joystick_switch1, machine.Pin.IN, machine.Pin.PULL_UP)
    joystick_switch2 = machine.Pin(pin_joystick_switch2, machine.Pin.IN, machine.Pin.PULL_UP)
    
    # init joystick position
    joystick_x1_position = machine.ADC(machine.Pin(pin_joystick_x1))
    joystick_y1_position = machine.ADC(machine.Pin(pin_joystick_y1))
    joystick_x2_position = machine.ADC(machine.Pin(pin_joystick_x2))
    joystick_y2_position = machine.ADC(machine.Pin(pin_joystick_y2))
    
    # init potentiometer position
    potentiometer1_position = machine.ADC(machine.Pin(pin_potentiometer1))
    potentiometer2_position = machine.ADC(machine.Pin(pin_potentiometer2))
    
    # init adc value
    joystick_x1_position.width(machine.ADC.WIDTH_10BIT)
    joystick_y1_position.width(machine.ADC.WIDTH_10BIT)
    joystick_x2_position.width(machine.ADC.WIDTH_10BIT)
    joystick_y2_position.width(machine.ADC.WIDTH_10BIT)
    potentiometer1_position.width(machine.ADC.WIDTH_10BIT)
    potentiometer2_position.width(machine.ADC.WIDTH_10BIT)
    
    joystick_x1_position.atten(machine.ADC.ATTN_11DB)
    joystick_y1_position.atten(machine.ADC.ATTN_11DB)
    joystick_x2_position.atten(machine.ADC.ATTN_11DB)
    joystick_y2_position.atten(machine.ADC.ATTN_11DB)
    potentiometer1_position.atten(machine.ADC.ATTN_11DB)
    potentiometer2_position.atten(machine.ADC.ATTN_11DB)
    
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
    
