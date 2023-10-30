/**
 * @file 语音识别模块
 * @author wds
 * @date 2023/10/9
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
 */

#include "ASR.h"  // 包含 ASR 库的头文件
#include "Speech_recognition.h"

/**
 * @brief 构造函数，初始化语音识别模块，录入词条
 * @param int_entry_int 是否初始化词条，传入 true 则初始化词条，传入 false 则不初始化词条
 * @param serial_int 是否初始化串口通信 ，传入串口波特率则初始化串口通信，不传入则不初始化串口通信
 * @param words1_int 词条1
 * @param words2_int 词条2
 * @param words3_int 词条3
 * @param words4_int 词条4
 * @param words5_int 词条5
 * @param words6_int 词条6
 * @param words7_int 词条7
 * @return 无
*/
Speech_recognition ::Speech_recognition(bool int_entry_int, uint32_t serial_int1,const char * words1_int,
const char * words2_int,const char * words3_int,const char * words4_int,const char * words5_int,
const char * words6_int,const char * words7_int)
: int_entry(int_entry_int), serial_int(serial_int1), words1(words1_int), 
words2(words2_int), words3(words3_int),words4(words4_int),words5(words5_int),
words6(words6_int),words7(words7_int){

    unsigned char cleck = 0xff;  // 定义并初始化 cleck 变量为 0xff
    unsigned char asr_version = 0;  // 定义并初始化 asr_version 变量为 0
    Wire.begin();  // 初始化 I2C 总线
    Wire.setClock(100000);  // 设置 I2C 通信速率为 100000（100kHz）
    
    #if serial_int
    Serial.begin(115200);  // 初始化串口通信，波特率设置为 115200
    #endif
    WireReadData(FIRMWARE_VERSION, &asr_version, 1);  // 通过 I2C 从 ASR 模块读取固件版本号，并将结果存储在 asr_version 变量中
    #if serial_int
    Serial.print("asr_version is ");  // 打印字符串 "asr_version is "
    Serial.println(asr_version);  // 打印 asr_version 变量的值
    #endif

#if 1  // 如果为 1，则写入词条，如果为 0，则不写入词条
    I2CWrite(ASR_CLEAR_ADDR, 0x40);  // 通过 I2C 向 ASR 模块发送命令，清除掉电保存区，录入前需要清除掉电保存区
    BusyWait();  // 等待操作完成
    #if serial_int
    Serial.println("clear flash is ok");  // 打印字符串 "clear flash is ok"
    #endif
    I2CWrite(ASR_MODE_ADDR, 1);  // 通过 I2C 向 ASR 模块发送命令，设置检测模式
    BusyWait();  // 等待操作完成
    #if serial_int
    Serial.println("mode set is ok");  // 打印字符串 "mode set is ok"
    #endif
    AsrAddWords(0, words1); 
    BusyWait();  // 等待操作完成

    AsrAddWords(1, words2);  
    BusyWait();  // 等待操作完成

    AsrAddWords(2, words3);  
    BusyWait();  // 等待操作完成

    AsrAddWords(3, words4);  
    BusyWait();  // 等待操作完成

    AsrAddWords(4, words5);  
    BusyWait();  // 等待操作完成

    AsrAddWords(5, words6);  
    BusyWait();  // 等待操作完成

    AsrAddWords(6, words7);  
    BusyWait();  // 等待操作完成

    while (cleck != 7) {  // 循环读取 ASR 模块的识别序号值，直到读取到的值为 7
        WireReadData(ASR_NUM_CLECK, &cleck, 1);  // 通过 I2C 从 ASR 模块读取识别序号值，并将结果存储在 cleck 变量中
        #if serial_int
        Serial.println(cleck);  // 打印 cleck 变量的值
        #endif
        delay(100);  // 延迟 100 毫秒
    }
    #if serial_int
    Serial.println("cleck is ok");  // 打印字符串 "cleck is ok"
    #endif
#endif

    I2CWrite(ASR_REC_GAIN, 0x40);  // 通过 I2C 向 ASR 模块发送命令，设置识别的灵敏度为 0x40（建议范围为 0x40-0x55）
    I2CWrite(ASR_VOICE_FLAG, 1);  // 通过 I2C 向 ASR 模块发送命令，设置识别结果提示音开关为开启状态
    I2CWrite(ASR_BUZZER, 1);  // 通过 I2C 向 ASR 模块发送命令，开启蜂鸣器
    RGB_Set(255, 255, 255);  // 设置模块的 RGB 灯为白色
    delay(500);  // 延迟 500 毫秒
    I2CWrite(ASR_BUZZER, 0);  // 通过 I2C 向 ASR 模块发送命令，关闭蜂鸣器
    RGB_Set(0, 0, 0);  // 关闭 RGB 灯



}


/**
 * @brief 获取语音识别结果
 * @param print 是否打印识别结果
 * @return result 识别结果
*/
unsigned char Speech_recognition :: Speech_get_result(bool print){
    unsigned char result;  // 定义 result 变量
    WireReadData(ASR_RESULT, &result, 1);  // 通过 I2C 从 ASR 模块读取识别序号值，并将结果存储在 result 变量中
    delay(100);  // 延迟 100 毫秒
    #if print
    Serial.println(result);  // 打印 result 变量的值
    #endif
    return result;  // 返回 result 变量的值
}


