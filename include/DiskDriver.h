#ifndef DISK_DRIVER_H
#define DISK_DRIVER_H
#include "include/define.h"
#include "include/DiskBlock.h"

/**
 * DiskDriver类：
 * 操纵linux系统调用。本程序的最底层。
 * 主要功能：装载、卸载img文件，按照块的方式读写“磁盘”(即img文件)
 * 
 */
class DiskDriver{
  private:
    bool isMounted=false;
  public:
    void mount();
    void unmount();
    DiskBlock* getBlk();
    void readBlk();
    void writeBlk();


};

#endif