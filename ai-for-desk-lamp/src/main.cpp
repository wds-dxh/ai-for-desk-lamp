/**
 * @file 主程序
 * @author wds
 * @date 2023/10/9
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
 */
#include <soc/soc.h> 
#include <soc/rtc_cntl_reg.h>  //关闭低电压检测,避免无限重启

#include <Arduino.h>
#include <lamp.h>                   // 包含 lamp 库的头文件
#include <Voice_prompt.h>           // 包含 Voice_prompt 库的头文件
#include <Human_detection.h>        // 包含 Human_detection 库的头文件
#include <Speech_recognition.h>     // 包含 Speech_recognition 库的头文件

RGBColor color = {255,255,255};//定义一个颜色结构体
Lamp lamp(255,144,2,color);//定义一个lamp对象
Voice_prompt voice_prompt(5,18,19,23);//定义一个voice_prompt对象
Human_detection human_detection(Serial2,256000,16,17);//定义一个human_detection对象
// Speech_recognition speech_recognition(true,0,"xiao ya",
// "hong deng","lv deng","lan deng","bai deng","hui deng","guan deng");//定义一个speech_recognition对象

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);//关闭低电压检测,避免无限重启
    Serial.begin(115200);//初始化串口
    delay(1000);
    Serial.println("草泥马");
}

void loop() {

        lamp.lamp_white(); //控制灯光为白色（开灯）
        Serial.println("lamp_white");
        delay(1000);

}

