#测试摄像头是否正常
import cv2
import numpy as np
import time

cap = cv2.VideoCapture(0)
# cap.set(3, 640)
# cap.set(4, 480)

while True:
    ret, frame = cap.read()
    if ret == True:
        cv2.imshow('frame', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        print('error')
        break