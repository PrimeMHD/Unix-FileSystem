#ifndef BUFFER_CACHE_H
#define BUFFER_CACHE_H
#include "define.h"
#include "DiskDriver.h"
#include "DiskBlock.h"
#include "BufferLruList.h"
#include "Buf.h"

struct DevTab
{
  Buf *b_forw;
};
class BufferCache
{
private:
  Buf bFreeList; //自由缓存队列控制块，将缓存控制块中b_addr指向相应缓冲区首地址。
  //需要设备缓存队列吗？毕竟只有一个块设备
  //DevTab devtab;
  Buf m_Buf[BUFFER_CACHE_NUM];        //缓存控制块数组
  DiskBlock Buffer[BUFFER_CACHE_NUM]; //缓存块空间
  //BufferLruList bufferLruList;
  DiskDriver *diskDriver;

public:
  void initialize(); //缓存控制块的初始化。
  void setDiskDriver(DiskDriver *diskDriver);
  int mount();
  void unmount();        //unmount的时候需要把脏缓存刷回
  Buf *Bread(int blkno); //将物理盘块一整块读入diskBlockPool
  void Bwrite(Buf *bp);  //写一个磁盘块
  void Bdwrite(Buf *bp); //延迟写磁盘块
  void Bflush();         //将dev指定设备队列中延迟写的缓存全部输出到磁盘.可能是在卸载磁盘的时候，需要全部刷回
  //void writeBlk(int blkno, const DiskBlock &contentToWrite); //将内存的一块区域，写入缓冲区（如果不在缓冲区的话，需要先读）
  Buf *GetBlk(int blkno); /* 申请一块缓存，用于读写设备dev上的字符块blkno。*/
  void Brelse(Buf *bp);   /* 释放缓存控制块buf */
  Buf &GetBFreeList();    //获取自由缓存队列控制块Buf对象引用
  void NotAvail(Buf *bp);
};

#endif