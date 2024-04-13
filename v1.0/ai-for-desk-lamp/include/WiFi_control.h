#ifndef __WIFI_CONTROL_H__
#define __WIFI_CONTROL_H__

#include <Arduino.h>   // Arduino核心库

/**
 * @brief WiFi控制类
 * @param none
 * @return none
*/
class WIFI_control{



public:
    void WiFi_control_init();//初始化
    // WIFI_control(int a); //构造函数
    void WiFi_control_run();     //运行
    void Wifi_data_transmission(int range);//wifi数据传输

};

#endif