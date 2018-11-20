from collections import deque
from picamera.array import PiRGBArray
from picamera import PiCamera
import argparse
import imutils
import cv2
import numpy

ap = argparse.ArgumentParser()
ap.add_argument('-v', '--video', help='path to the (optional) video file')
ap.add_argument('-b', '--buffer', type=int, default=64, help='max buffer size')
args = vars(ap.parse_args())

orange_lower = (0, 104, 106)
orange_upper = (35, 255, 255)

orangeLowL = 140
orangeHighL = 255
orangeLowA = 145
orangeHighA = 190
orangeLowB = 130
orangeHighB = 255
ORANGE_LOWER =  numpy.array([orangeLowL, orangeLowA, orangeLowB])
ORANGE_UPPER =  numpy.array([orangeHighL, orangeHighA, orangeHighB])

LOWER = (0, 100, 102)
UPPER = (16, 255, 255)

pts = deque(maxlen=args['buffer'])

if not args.get('video', False):
    camera = PiCamera(sensor_mode=4)
    camera.shutter_speed = 26700
    camera.resolution = (800, 608)
    rawCapture = PiRGBArray(camera, size=camera.resolution)
else:
    camera = cv2.VideoCapture(args['video'])

for frame in camera.capture_continuous(rawCapture, format='bgr', use_video_port=True):
    image = frame.array

    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    mask = cv2.inRange(hsv, LOWER, UPPER)

    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)[-2]
    center = None

    if len(cnts) > 0:
        cnt = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(cnt)
        x = x - 400
        y = y - 304
        print(x, y, radius)
        M = cv2.moments(cnt)
        center = (int(M['m10'] / M['m00']), int(M['m01'] / M['m00']))

        if radius > 0:
            cv2.circle(image, (int(x), int(y)), int(radius), (0, 255, 255), 2)
            #cv2.circle(image, center, 5, (0, 0, 255), -1)

    pts.appendleft(center)

    for i in range(1, len(pts)):
        if pts[i - 1] is None or pts[i] is None:
            continue
        
        thickness = int(numpy.sqrt(args['buffer'] / float(i + 1)) * 2.5)
        #cv2.line(image, pts[i - 1], pts[i], (0, 0, 255), thickness)

    cv2.imshow('Image', image)

    rawCapture.truncate(0)

    key = cv2.waitKey(1) & 0xFF

    if key == ord('q'):
        break

camera.close()
cv2.destroyAllWindows()
