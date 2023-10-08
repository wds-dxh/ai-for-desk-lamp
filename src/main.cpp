/**
 * @file 主程序
 * @author wds
 * @date 2023/9/14
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
 */

#include <Arduino.h>
#include <lamp.h>

RGBColor color = {255,255,255};//定义一个颜色结构体

void setup() {

Lamp lamp(255,144,2,color);//定义一个lamp对象

}

void loop() {
    
    void lamp_white(); //控制灯光为白色（开灯）
    
}

