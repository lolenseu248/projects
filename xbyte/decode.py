import serial

port = 'COM3'  # Adjust this to your serial port
baud_rate = 9600

try:
    ser = serial.Serial(port, baud_rate, timeout=1)
    print(f"Listening to {port} at {baud_rate} baud...")

    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            print(f'Received: {data}')

except serial.SerialException as e:
    print(f"Serial port {port} is busy or not available: {e}")

except KeyboardInterrupt:
    print('Keyboard Interrupt detected. Exiting...')

finally:
    if 'ser' in locals():
        ser.close()
        print(f"Serial port {port} closed.")
