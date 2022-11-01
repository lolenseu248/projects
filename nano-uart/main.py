from machine import Pin, PWM
from utime import sleep

pwm = PWM(pin(1)
pwm.freq(60)

while True:
	pwm.duty_u16(1000)
	sleep(1)
