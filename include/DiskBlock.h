#ifndef DISKBLOCK_H
#define DISKBLOCK_H

#include "define.h"
class DiskBlock{
  private:
    uint8_t content[DISK_BLOCK_SIZE];
    //数据存放区域，大小为DISK_BLOCK_SIZE个字节
  public:
    

};

#endif