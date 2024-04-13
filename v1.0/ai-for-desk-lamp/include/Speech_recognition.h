#ifndef __SPEECH_RECOGNITION_H__
#define __SPEECH_RECOGNITION_H__

/**
 * @brief 语音识别类
 * @param int_entry 是否打印识别结果
 * @param serial_int 是否初始化串口通信
 * @param words1 词条1
 * @param words2 词条2
 * @param words3 词条3
 * @param words4 词条4
 * @param words5 词条5
 * @param words6 词条6
 * @param words7 词条7
*/
class Speech_recognition{
private:
    bool int_entry; //是否打印识别结果
    uint32_t serial_int; //是否初始化串口通信
    const char * words1; //词条1
    const char * words2; //词条2
    const char * words3; //词条3
    const char * words4; //词条4
    const char * words5; //词条5
    const char * words6; //词条6
    const char * words7; //词条7
    
public:
    //构造函数，初始化词表
    Speech_recognition(bool int_entry_int, uint32_t serial_int1,const char * words1_int,const char * words2_int,const char * words3_int,const char * words4_int,const char * words5_int,const char * words6_int,const char * words7_int);
    unsigned char Speech_get_result(bool print);  //获取识别结果

};









#endif // __SPEECH_RECOGNITION_H__