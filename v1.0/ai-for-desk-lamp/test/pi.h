#ifndef __PI_H__
#define __PI_H__

#include <stdint.h>
// 定义数据结构
struct DataPacket{
  uint8_t frameHeader1;
  uint8_t frameHeader2;
  int16_t cx;
  int16_t cy;
  int16_t cw;
  int16_t ch;
  uint8_t frameFooter;
};


class Pi {
private:
    DataPacket packet;

    
public:
    int16_t connect_pi();






};






#endif // __PI_H__