import os
import time
import numpy as np
import pandas as pd
import cv2 # opencv-python
from PIL import Image, ImageFont, ImageDraw
import mediapipe as mp
from tqdm import tqdm # 进度条
import matplotlib.pyplot as plt
import torch
import torch.nn.functional as F
from torchvision import models
from torchvision import transforms
import serial
import struct
import serial_my

#串口初始化
# ser = serial_my.init_serial(baudrate=115200,port='/dev/ttyS0')

# 有 GPU 就用 GPU，没有就用 CPU
device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')
print('device:', device)
# 导入中文字体，指定字号
font = ImageFont.truetype('SimHei.ttf', 32)
idx_to_labels = np.load('idx_to_labels.npy', allow_pickle=True).item()
print(idx_to_labels)
model = torch.load('checkpoints/mymodel/zzsb.pth', map_location=torch.device('cuda'))
model = model.eval().to(device)


# 测试集图像预处理-RCTN：缩放裁剪、转 Tensor、归一化
test_transform = transforms.Compose([transforms.Resize(256),
                                     transforms.CenterCrop(224),
                                     transforms.ToTensor(),
                                     transforms.Normalize(
                                         mean=[0.485, 0.456, 0.406],
                                         std=[0.229, 0.224, 0.225])
                                    ])

mode = False
modelComplexity = 1
detectionCon = 0.5
trackCon = 0.5
mpHolistic = mp.solutions.holistic
draw = True
holistic = mpHolistic.Holistic(mode,
            model_complexity=modelComplexity,
            smooth_segmentation=True,
            refine_face_landmarks=True,
            min_detection_confidence=detectionCon,
            min_tracking_confidence=trackCon)
mpDraw = mp.solutions.drawing_utils
width, height = 640, 480

# 处理帧函数
def process_frame(img):
    start_time = time.time()
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img_pil = Image.fromarray(img_rgb)# array 转 PIL
    input_img = test_transform(img_pil).unsqueeze(0).to(device)
    pred_logits = model(input_img)  # pred_logits:是一个tensor
    pred_softmax = F.softmax(pred_logits, dim=1)

    top_n = torch.topk(pred_softmax, 1)  # 仅获取最高置信度的一个结果
    values = top_n.values.cpu().detach().numpy().squeeze()
    indices = top_n.indices.cpu().detach().numpy().squeeze()
    draw = ImageDraw.Draw(img_pil)
    pred_class = idx_to_labels[int(indices)]  # 将 NumPy 数组转换为标量值作为字典的键
    text = '{:<8} {:>.3f}'.format(pred_class, values)
    # 文字坐标，中文字符串，字体，rgba颜色
    draw.text((50, 100), text, font=font, fill=(255, 0, 0, 1))
    img = np.array(img_pil)  # PIL 转 array
    img = np.array(img_pil)
    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)

    end_time = time.time()
    FPS = 1 / (end_time - start_time)
    img = cv2.putText(img, 'FPS: ' + str(int(FPS)), (50, 80),
                      cv2.FONT_HERSHEY_SIMPLEX,
                      1,#字体大小
                      (255, 0, 255),
                      4,
                      cv2.LINE_AA)#org:左下角坐标

    return img, pred_class, values



ids_show = "000"
def sending_esp32():  #作用是根据预测结果发送数据到esp32
    global ids_show
    while True:
        if ids_show == 'right':
            serial_my.sending_data(cx=1)
            print('right')
        elif ids_show == 'down':
            serial_my.sending_data(cx=2)
            print('down')
        elif ids_show == 'no':
            serial_my.sending_data(cx=3)
            print('no')
        else:
            serial_my.sending_data(cx=0)
            print('phone')
        time.sleep(2)


def main():
    global ids_show
    # 读取视频
    cap = cv2.VideoCapture(0)
    # 设置 640 x 480 分辨率
    cap.set(3, 640)  # 3对应宽
    cap.set(4, 480)  # 4对应高

    white_color = (0, 0, 0)  # 白色的 RGB 值
    canvas = np.ones((height, width, 3), dtype=np.uint8)
    canvas[:] = white_color  # 将画布填充为白色
    ret, img = cap.read()
    if ret is False:
        print("摄像头打开失败")
    # 读取视频流
    while ret is True:
        ret, img = cap.read()
        # 图像处理
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img = cv2.flip(img, 1)  # 水平翻转
        results = holistic.process(img)
        # if results.pose_landmarks:
        if True:
            canvas[:] = white_color  # 重新填充画布为白色
            mpDraw.draw_landmarks(canvas, results.pose_landmarks, mpHolistic.POSE_CONNECTIONS)
            canvas,ids_show,values = process_frame(canvas)
            if ids_show == 'right':
                print('1')
            cv2.imshow('my_window', canvas)
            if cv2.waitKey(1) in [ord('q'), 27]:  # 按键盘上的q或esc退出（在英文输入法下）
                break

    # 释放资源
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()







