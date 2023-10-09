/**
 * @file 主程序
 * @author wds
 * @date 2023/10/9
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
 */

#include <Arduino.h>
#include <lamp.h>                   // 包含 lamp 库的头文件
#include <Voice_prompt.h>           // 包含 Voice_prompt 库的头文件
#include <Human_detection.h>        // 包含 Human_detection 库的头文件
#include <Speech_recognition.h>     // 包含 Speech_recognition 库的头文件

RGBColor color = {255,255,255};//定义一个颜色结构体
Lamp lamp(255,144,2,color);//定义一个lamp对象
Voice_prompt voice_prompt(5,18,19,23);//定义一个voice_prompt对象
Human_detection human_detection(Serial2,256000,16,17);//定义一个human_detection对象
Speech_recognition speech_recognition(true,115200,"xiao ya",
"hong deng","lv deng","lan deng","bai deng","hui deng","guan deng");//定义一个speech_recognition对象

void setup() {


}

void loop() {
    FrameData frameData;//定义一个帧内数据结构体
    lamp.lamp_white(); //控制灯光为白色（开灯）
    voice_prompt.Vioce_prompt_run(0); //语音提示模块运行（姿态正确）
    frameData = human_detection.readFrameData(); //人体检测模块运行
    speech_recognition.Speech_get_result(0); //语音识别模块运行 
   
    
}

