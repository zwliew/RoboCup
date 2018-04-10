from picamera.array import PiRGBArray
from picamera import PiCamera
from threading import Thread
import math
import cv2
import time
import imutils
import serial

ARDUINO_CONNECTED = False

class PiVideoStream:
    # Orange ball HSV values
    LOWER = (60, 50, 150)
    UPPER = (110, 110, 210)

    SENSOR_MODE = 4
    RESOLUTION = (800, 608)

    def __init__(self):
        # initialize the camera and stream
        self.camera = PiCamera(sensor_mode=PiVideoStream.SENSOR_MODE)
        self.camera.resolution = PiVideoStream.RESOLUTION
        self.rawCapture = PiRGBArray(self.camera)
        self.stream = self.camera.capture_continuous(self.rawCapture,
        format="bgr", use_video_port=True)

        # initialize the frame and the variable used to indicate
        # if the thread should be stopped
        self.frame = None
        self.stopped = False

    def start(self):
        # start the thread to read frames from the video stream
        Thread(target=self.update, args=()).start()
        return self

    def update(self):
        # keep looping infinitely until the thread is stopped
        for f in self.stream:
            # grab the frame from the stream and clear the stream in
            # preparation for the next frame
            self.frame = frame.array

            self.rawCapture.truncate(0)

            # if the thread indicator variable is set, stop the thread
            # and resource camera resources
            if self.stopped:
                self.stream.close()
                self.rawCapture.close()
                self.camera.close()
                return

    def read(self):
        # return the frame most recently read
        return self.frame

    def stop(self):
        # indicate that the thread should be stopped
        self.stopped = True

if ARDUINO_CONNECTED:
    ser = serial.Serial(
        port='/dev/serial0',
        baudrate = 230400,
    )

stream = PiVideoStream().start()
time.sleep(0.3)

while True:
    frame = stream.read()
    mask = cv2.inRange(frame, redLower, redUpper)
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)[-2]
    if len(cnts) > 0:
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        x1 = x - 400
        y1 = y - 304
        if x1 == 0:
            x1 = 0.01
        polarDist = math.sqrt(abs(x1) + abs(y1))
        polarAngle = math.atan(y1 / x1)
        polarAngle = polarAngle * 180 / math.pi
        if x1 < 0:
            polarAngle = -(90 + polarAngle)
        else:
            polarAngle = 90 - polarAngle
        polarAngle = str(polarAngle)
        polarAngle = polarAngle + (119 - len(polarAngle)) * "a" + "b"
        if ARDUINO_CONNECTED:
            ser.write(bytes(polarAngle, 'utf-8'))
        else:
            print(polarAngle)
    else:
        if ARDUINO_CONNECTED:
            ser.write(bytes((119 * "r" + "b"), 'utf-8'))

stream.stop()
