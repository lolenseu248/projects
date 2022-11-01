from machine import Pin, ADC, PWM
from utime import sleep

adc = ADC(Pin(0))
pwm = PWM(Pin(1))
pwm.freq(60)

while True:
	pwm.duty_u16(1000)
	adc.read()
	sleep(1)
