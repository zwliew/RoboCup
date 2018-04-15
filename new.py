from picamera.array import PiRGBArray
from picamera import PiCamera
from threading import Thread
import cv2
import math
import imutils
import serial
import numpy

SENSOR_MODE = 4
RESOLUTION = (800, 608)
SHUTTER_SPEED = 26700

class PiVideoStream:
    def __init__(self):
        self.camera = PiCamera(sensor_mode=SENSOR_MODE)
        self.camera.resolution = RESOLUTION
        self.camera.shutter_speed = SHUTTER_SPEED
        self.camera.hflip = True
        self.rawCapture = PiRGBArray(self.camera)
        self.stream = self.camera.capture_continuous(self.rawCapture,
            format="bgr", use_video_port=True)

        self.frame = None
        self.stopped = False

    def start(self):
        Thread(target=self.update, args=()).start()
        return self

    def read(self):
        return self.frame
    
    def stop(self):
        self.stopped = True
    
    def update(self):
        for f in self.stream:
            self.frame = f.array
            self.rawCapture.truncate(0)

            if self.stopped:
                self.stream.close()
                self.rawCapture.close()
                self.camera.close()
                return

vs = PiVideoStream().start()

while vs.read() is None:
    pass

LOWER = (0, 110, 110)
UPPER = (20, 255, 255)

ARDUINO_CONNECTED = True

if ARDUINO_CONNECTED:
    s = serial.Serial("/dev/serial0", 112500)

while True:
    bgr = vs.read()
    hsv = cv2.cvtColor(bgr, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, LOWER, UPPER)
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    if len(cnts) > 0:
        cnt = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(cnt)

        x = int(x - RESOLUTION[0] / 2 + 25)
        y = int(RESOLUTION[1] / 2 - y + 25)
        if not ARDUINO_CONNECTED:
            print(x, y, int(radius))

        if x == 0 and y == 0:
            angle = 0
            distance = 0
        else:
            angle = math.atan2(y, x)
            distance = int(math.hypot(x, y) / RESOLUTION[1] * 12.7)
            if angle < 0:
                angle = -angle + math.pi / 2
            elif angle < math.pi / 2:
                angle = math.pi / 2 - angle
            else:
                angle = math.pi * 2 - (angle - math.pi / 2)
            angle = int(math.degrees(angle))

        to_write = str(angle) + ',' + str(distance) + ';'
        if ARDUINO_CONNECTED:
            s.write(bytes(to_write, 'utf-8'))
        else:
            print(to_write)
        
    if not ARDUINO_CONNECTED:
        print()

vs.stop()
cv2.destroyAllWindows()
