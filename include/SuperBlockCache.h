#ifndef SUPER_BLOCK_CACHE_H
#define SUPER_BLOCK_CACHE_H
#include "define.h"
#include "Bitmap.h"

class SuperBlockCache
{
public:
    bool dirty = false;
    int free_inode_num;
    int free_block_bum;
    int total_block_num;
    int total_inode_num;
    Bitmap *p_disk_block_bitmap; //用bitmap管理空闲盘块
};

#endif