import serial
import time

s = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

while True:
    x = 500
    y = 300
    s.write(x + ":" + y + ";")
    time.sleep(1)

s.close()
