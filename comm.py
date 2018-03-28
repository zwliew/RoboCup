import serial
import time

s = serial.Serial(
    port='/dev/serial0',
    baudrate=115200,
)

while True:
    x = 500
    y = 300
    s.write(bytes(x + ":" + y + ";"))
    time.sleep(1)

s.close()
