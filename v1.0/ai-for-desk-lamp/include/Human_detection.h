#ifndef __HUMAN_DETECTION_H__
#define __HUMAN_DETECTION_H__

#include <Arduino.h>

// 帧内数据结构体
typedef struct {
  byte targetStatus;         // 0: 无人 1: 有人
  uint16_t targetDistance;      // 目标距离
  uint8_t targetEnergy;         // 目标能量
  uint16_t stationaryDistance;  // 静止目标距离
  uint8_t stationaryEnergy;     // 静止目标能量
  uint16_t detectionDistance;   // 检测距离
} FrameData;


/**
 * @brief 人体检测模块类
 * @param serial 串口
 * @param rxPin 串口接收引脚
 * @param txPin 串口发送引脚
 * @param baudRate 串口波特率
*/
class Human_detection {
private:
    HardwareSerial& serial; // 串口  （使用引用，不需要拷贝构造函数）串口2
    uint32_t baudRate;      // 串口波特率  人体检测模块为256000
    uint8_t rxPin;          // 串口接收引脚  --  16
    uint8_t txPin;          // 串口发送引脚  --  17


public:
    
    Human_detection(HardwareSerial& serial_int, uint32_t baudRate_int, uint8_t rxPin_int, uint8_t txPin_int);// 构造函数
    FrameData readFrameData();// 读取帧内数据
    void frameData_show(HardwareSerial& serial_show,FrameData frameData);// 显示帧内数据
    bool isHuman();// 判断是否有人
 
};



#endif // __HUMAN_DETECTION_H__