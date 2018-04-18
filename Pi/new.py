#!/usr/bin/python

from picamera.array import PiRGBArray
from picamera import PiCamera
from threading import Thread
import cv2
import math
import imutils
import serial
import numpy

# Serial
BAUD_RATE = 250000

# Pi Camera
SENSOR_MODE = 4
RESOLUTION = (800, 608)
HALF_RES = (400, 304)
SHUTTER_SPEED = 26700

# Ball
BALL_LOWER = (0, 110, 110)
BALL_UPPER = (20, 255, 255)

# Coordinates
X_OFFSET = 25
Y_OFFSET = 40

# Mathematical constants
HALF_PI = math.pi / 2
DOUBLE_PI = math.pi * 2

# Debug
ARDUINO_CONNECTED = True

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

def main():
    vs = PiVideoStream().start()

    while vs.read() is None:
        pass

    if ARDUINO_CONNECTED:
        s = serial.Serial("/dev/serial0", BAUD_RATE)

    while True:
        bgr = vs.read()
        hsv = cv2.cvtColor(bgr, cv2.COLOR_BGR2HSV)

        # Ball
        mask = cv2.inRange(hsv, BALL_LOWER, BALL_UPPER)
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
        if len(cnts) > 0:
            cnt = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(cnt)

            x = int(x - HALF_RES[0] + X_OFFSET)
            y = int(HALF_RES[1] - y + Y_OFFSET)
            if not ARDUINO_CONNECTED:
                print(x, y, int(radius))

            if x == 0 and y == 0:
                angle = 0
                distance = 0
            else:
                angle = math.atan2(y, x)
                distance = int(math.hypot(x, y) / RESOLUTION[1] * 12.7)
                if angle < 0:
                    angle = -angle + HALF_PI
                elif angle < HALF_PI:
                    angle = HALF_PI - angle
                else:
                    angle = DOUBLE_PI - (angle - HALF_PI)
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

main()
