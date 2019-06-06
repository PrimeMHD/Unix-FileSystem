#ifndef BUFFER_CACHE_H
#define BUFFER_CACHE_H
#include "define.h"
#include "DiskDriver.h"
#include "DiskBlock.h"
#include "BufferLruList.h"

class BufferCache
{
private:
  DiskBlock diskBlockPool[BUFFER_CACHE_NUM]; //缓存空间
  BufferLruList bufferLruList;
  DiskDriver *diskDriver;

public:
  void setDiskDriver(DiskDriver *diskDriver);
  void mount();
  void unmount();                                             //unmount的时候需要把脏缓存刷回
  void readBlk(int blkNum);                                   //将物理盘块一整块读入diskBlockPool
  void writeBlk(int blkNum, const DiskBlock &contentToWrite); //将内存的一块区域，写入缓冲区（如果不在缓冲区的话，需要先读）
  void BDwrite();                                             //TODO 延迟写，未生命完整
};

#endif