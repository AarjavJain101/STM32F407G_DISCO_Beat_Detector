# Python code to read dev/ttyUSB0 at 112500 baudrate stop bits 1 and parity none. 8 bits data And print the data on the console.

import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 112500, timeout=1, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE, bytesize=serial.EIGHTBITS)

while True:
    data = ser.readline()
    print(data.decode('utf-8'), end='')
    time.sleep(0.03)
