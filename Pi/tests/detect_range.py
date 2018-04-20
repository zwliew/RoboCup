#!/usr/bin/env python
# -*- coding: utf-8 -*-

# USAGE: You need to specify a filter and "only one" image source
#
# (python) range_detector --filter RGB --image /path/to/image.png
# or
# (python) range_detector --filter HSV --webcam

from picamera.array import PiRGBArray
from picamera import PiCamera
import cv2
import argparse
import time
from operator import xor
import math
import numpy as np

# Coordinates
X_OFFSET = 50
Y_OFFSET = 45

orangeLowL = 140
orangeLowA = 161
orangeLowB = 128
orangeHighL = 211
orangeHighA = 178
orangeHighB = 167

orangeLower =  np.array([orangeLowL, orangeLowA, orangeLowB])
orangeUpper =  np.array([orangeHighL, orangeHighA, orangeHighB])

def callback(value):
    pass


def setup_trackbars(range_filter):
    cv2.namedWindow("Trackbars", 0)

    for i in ["MIN", "MAX"]:
        v = 0 if i == "MIN" else 255

        for j in range_filter:
            cv2.createTrackbar("%s_%s" % (j, i), "Trackbars", v, 255, callback)


def get_arguments():
    ap = argparse.ArgumentParser()
    ap.add_argument('-f', '--filter', required=True,
                    help='Range filter. RGB or HSV')
    ap.add_argument('-i', '--image', required=False,
                    help='Path to the image')
    ap.add_argument('-w', '--webcam', required=False,
                    help='Use webcam', action='store_true')
    ap.add_argument('-p', '--preview', required=False,
                    help='Show a preview of the image after applying the mask',
                    action='store_true')
    args = vars(ap.parse_args())

    if not xor(bool(args['image']), bool(args['webcam'])):
        ap.error("Please specify only one image source")

    if not args['filter'].upper() in ['RGB', 'HSV']:
        ap.error("Please speciy a correct filter.")

    return args


def get_trackbar_values(range_filter):
    values = []

    for i in ["MIN", "MAX"]:
        for j in range_filter:
            v = cv2.getTrackbarPos("%s_%s" % (j, i), "Trackbars")
            values.append(v)

    return values


def main():
    args = get_arguments()

    range_filter = args['filter'].upper()

    if args['image']:
        image = cv2.imread(args['image'])

        if range_filter == 'RGB':
            frame_to_thresh = image.copy()
        else:
            frame_to_thresh = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    else:
        camera = PiCamera(sensor_mode=4)
        camera.resolution = (800, 608)
        camera.shutter_speed = 22000
        #camera.contrast = 50
        #camera.exposure_compensation = 6
        camera.hflip = True
        rawCapture = PiRGBArray(camera, size=camera.resolution)

        time.sleep(0.1)

    setup_trackbars(range_filter)

    for frame in camera.capture_continuous(rawCapture, format='bgr', use_video_port=True):
        image = frame.array

        if range_filter == 'RGB':
            frame_to_thresh = image.copy()
        else:
            frame_to_thresh = cv2.cvtColor(image, cv2.COLOR_BGR2Lab)

        v1_min, v2_min, v3_min, v1_max, v2_max, v3_max = get_trackbar_values(range_filter)

        thresh = cv2.inRange(frame_to_thresh, (v1_min, v2_min, v3_min), (v1_max, v2_max, v3_max))
        cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
        if len(cnts) > 0:
            cnt = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(cnt)
            cv2.circle(image, (int(x), int(y)), 10, (0, 255, 255), 2)

            x = int(x - 800 / 2 + X_OFFSET)
            y = int(608 / 2 - y + Y_OFFSET)
            print(int(x), int(y), int(radius))

        if x == 0 and y == 0:
            angle = 0
            distance = 0
        else:
            angle = math.atan2(y, x)
            distance = int(math.hypot(x, y))
            if angle < 0:
                angle = -angle + math.pi / 2
            elif angle < math.pi / 2:
                angle = math.pi / 2 - angle
            else:
                angle = math.pi * 2 - (angle - math.pi / 2)
            angle = int(math.degrees(angle))

        print(angle, distance)

        #thresh = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, None)
        #thresh = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, None)

        if args['preview']:
            preview = cv2.bitwise_and(image, image, mask=thresh)
            cv2.imshow("Preview", preview)
        else:
            cv2.imshow("Original", image)
            cv2.imshow("Thresh", thresh)

        rawCapture.truncate(0)

        if cv2.waitKey(1) & 0xFF is ord('q'):
            break


if __name__ == '__main__':
    main()
