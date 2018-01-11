from picamera.array import PiRGBArray
from picamera import PiCamera
from threading import Thread
import cv2
import time

# Orange ball HSV values
LOWER = (0, 104, 106)
UPPER = (35, 255, 255)

class PiVideoStream:
    def __init__(self, sensor_mode=0, resolution=(320, 240), framerate=32):
        # initialize the camera and stream
        self.camera = PiCamera()
        self.camera.sensor_mode = sensor_mode
        self.camera.resolution = resolution
        self.camera.framerate = framerate
        self.rawCapture = PiRGBArray(self.camera, size=resolution)
        self.stream = self.camera.capture_continuous(self.rawCapture,
        format="bgr", use_video_port=True)

        # initialize the frame and the variable used to indicate
        # if the thread should be stopped
        self.frame = None
        self.stopped = False

    def start(self):
        # start the thread to read frames from the video stream
        t = Thread(target=self.update, args=())
        t.daemon = True
        t.start()
        return self

    def update(self):
        # keep looping infinitely until the thread is stopped
        for f in self.stream:
            # grab the frame from the stream and clear the stream in
            # preparation for the next frame
            frame = f.array

            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            frame = cv2.inRange(frame, LOWER, UPPER)
            frame = cv2.morphologyEx(frame, cv2.MORPH_OPEN, None)
            frame = cv2.morphologyEx(frame, cv2.MORPH_CLOSE, None)

            self.frame = frame

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

SENSOR_MODE = 4
RESOLUTION = (1200, 1200)
FRAMERATE = 40

stream = PiVideoStream(SENSOR_MODE, RESOLUTION, FRAMERATE).start()
time.sleep(2)

while True:
    frame = stream.read()

    cnts = cv2.findContours(frame.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)[-2]
    if len(cnts) > 0:
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)

stream.stop()
