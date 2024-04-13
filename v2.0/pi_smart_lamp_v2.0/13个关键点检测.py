import cv2
import mediapipe as mp
import time

label = ["right", "down", "no"]
class UpperBodyDetector():
    def __init__(self, mode=False, modelComplexity=1,detectionCon=0.5, trackCon=0.5):
        self.mode = mode
        self.detectionCon = detectionCon
        self.trackCon = trackCon
        self.modelComplexity = modelComplexity

        self.mpHolistic = mp.solutions.holistic
        self.holistic = self.mpHolistic.Holistic(self.mode,
                                                 model_complexity=self.modelComplexity,
                                                 smooth_segmentation=True,
                                                 refine_face_landmarks=True,
                                                 min_detection_confidence=self.detectionCon,
                                                 min_tracking_confidence=self.trackCon)
        self.mpDraw = mp.solutions.drawing_utils
    def findUpperBody(self, img, draw=True):
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.holistic.process(imgRGB)
        if self.results.pose_landmarks:
            pose_local = []
            for i in range(13):
                # 获取关键点的位置信息
                x = min(int(self.results.pose_landmarks.landmark[i].x * img.shape[1]), img.shape[1] - 1)
                y = min(int(self.results.pose_landmarks.landmark[i].y * img.shape[0]), img.shape[0] - 1)
                pose_local.append([x, y])
            print("可以这样检测到关键点的位置信息")
            if draw:
                self.mpDraw.draw_landmarks(img, self.results.pose_landmarks, self.mpHolistic.POSE_CONNECTIONS)

        return img


def main():
    pTime = 0
    cTime = 0
    cap = cv2.VideoCapture(1)
    detector = UpperBodyDetector()

    while True:
        success, img = cap.read()
        img = detector.findUpperBody(img)
        cTime = time.time()
        fps = 1 / (cTime - pTime)
        pTime = cTime
        cv2.putText(img, f"FPS: {int(fps)}", (10, 70), cv2.FONT_HERSHEY_PLAIN, 3, (255, 0, 0), 3)

        cv2.imshow("Image", img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
