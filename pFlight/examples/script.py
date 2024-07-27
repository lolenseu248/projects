from imu import MPU6050
from machine import Pin, I2C

import utime

i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)
imu = MPU6050(i2c)

xinput_num = 1000
xnum = 1500

throttle = 1000 
yaw = 1500 #if xnum < yaw: r_left - if xnum > yaw: r_right
pitch = 1500 #if xnum < pitch: backwards - if xnum > pitch: forwards
roll = 1500 #if xnum < yaw: left - if xnum > yaw: right

deff esc_1(motor1):
    motor1
    
deff esc_2(motor2):
    motor2

deff esc_3(motor3):
    motor3

deff esc_4(motor4):
    motor4

while True:
	if throttle < xinput_num:
		xinput_num = xinput_num - 1000
		total_throttle = throttle + xinput_ num
	else:
		throttle = 1000
		
		
    esc_1()
	esc_2()
	esc_3()
	esc_4()

while True:
    print(imu.accel.xyz,imu.gyro.xyz,imu.temperature,end='\r')
    ax=round(imu.accel.x,2)
    ay=round(imu.accel.y,2)
    az=round(imu.accel.z,2)
    gx=round(imu.gyro.x)
    gy=round(imu.gyro.y)
    gz=round(imu.gyro.z)
    tem=round(imu.temperature,2)
    utime.sleep(0.2)


