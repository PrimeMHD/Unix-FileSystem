#ifndef SUPER_BLOCK_H
#define SUPER_BLOCK_H
#include "define.h"
#include "Bitmap.h"
/**
 * SuperBlock在安装磁盘的时候读入内存，磁盘使用过程改动内存超级块。
 * 最晚在磁盘卸载的时候，需要把脏超级块刷回磁盘。（这个时候磁盘缓存也会被刷回）
 */

/**
 * 我的超级块采用的是bitmap管理空闲盘块
 * 一个SuperBlock应该占据有一块盘块，大小为DISK_BLOCK_SIZE
 */
class SuperBlock
{
public:
  SuperBlock();
  size_t SuperBlockBlockNum = 1; //暂时考虑superblock占1个磁盘block
  int free_inode_num;            //空闲inode
  int free_block_bum;            //空闲盘块数
  int total_block_num;           //总盘块数
  int total_inode_num;           //总inode数
  Bitmap disk_block_bitmap;      //用bitmap管理空闲盘块
  char padding[2016];            //NOTE:这个2016是手工计算的结果。只针对ubuntu系统，也许别的机器就不对了。
                                 //确保一个SuperBlock填满一个block

  BlkNum balloc();
  void bfree(BlkNum blkNum);
  void bsetOccupy(BlkNum blkNum);
};

#endif