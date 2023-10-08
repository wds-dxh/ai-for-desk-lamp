#ifndef __LAMP_H__
#define __LAMP_H__
/**
 * @file 台灯控制类
 * @brief 用于控制台灯的开关、亮度、颜色等
 * @param uint8_t luminance; //亮度
 * @param uint8_t LED_COUNT; //灯珠数量
 * @param uint8_t LED_PIN;   //控制引脚
 * @param RGBColor color;    //颜色
*/
#include <stdint.h>

//定义RGB颜色结构体
struct RGBColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Lamp
{
private:
    uint8_t luminance; //亮度
    uint8_t LED_COUNT; //灯珠数量
    uint8_t LED_PIN;   //控制引脚
    RGBColor color;    //颜色

public:

    void lamp(uint8_t luminance,uint8_t LED_COUNT, uint8_t LED_PIN,RGBColor color_int); //构造函数
    void lamp_color(RGBColor color_transfer);  // 控制全部灯光
    void lamp_luminance(uint8_t luminance_transfer); //控制亮度
    void lamp_white(); //控制灯光为白色（开灯）
    void lamp_off(); //控制灯光为黑色（关灯）






};








#endif // __LAMP_H__