from machine import I2C, Pin
import time

# Initialize I2C
i2c = I2C(0, scl=Pin(1), sda=Pin(0), freq=100000)

# Sensor Addresses
MPU6050_ADDR = 0x68
MAGNETOMETER_ADDR = 0x0D  # Verify if this is the correct address
BMP280_ADDR = 0x77

# MPU6050 Registers
MPU6050_REG_PWR_MGMT_1 = 0x6B
MPU6050_REG_ACCEL_XOUT_H = 0x3B
MPU6050_REG_GYRO_XOUT_H = 0x43

# BMP280 Registers
BMP280_REG_CONTROL = 0xF4
BMP280_REG_CONFIG = 0xF5
BMP280_REG_PRESS_MSB = 0xF7
BMP280_REG_PRESS_LSB = 0xF8
BMP280_REG_PRESS_XLSB = 0xF9

# Magnetometer Registers (example, update based on actual magnetometer)
MAGNETOMETER_REG_XOUT_H = 0x03
MAGNETOMETER_REG_XOUT_L = 0x04
MAGNETOMETER_REG_YOUT_H = 0x05
MAGNETOMETER_REG_YOUT_L = 0x06
MAGNETOMETER_REG_ZOUT_H = 0x07
MAGNETOMETER_REG_ZOUT_L = 0x08

def read_register(addr, reg, length=1):
    try:
        data = i2c.readfrom_mem(addr, reg, length)
        return data
    except OSError as e:
        print(f"Failed to read from register {hex(reg)} at address {hex(addr)}: {e}")
        return None

def write_register(addr, reg, value):
    try:
        i2c.writeto_mem(addr, reg, bytes([value]))
    except OSError as e:
        print(f"Failed to write to register {hex(reg)} at address {hex(addr)}: {e}")

def init_mpu6050():
    try:
        write_register(MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, 0x00)  # Wake up MPU6050
        print("MPU6050 initialization successful")
    except OSError as e:
        print("Failed to initialize MPU6050:", e)

def init_bmp280():
    try:
        write_register(BMP280_ADDR, BMP280_REG_CONTROL, 0x27)  # Control register: Normal mode, pressure and temperature
        write_register(BMP280_ADDR, BMP280_REG_CONFIG, 0xA0)   # Config register: Standby time 1000ms, filter coefficient 16
        print("BMP280 initialization successful")
    except OSError as e:
        print("Failed to initialize BMP280:", e)

def init_magnetometer():
    try:
        # Add initialization commands for the magnetometer if needed
        print("Magnetometer initialization successful")
    except OSError as e:
        print("Failed to initialize Magnetometer:", e)

def read_mpu6050():
    try:
        accel_data = read_register(MPU6050_ADDR, MPU6050_REG_ACCEL_XOUT_H, 6)
        gyro_data = read_register(MPU6050_ADDR, MPU6050_REG_GYRO_XOUT_H, 6)
        
        if accel_data and gyro_data:
            accel = {
                'x': (accel_data[0] << 8) | accel_data[1],
                'y': (accel_data[2] << 8) | accel_data[3],
                'z': (accel_data[4] << 8) | accel_data[5]
            }
            
            gyro = {
                'x': (gyro_data[0] << 8) | gyro_data[1],
                'y': (gyro_data[2] << 8) | gyro_data[3],
                'z': (gyro_data[4] << 8) | gyro_data[5]
            }
            
            return accel, gyro
        else:
            return None, None
    except Exception as e:
        print("Failed to read MPU6050 data:", e)
        return None, None

def read_bmp280():
    try:
        press_data = read_register(BMP280_ADDR, BMP280_REG_PRESS_MSB, 3)
        
        if press_data:
            press_raw = (press_data[0] << 12) | (press_data[1] << 4) | (press_data[2] >> 4)
            
            # Apply calibration (simplified example)
            pressure = press_raw / 16.0
            
            return pressure
        else:
            return None
    except Exception as e:
        print("Failed to read BMP280 data:", e)
        return None

def read_magnetometer():
    try:
        mag_data = read_register(MAGNETOMETER_ADDR, MAGNETOMETER_REG_XOUT_H, 6)
        
        if mag_data:
            magnetometer = {
                'x': (mag_data[0] << 8) | mag_data[1],
                'y': (mag_data[2] << 8) | mag_data[3],
                'z': (mag_data[4] << 8) | mag_data[5]
            }
            
            return magnetometer
        else:
            return None
    except Exception as e:
        print("Failed to read magnetometer data:", e)
        return None

while True:
    init_mpu6050()
    init_bmp280()
    init_magnetometer()
    
    # Read and print data from MPU6050
    accel, gyro = read_mpu6050()
    if accel and gyro:
        print("Accelerometer: x={}, y={}, z={}".format(accel['x'], accel['y'], accel['z']))
        print("Gyroscope: x={}, y={}, z={}".format(gyro['x'], gyro['y'], gyro['z']))
    else:
        print("Error reading MPU6050 data")
    
    # Read and print pressure from BMP280
    pressure = read_bmp280()
    if pressure is not None:
        print("Pressure:", pressure)
    else:
        print("Error reading BMP280 data")

    # Read and print magnetometer data
    magnetometer = read_magnetometer()
    if magnetometer:
        print("Magnetometer: x={}, y={}, z={}".format(magnetometer['x'], magnetometer['y'], magnetometer['z']))
    else:
        print("Error reading magnetometer data")
    
    # Delay between readings
    time.sleep(.1)

