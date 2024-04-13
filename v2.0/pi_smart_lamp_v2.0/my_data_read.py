import cv2
import mediapipe as mp
import time

import numpy as np


label = ["right", "down", "no", "phone"]

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
def main():
    cap = cv2.VideoCapture(0)
    #设置 640 x 480 分辨率
    cap.set(3, width)#3对应宽
    cap.set(4, height)#4对应高
    white_color = (0, 0, 0)  # 白色的 RGB 值

    canvas = np.ones((height, width, 3), dtype=np.uint8)
    canvas[:] = white_color  # 将画布填充为白色

    ret, img = cap.read()
    if ret is False:
        print("摄像头打开失败")
    count = 0#计数
    while ret is True:
        ret, img = cap.read()

        #图像处理
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        img = cv2.flip(img, 1)  # 水平翻转
        results = holistic.process(img)
        if results.pose_landmarks:
            # print("检测成功")
            canvas[:] = white_color  # 重新填充画布为白色
            mpDraw.draw_landmarks(canvas, results.pose_landmarks, mpHolistic.POSE_CONNECTIONS)
            #保存canvas
            # cv2.imwrite(f"data/{count}.jpg", canvas)  # Change "pose_canvas.jpg" to your desired file name/path
            count += 1
            if count == 1500:
                break
            # time.sleep(0.1)
            print(count)
            #img：要画的图像
            #results.pose_landmarks：要画的关键点
            #mpHolistic.POSE_CONNECTIONS：画关键点的连接线
        #显示图像
        img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
        cv2.imshow("img", canvas)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
