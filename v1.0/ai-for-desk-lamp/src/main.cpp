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
// #include <connect_pi.h>             // 包含 connect_pi 库的头文件
#include "WiFi_control.h"


typedef struct {
  uint8_t header1;
  uint8_t header2;
  uint8_t data1;
  uint8_t data2;
  uint8_t data3;
  uint8_t data4;
  uint8_t footer;

} pi_data;


RGBColor color = {255,255,255};//定义一个颜色结构体
extern int32_t luminance_wifi;  //灯光亮度
extern bool humanBaodyDetection_wifi; //人体检测
Adafruit_NeoPixel strip(144, 2, NEO_GRB + NEO_KHZ800);


Lamp lamp(255,144,2,color);//定义一个lamp对象0    初始颜色为白灯
Voice_prompt voice_prompt(5,18,19,23);//定义一个voice_prompt对象
Human_detection human_detection(Serial2,256000,16,17);//定义一个human_detection对象
WIFI_control wifi_control;//定义一个wifi_control对象
// Speech_recognition speech_recognition(true,0,"xiao ya",
// "hong deng","lv deng","lan deng","bai deng","hui deng","guan deng");//定义一个speech_recognition对象
// Connect_Pi connect_pi;//定义一个connect_pi对象

void Xcontrol_wifi(void *parameter) ;     //wifi控制小车运行
void Xothers(void *parameter) ;       //灯光
void Posture_reminder_control();      //姿态提醒
void human_detection_control();       //人体检测

pi_data packet;  // 用于存储数据包的变量

void setup() {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);//关闭低电压检测,避免无限重启
    wifi_control.WiFi_control_init();  // 初始化WIFI
    Serial.begin(115200);//初始化串口
    delay(1000);
    Serial.println("start");

    xTaskCreatePinnedToCore(Xcontrol_wifi, "TaskOne", 15000, NULL, 2, NULL, 0);  //Xcontrol_wifi在 0核心
    xTaskCreatePinnedToCore(Xothers, "TaskTwo", 4096, NULL, 1, NULL, 1);  //Xothers在 1核心
    
    lamp.lamp_white(); //控制灯光为白色（开灯）
}

void loop() {

// while (1)
//   {
//   wifi_control.WiFi_control_run();
//   lamp.strip.setBrightness(luminance_wifi); //设置亮度
  

//   if (humanBaodyDetection_wifi == true){  //是否开启人体检测
//   human_detection_control(); //人体检测
//   }

//   Posture_reminder_control(); //姿态提醒
  

//   }

}







void Xcontrol_wifi(void *pvParameters)
 
{
  while (1)
  {
    wifi_control.WiFi_control_run();
    // vTaskDelay(1000 / portTICK_PERIOD_MS); //延时1s
    Posture_reminder_control(); //姿态提醒
  }

   vTaskDelete(NULL);  

}
 




void Xothers(void *pvParameters) 
{
  while (1){
  strip.setBrightness(luminance_wifi); //设置亮度    不知道什么原因无法直接调用lamp的函数，只能这样调用
  //lamp.strip.setBrightness(luminance_wifi); //设置亮度

  if (humanBaodyDetection_wifi == true){  //是否开启人体检测
  human_detection_control(); //人体检测
  }

  Posture_reminder_control(); //姿态提醒
  // wifi_control.WiFi_control_run();
  }
  

  vTaskDelete(NULL);

}




void human_detection_control(){

  //雷达控制灯光
  if (human_detection.isHuman() == true){
  lamp.lamp_white(); //控制灯光为白色（开灯）
  }
  else{
  lamp.lamp_off(); //控制灯光为关闭
  }
  // Serial.println(human_detection.isHuman());


}



void Posture_reminder_control(){      //姿态提醒

 // 如果串口缓冲区中有足够的字节可供读取，就执行解析
  if (Serial.available() >= sizeof(pi_data)) {

    delay(1000);    //延时等待播放完成两个
    delay(200);
    Serial.readBytes((char*)&packet, sizeof(pi_data));
    if (packet.header1 == 0x2C && packet.header2 == 0x12 && packet.footer == 0x5B) {

        if (packet.data1 == 1){
        // voice_prompt.Vioce_prompt_run(1);       //姿态正确
        // Serial.println("right");
        // Serial.println(packet.data1);
        }
        if(packet.data1 == 2){
        voice_prompt.Vioce_prompt_run(2);       //姿态错误
        // Serial.println("down");
        // Serial.println(packet.data1);
        }
        if(packet.data1 == 3){
        // voice_prompt.Vioce_prompt_run(3);       //没有学习
        // Serial.println("no");
        // Serial.println(packet.data1);

        }
        if (packet.data1 == 0){
        voice_prompt.Vioce_prompt_run(0);       //无人
        // Serial.println("no people");
        // Serial.println(packet.data1);
        }

    } 
    
    else {
      // 帧头部不匹配，丢弃数据或进行其他错误处理
      Serial.println("Invalid header.");
    }
   
    
    }
       
}