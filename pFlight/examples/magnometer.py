from machine import I2C, Pin
import time

# Initialize I2C
i2c = I2C(0, scl=Pin(1), sda=Pin(0), freq=100000)

# Magnetometer Constants (HMC5883L or QMC5883L example)
MAG_ADDR = 0x0D  # Your magnetometer's I2C address
MAG_DATA_REG = 0x03  # Register to start reading data for HMC5883L
MAG_MODE_REG = 0x09  # Mode register for QMC5883L

# Initialize Magnetometer (if necessary)
try:
    # Set the magnetometer to continuous measurement mode
    i2c.writeto_mem(MAG_ADDR, MAG_MODE_REG, b'\x01')
except Exception as e:
    print("Magnetometer initialization failed:", e)

def read_magnetometer():
    # Read Magnetometer data
    try:
        mag_data = i2c.readfrom_mem(MAG_ADDR, MAG_DATA_REG, 6)
    except Exception as e:
        print("Failed to read from magnetometer:", e)
        return 0, 0, 0
    
    if len(mag_data) != 6:
        print("Incorrect data length:", len(mag_data))
        return 0, 0, 0
    
    mag_x = int.from_bytes(mag_data[0:2], 'big')
    mag_y = int.from_bytes(mag_data[4:6], 'big')
    mag_z = int.from_bytes(mag_data[2:4], 'big')
    
    return mag_x, mag_y, mag_z

while True:
    # Read data from magnetometer
    mag = read_magnetometer()
    
    # Print the magnetometer data
    print("Magnetometer:", mag)
    
    # Delay between readings
    time.sleep(1)
