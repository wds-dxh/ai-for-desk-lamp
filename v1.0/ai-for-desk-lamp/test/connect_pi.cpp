/**
 * @file 连接树莓派
 * @author wds
 * @date 2023/10/30
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
 */

#include "connect_pi.h" 


/**
 * @brief 构造函数
 * @param 无
 * @return 无
*/
Connect_Pi::Connect_Pi(){
    Serial.begin(115200);
}


/**
 * @brief 读取树莓派数据
 * @param 无
 * @return pi_data 
*/
pi_data Connect_Pi::read_pi_data(){
    
    
 // 如果串口缓冲区中有足够的字节可供读取，就执行解析
  if (Serial.available() >= sizeof(pi_data)) {
    

    // 从串口读取数据包
    Serial.readBytes((char*)&packet, sizeof(pi_data));

    // 检查帧头部
    if (packet.header1 == 0x2C && packet.header2 == 0x12 && packet.footer == 0x5B) {
    return packet;
      // 解析数据
    //   int cx = packet.data1;
    //   int cy = packet.data2;
    //   int cw = packet.data3;
    //   int ch = packet.data4;

    } else {
      // 帧头部不匹配，丢弃数据或进行其他错误处理
      Serial.println("Invalid header.");
    }
  }
}