import time

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
import pyttsx3  # 导入库
from multiprocessing import Process,Queue
# import fcntl

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


# fourcc = cv2.VideoWriter_fourcc(*'XVID')  # 定义视频编解码器
# out = cv2.VideoWriter('output_video.mp4', fourcc, 20.0, (640, 480))  # 输出视频文件名、编解码器、帧率、大小

say_eng = pyttsx3.init() #初始化一个实例
say = True
#信号
signal_no = "无人在周围，"
signal_right = "坐姿正确，请保持"
signal_down = "坐姿错误，请端正坐姿"
signal_nocare = "请集中注意力，认真学习"
# ids_show = 'right'
def say():
    # global say
    # global ids_show
    # ids_show = q.get()

    # with open('ids_show.txt', 'r') as file:
    #     content_read = file.read()  # 从文件中读取内容到变量中
    # ids_show = str(content_read)
    while True:
        with open('ids_show.txt', 'r') as file:
            content_read = file.read()  # 从文件中读取内容到变量中
        ids_show = str(content_read)
        if say:
            if ids_show == 'right':
                say_eng.say(signal_right)  # say 用于传递要说的文本的方法
                say_eng.runAndWait()  # 运行并处理语音命令
                print('right')
                # time.sleep(3)
            elif ids_show == 'down':
                say_eng.say(signal_down)
                say_eng.runAndWait()  # 运行并处理语音命令
                print('down')
                # time.sleep(3)
            elif ids_show == 'no':
                say_eng.say(signal_no)
                say_eng.runAndWait()  # 运行并处理语音命令pyttsx3
                print('no')
                # time.sleep(3)
            elif ids_show == "lack of concentration":       #no_care
                say_eng.say(signal_nocare)
                say_eng.runAndWait()  # 运行并处理语音命令
                print(ids_show)
                # time.sleep(3)


def main():
    # global ids_show
    # ids_show_over = "right"

    # 读取视频
    cap = cv2.VideoCapture(0)
    # 设置 640 x 480 分辨率
    cap.set(3, 640)  # 3对应宽
    cap.set(4, 640)  # 4对应高

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
            mpDraw.draw_landmarks(img, results.pose_landmarks, mpHolistic.POSE_CONNECTIONS)
            canvas,ids_show,values,text ,FPS= process_frame(canvas)
            # q.put(ids_show)
            with open('ids_show.txt', 'w') as file:
                file.write(ids_show)

            str_values = '{:>.3f}'.format(values)
            img = cv2.putText(img, 'FPS  ' + str(int(FPS)), (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 4,
                              cv2.LINE_AA)
            img = cv2.putText(img, "status:" + ids_show, (20, 100), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 2,
                              cv2.LINE_AA)
            img = cv2.putText(img, "status:" + str_values, (20, 150), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255), 2,
                              cv2.LINE_AA)
            img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
            cv2.imshow('my_window', img)
            # out.write(img)
            if cv2.waitKey(1) in [ord('q'), 27]:  # 按键盘上的q或esc退出（在英文输入法下）
                break

    # 释放资源
    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    # q = Queue()

    process_list = []
    p = Process(target=main, args=())  # 注意args里面要把q对象传给我们要执行的方法，这样子进程才能和主进程用Queue来通信
    p.start()
    process_list.append(p)

    p = Process(target=say, args=())  # 注意args里面要把q对象传给我们要执行的方法，这样子进程才能和主进程用Queue来通信
    p.start()
    process_list.append(p)
    for i in process_list:
        p.join()
