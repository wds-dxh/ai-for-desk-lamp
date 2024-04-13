import cv2
import numpy as np
import time
from threading import Thread
import random
import serial


import serial_my

#串口初始化
ser = serial_my.init_serial(baudrate=9600,port='/dev/ttyS0')

#测试串口
while True:
    serial_my.sending_data(cx=1)
    time.sleep(1)
    print('send_data')

ser.close()
