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
 */
class SuperBlock{
  public:
    bool dirty=false;
    int free_inode_num;
    int free_block_bum;
    int total_block_num;
    int total_inode_num;
    Bitmap *p_disk_block_bitmap; //用bitmap管理空闲盘块


};

#endif