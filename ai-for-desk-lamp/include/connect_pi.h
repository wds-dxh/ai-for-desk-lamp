#ifndef __CONNECT_PI_H__
#define __CONNECT_PI_H__
#include <Arduino.h>

typedef struct {
  uint8_t header1;
  uint8_t header2;
  uint8_t data1;
  uint8_t data2;
  uint8_t data3;
  uint8_t data4;
  uint8_t footer;

} pi_data;


class Connect_Pi {

public:
    Connect_Pi();
    pi_data read_pi_data();
    pi_data packet;  // 用于存储数据包的变量
    



};



#endif // __CONNECT_PI_H_