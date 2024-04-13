from threading import Thread
import  mediapipe as mp
import numpy as np
from flask import Flask, Response
import cv2
import time
from threading import Thread

import cv2
import onnxruntime
import torch
import  mediapipe as mp
from torchvision import transforms
import torch.nn.functional as F
import pandas as pd
import numpy as np
from PIL import Image, ImageFont, ImageDraw
import matplotlib.pyplot as plt

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
ort_session = onnxruntime.InferenceSession("zzsb.onnx")


# 测试集图像预处理-RCTN：缩放裁剪、转 Tensor、归一化
test_transform = transforms.Compose([transforms.Resize(256),         #意思是将图像的短边缩放到256像素
                                     transforms.CenterCrop(256),     #意思是从中心位置裁剪
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

# 处理帧函数
def process_frame(img):
    '''
    输入摄像头拍摄画面bgr-array，输出图像分类预测结果bgr-array
    '''

    # 记录该帧开始处理的时间
    start_time = time.time()

    ## 画面转成 RGB 的 Pillow 格式
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # BGR转RGB
    img_pil = Image.fromarray(img_rgb)  # array 转 PIL

    ## 预处理
    input_img = test_transform(img_pil)  # 预处理
    input_tensor = input_img.unsqueeze(0).numpy()

    ## onnx runtime 预测
    ort_inputs = {'input': input_tensor}  # onnx runtime 输入
    pred_logits = ort_session.run(['output'], ort_inputs)[0]  # onnx runtime 输出
    pred_logits = torch.tensor(pred_logits)     # numpy 转 tensor
    pred_softmax = F.softmax(pred_logits, dim=1)  # 对 logit 分数做 softmax 运算

    top_n = torch.topk(pred_softmax, 1)  # 仅获取最高置信度的一个结果
    values = top_n.values.cpu().detach().numpy().squeeze()
    indices = top_n.indices.cpu().detach().numpy().squeeze()
    # draw = ImageDraw.Draw(img_pil)
    pred_class = idx_to_labels[int(indices)]  # 将 NumPy 数组转换为标量值作为字典的键
    text = '{:<8} {:>.3f}'.format(pred_class, values)
    # 文字坐标，中文字符串，字体，rgba颜色
    # draw.text((50, 100), text, font=font, fill=(255, 0, 0, 1))
    img = np.array(img_pil)  # PIL 转 array
    img = np.array(img_pil)
    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)

    # 记录该帧处理完毕的时间
    end_time = time.time()
    # 计算每秒处理图像帧数FPS
    FPS = 1 / (end_time - start_time)
    # 图片，添加的文字，左上角坐标，字体，字体大小，颜色，线宽，线型
    # img = cv2.putText(img, 'FPS  ' + str(int(FPS)), (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 4,
    #                   cv2.LINE_AA)
    return img, pred_class, values, text, FPS


ids_show = 'right'
values = 0
text = 'right'
FPS = 0

def predict(cap):
    global ids_show,values,text ,FPS
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
        if True:
            canvas[:] = white_color  # 重新填充画布为白色
            mpDraw.draw_landmarks(canvas, results.pose_landmarks, mpHolistic.POSE_CONNECTIONS)
            canvas,ids_show,values,text ,FPS= process_frame(canvas)
            if cv2.waitKey(1) in [ord('q'), 27]:  # 按键盘上的q或esc退出（在英文输入法下）
                break
    # 释放资源
    cap.release()
    cv2.destroyAllWindows()





def main():
    global ids_show,values,text ,FPS
    # 读取视频
    cap = cv2.VideoCapture(0)
    # 设置 640 x 480 分辨率
    cap.set(3, 640)  # 3对应宽
    cap.set(4, 480)  # 4对应高
    m_thread = Thread(target = predict, args=([cap]), daemon=True)
    m_thread.start()

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
        # img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img = cv2.flip(img, 1)  # 水平翻转
        results = holistic.process(img)

        if True:
            mpDraw.draw_landmarks(img, results.pose_landmarks, mpHolistic.POSE_CONNECTIONS)
            img = cv2.putText(img, 'FPS  ' + str(int(FPS)), (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 4,
                              cv2.LINE_AA)
            img = cv2.putText(img, "status:" + text, (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 2,
                              cv2.LINE_AA)


            cv2.imshow('my_window', img)
            if cv2.waitKey(1) in [ord('q'), 27]:  # 按键盘上的q或esc退出（在英文输入法下）
                break
    # 释放资源
    cap.release()
    cv2.destroyAllWindows()



app = Flask(__name__)

def generate_frames():
    global ids_show,values,text ,FPS
    # 读取视频
    cap = cv2.VideoCapture(0)
    # 设置 640 x 480 分辨率
    cap.set(3, 640)  # 3对应宽
    cap.set(4, 480)  # 4对应高
    m_thread = Thread(target = predict, args=([cap]), daemon=True)
    m_thread.start()

    white_color = (0, 0, 0)  # 白色的 RGB 值
    canvas = np.ones((height, width, 3), dtype=np.uint8)
    canvas[:] = white_color  # 将画布填充为白色

    ret, img = cap.read()
    if ret is False:
        print("摄像头打开失败")
    # 读取视频流

    while True:
        ret, img = cap.read()

        # 图像处理
        # img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img = cv2.flip(img, 1)  # 水平翻转
        results = holistic.process(img)

        if True:
            mpDraw.draw_landmarks(img, results.pose_landmarks, mpHolistic.POSE_CONNECTIONS)
            img = cv2.putText(img, 'FPS  ' + str(int(FPS)), (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 4,
                              cv2.LINE_AA)
            img = cv2.putText(img, "status:" + text, (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 2,
                              cv2.LINE_AA)
        # ret, frame = cap.read()
        ret, buffer = cv2.imencode('.jpg', img)  # 将帧数据编码为JPEG格式
        img = buffer.tobytes()        # 转换为二进制格式
        if cv2.waitKey(1) in [ord('q'), 27]:
            break
        yield (b'--frame\r\n'       # 分隔符
                   b'Content-Type: image/jpeg\r\n\r\n' + img + b'\r\n')       # 数据


@app.route('/video')
def video_feed():
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')
        #传入参数：生成器函数，mimetype：数据类型


if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True, threaded=True)
