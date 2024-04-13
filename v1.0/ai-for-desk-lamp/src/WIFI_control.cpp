/**
 * @file WiFi控制类
 * @author wds
 * @date 2023/11/12
 * @email wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
*/

#define BLINKER_WIFI        //wifi连接模式
#define BLINKER_WITHOUT_SSL  //
// #define BLINKER_PRINT Serial    //定义调试串口


#include "WiFi_control.h"
#include "Blinker.h"
#include<lamp.h>


char auth[] = "6fd542ef6171";

char ssid[] = "wds";
char pswd[] = "wdsshy0320";


extern Lamp lamp;

// 数据传输对象
BlinkerNumber Number1("num-uwb");  
BlinkerNumber Number2("num-angle"); 


BlinkerButton Button1("btn-turnOn");  //开灯

BlinkerButton Button2("btn-turnOff"); //关灯

BlinkerButton Button3("btn-humenBaodyDetection"); //开启人体检测


BlinkerButton Button4("btn-toright"); 
BlinkerButton Button5("btn-toleft"); 

BlinkerButton Button6("btn-speedup"); 
BlinkerButton Button7("btn-speeddown"); 


//滑动条，控制灯光亮度
BlinkerSlider Slider1("luminance");  //灯光亮度

int32_t luminance_wifi =255;  
bool humanBaodyDetection_wifi = true; //人体检测


void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    
}


void button1_callback(const String & state) {       //开灯
    BLINKER_LOG("get button state: ", state);
    lamp.lamp_white();//白灯,开灯
  
}


void button2_callback(const String & state) {       //关灯
    BLINKER_LOG("get button state: ", state);
    lamp.lamp_off();//关灯
}


void button3_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    humanBaodyDetection_wifi = !humanBaodyDetection_wifi;
}


void button4_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);

}

void button5_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);

}

void button6_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);

}

void button7_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);

}


void slider1_callback(int32_t value)//滑动条，调节灯光亮度
{
    BLINKER_LOG("get slider value: ", value);
    luminance_wifi = value;
    //打印灯光亮度
    Serial.println(luminance_wifi);
}

/**
  * @brief    WiFi控制类初始化(链接blinker)
  * @param    None
  * @retval   None
  */
void WIFI_control :: WiFi_control_init(){     //构造函数

    #if defined(BLINKER_PRINT)//如果定义了BLINKER_PRINT
        BLINKER_DEBUG.stream(BLINKER_PRINT);//将BLINKER_PRINT串口重定向到BLINKER_DEBUG
    #endif
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);

    Blinker.attachData(dataRead);//绑定数据接收函数

    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
    Button5.attach(button5_callback);
    Button6.attach(button6_callback);
    Button7.attach(button7_callback);

    Slider1.attach(slider1_callback);//滑动条，调节灯光亮度

}



/**
  * @brief    WiFi控制小车运行
  * @param    None
  * @retval   None
  */
void WIFI_control :: WiFi_control_run(){
    Blinker.run();
    
}


void WIFI_control :: Wifi_data_transmission(int range){     //wifi数据传输
    Number1.print(range);
    
    
}