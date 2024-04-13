import cv2
import mediapipe as mp

def main():
    # 初始化 MediaPipe Pose 模型
    mp_pose = mp.solutions.pose     # 加载模型
    mpDraw = mp.solutions.drawing_utils     # 加载绘图函数

    pose = mp_pose.Pose(static_image_mode=False,  # 静态图模式，False代表置信度高时继续跟踪，True代表实时跟踪检测新的结果
                       smooth_landmarks=True,  # 平滑，一般为True
                       min_detection_confidence=0.8,  # 检测置信度
                       min_tracking_confidence=0.9,     # 跟踪置信度
                        )
    # 检测置信度大于0.5代表检测到了，若此时跟踪置信度大于0.5就继续跟踪，小于就沿用上一次，避免一次又一次重复使用模型

    # 使用摄像头捕获视频
    cap = cv2.VideoCapture(1)
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
        results = pose.process(frame)
        # 获取检测到的关键点
        if results.pose_landmarks:
            mpDraw.draw_landmarks(frame, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)
            for landmark in results.pose_landmarks.landmark:
                # 获取关键点的位置信息
                landmark_x = int(landmark.x * frame.shape[1])
                landmark_y = int(landmark.y * frame.shape[0])

        # 显示结果
        cv2.imshow('Pose Detection', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
