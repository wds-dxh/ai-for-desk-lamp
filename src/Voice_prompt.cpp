/**
 * @file 语音提示模块类
 * @author wds
 * @date 2023/10/8
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
*/

#include "Voice_prompt.h"

/**
 * @brief 构造函数
 * @param VOICE_PROMPT_PIN_correct_int 语音提示模块引脚--坐姿正确
 * @param VOICE_PROMPT_PIN_error_int 语音提示模块引脚--坐姿错误
 * @param VOICE_PROMPT_PIN_rest_int 语音提示模块引脚--休息提示
 * @param VOICE_PROMPT_PIN_national_anthem_int 语音提示--国歌
 * @return 无
*/
Voice_prompt :: Voice_prompt(uint8_t VOICE_PROMPT_PIN_correct_int,uint8_t VOICE_PROMPT_PIN_error_int,uint8_t VOICE_PROMPT_PIN_rest_int,uint8_t VOICE_PROMPT_PIN_national_anthem_int)
{
    VOICE_PROMPT_PIN_correct = VOICE_PROMPT_PIN_correct_int;
    VOICE_PROMPT_PIN_error = VOICE_PROMPT_PIN_error_int;
    VOICE_PROMPT_PIN_rest = VOICE_PROMPT_PIN_rest_int;
    VOICE_PROMPT_PIN_national_anthem = VOICE_PROMPT_PIN_national_anthem_int;
    pinMode(VOICE_PROMPT_PIN_correct,OUTPUT);   
    pinMode(VOICE_PROMPT_PIN_error,OUTPUT);
    pinMode(VOICE_PROMPT_PIN_rest,OUTPUT);
    pinMode(VOICE_PROMPT_PIN_national_anthem,OUTPUT);

    digitalWrite(VOICE_PROMPT_PIN_correct, HIGH);           //初始化语音提示引脚为高电平
    digitalWrite(VOICE_PROMPT_PIN_error, HIGH);             //初始化语音提示引脚为高电平
    digitalWrite(VOICE_PROMPT_PIN_rest, HIGH);              //初始化语音提示引脚为高电平 
    digitalWrite(VOICE_PROMPT_PIN_national_anthem, HIGH);   //初始化语音提示引脚为高电平
}


/**
 * @brief 语音提示模块运行
 * @param sign 语音提示模块运行标志位
 * @return 无
*/
void Voice_prompt :: Vioce_prompt_run(uint8_t sign){

  switch (sign)
    {
    case 1:   //雨雪天气正常天气
        digitalWrite(VOICE_PROMPT_PIN_correct, LOW);
        digitalWrite(VOICE_PROMPT_PIN_error, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_rest, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_national_anthem, HIGH);

        delay(50);
        digitalWrite(VOICE_PROMPT_PIN_correct, HIGH);  //复位引脚保证能重复触发单个语音提示

        break;


    case 2:  //雨雪天气
        digitalWrite(VOICE_PROMPT_PIN_correct, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_error, LOW);
        digitalWrite(VOICE_PROMPT_PIN_rest, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_national_anthem, HIGH);

        delay(50);
        digitalWrite(VOICE_PROMPT_PIN_error, HIGH);  //复位引脚保证能重复触发单个语音提示

        break;


    case 3:  //预留提示
        digitalWrite(VOICE_PROMPT_PIN_correct, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_error, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_rest, LOW);
        digitalWrite(VOICE_PROMPT_PIN_national_anthem, HIGH);

        delay(50);
        digitalWrite(VOICE_PROMPT_PIN_rest, HIGH);  //复位引脚保证能重复触发单个语音提示

        break;


    case 4://预留提示
        digitalWrite(VOICE_PROMPT_PIN_correct, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_error, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_rest, HIGH);
        digitalWrite(VOICE_PROMPT_PIN_national_anthem, LOW);

        delay(50);  
        digitalWrite(VOICE_PROMPT_PIN_national_anthem, HIGH);  //复位引脚保证能重复触发单个语音提示

        break;


    default:
        break;
    }
    
    //避免误触发
    delay(10);

}

