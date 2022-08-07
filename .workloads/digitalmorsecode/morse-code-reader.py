# For Raspberry Pi Pico
from machine import Pin, PWM
from utime import sleep

buzzer = PWM(Pin(16)) # GP16 and GND
pitch = 800000

morsecode = input("> ")

number = 0
for i in range(len(morsecode)):
    x = morsecode[number]
    print(x)
    if x == "0":
        buzzer.duty_u16(pitch)
        sleep(0.1)
        buzzer.duty_u16(0)
    elif x == "1":
        buzzer.duty_u16(pitch)
        sleep(0.5)
        buzzer.duty_u16(0)
    sleep(0.02)
    number += 1  
