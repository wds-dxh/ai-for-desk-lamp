/**
 * @file 人体检测（LD2410C）
 * @author wds
 * @date 2023/10/9
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
 */

#include <HardwareSerial.h>
#include "Human_detection.h"

/**
 * @brief 人体检测模块类
 * @param serial 串口
 * @param rxPin 串口接收引脚
 * @param txPin 串口发送引脚
 * @param baudRate 串口波特率
*/
Human_detection :: Human_detection(HardwareSerial& serial_int,uint32_t baudRate_int,uint8_t rxPin_int, uint8_t txPin_int)
    : serial(serial_int), rxPin(rxPin_int), txPin(txPin_int), baudRate(baudRate_int) {  //初始化列表

  // 初始化串口
  serial.begin(baudRate, SERIAL_8N1, rxPin, txPin);
  //初始化输入引脚27
  pinMode(27, INPUT);
}


/**
 * @brief 读取帧内数据
 * @return FrameData 帧内数据
*/
FrameData Human_detection :: readFrameData(){
  if (serial.available()) {
    // 读取帧头部
    uint8_t header[6];
    for (int i = 0; i < 6; i++) {
      header[i] = serial.read();
    }

    // 检查帧头部是否匹配
    if (header[0] == 0xF4 && header[1] == 0xF3 && header[2] == 0xF2 && header[3] == 0xF1 && header[4] == 0x0D && header[5] == 0x00) {
    Serial.println("ok ok ok ok ");
      // 读取帧内数据长度
      // uint16_t dataLength = serial.read() << 8 | serial.read();  // 读取帧内数据长度,高8位左移8位，低8位或运算

      // 创建帧内数据对象
      FrameData frameData;

      // 读取帧内数据
      frameData.targetStatus = serial.read();
      frameData.targetDistance = serial.read() << 8 | serial.read();
      frameData.targetEnergy = serial.read();
      frameData.stationaryDistance = serial.read() << 8 | serial.read();
      frameData.stationaryEnergy = serial.read();
      frameData.detectionDistance = serial.read() << 8 | serial.read();
      return frameData;
    }
  }
  
}


/**
 * @brief 显示帧内数据
 * @param frameData 帧内数据
 * @param serial_show 显示数据的串口
 * @return void
*/
void Human_detection :: frameData_show(HardwareSerial& serial_show,FrameData frameData){
  // 打印帧内数据
  // serial_show.print("Target Status: ");
  serial_show.println(frameData.targetStatus);
  // serial_show.print("Target Distance: ");
  // serial_show.println(frameData.targetDistance);
  // serial_show.print("Target Energy: ");
  // serial_show.println(frameData.targetEnergy);
  // serial_show.print("Stationary Distance: ");
  // serial_show.println(frameData.stationaryDistance);
  // serial_show.print("Stationary Energy: ");
  // serial_show.println(frameData.stationaryEnergy);
  // serial_show.print("Detection Distance: ");
  // serial_show.println(frameData.detectionDistance);
}


/**
 * @brief 判断是否有人
 * @return bool 有人返回true，无人返回false
*/
bool Human_detection :: isHuman(){
  if (digitalRead(27) == HIGH) {
    return true;
  } else {
    return false;
  }
}

