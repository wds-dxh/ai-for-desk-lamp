#ifndef __VOICE_PROMPT_H__
#define __VOICE_PROMPT_H__

#include <Arduino.h>


/**
 * @file 语音提示模块类
 * @brief 用于控制语音提示模块的运行
 * @param uint8_t VOICE_PROMPT_PIN_correct;     //语音提示模块引脚--坐姿正确
 * @param uint8_t VOICE_PROMPT_PIN_error;     //语音提示模块引脚--坐姿错误
 * @param uint8_t VOICE_PROMPT_PIN_rest;  //语音提示模块引脚--休息提示
 * @param uint8_t VOICE_PROMPT_PIN_national_anthem;      //语音提示--国歌
*/
class Voice_prompt{

private:
    uint8_t VOICE_PROMPT_PIN_correct;     //语音提示模块引脚--坐姿正确
    uint8_t VOICE_PROMPT_PIN_error;     //语音提示模块引脚--坐姿错误
    uint8_t VOICE_PROMPT_PIN_rest;  //语音提示模块引脚--休息提示
    uint8_t VOICE_PROMPT_PIN_national_anthem;      //语音提示--国歌
public:
    Voice_prompt(uint8_t VOICE_PROMPT_PIN_correct_int,uint8_t VOICE_PROMPT_PIN_error_int,uint8_t VOICE_PROMPT_PIN_rest_int,uint8_t VOICE_PROMPT_PIN_national_anthem_int); //构造函数
    void Vioce_prompt_run(uint8_t sign); //语音提示模块运行

};




#endif // __VOICE_PROMPT_H__