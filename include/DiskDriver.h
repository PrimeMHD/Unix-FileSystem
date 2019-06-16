#ifndef DISK_DRIVER_H
#define DISK_DRIVER_H
#include "define.h"
#include "DiskBlock.h"
#include "DiskInode.h"
const int devno = 12;
#define DEF_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
/**
 * DiskDriver类：
 * 操纵linux系统调用。本程序的最底层。
 * 主要功能：装载、卸载img文件，按照块的方式读写“磁盘”(即img文件)
 * 
 */
class DiskDriver
{
private:
  bool isMounted = false;
  FileFd DiskFd; //挂载磁盘文件的句柄
  DiskBlock *DiskMemAddr;
  const char *TAG;

public:
  DiskDriver();
  ~DiskDriver();
  int mount();                                       //安装img磁盘
  void unmount();                                    //卸载磁盘
  DiskBlock *getBlk(int blockNum);                   //获得指向块的指针
  void readBlk(int blockNum, DiskBlock *dst);        //读取块
  void writeBlk(int blockNum, const DiskBlock &blk); //写入块
  bool isDiskMounted();
  DiskBlock *getDiskMemAddr();
};

#endif