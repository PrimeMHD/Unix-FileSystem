#ifndef SUPER_BLOCK_H
#define SUPER_BLOCK_H
#include "define.h"
#include "Bitmap.h"

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
    Bitmap disk_block_bitmap; //用bitmap管理空闲盘块


};

#endif