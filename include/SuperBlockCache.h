#ifndef SUPER_BLOCK_CACHE_H
#define SUPER_BLOCK_CACHE_H
#include "define.h"
#include "Bitmap.h"

class SuperBlockCache
{
public:
    SuperBlockCache();
    bool dirty = false;
    
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