/**
 * @file 连接树莓派
 * @author wds
 * @date 2023/11/9
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
 */

#include <stdint.h>
#include <Arduino.h>


int16_t connect_pi() {
        int16_t cx = 0;
        int16_t cy = 0;
        int16_t cw = 0;
        int16_t ch = 0;

  if (Serial.available() >= sizeof(packet)) {
    // 读取数据包
    
    Serial.readBytes((uint8_t*)&packet, sizeof(packet));

    if (packet.frameHeader2 == 0x2C && packet.frameFooter == 0x12 &&packet.frameHeader1 == 0x5B) {
        int16_t cx = packet.cx;
        int16_t cy = packet.cy;
        int16_t cw = packet.cw;
        int16_t ch = packet.ch;
        // Serial.print("cx: ");
        // Serial.println(cx);
        // Serial.print("cy: ");
        // Serial.println(cy);
        // Serial.print("cw: ");
        // Serial.println(cw);
        // Serial.print("ch: ");
        // Serial.println(ch);
        return cx;
    }
    else{
        Serial.println("error");
    }
    
  }
}
