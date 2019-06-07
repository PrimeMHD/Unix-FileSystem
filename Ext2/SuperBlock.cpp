#include "../include/SuperBlock.h"

SuperBlock::SuperBlock() : disk_block_bitmap(DISK_SIZE / DISK_BLOCK_SIZE)
{
}

/**
     * 分配一个空闲inode号
     */
BlkNum SuperBlock::balloc()
{
    int ret = disk_block_bitmap.getAFreeBitNum();
    disk_block_bitmap.setBit(ret);
    return ret;
}

/**
     * 回收一个inode
     */
void SuperBlock::bfree(BlkNum blknum)
{
    disk_block_bitmap.unsetBit(blknum);
    //暂时不用对inode块做什么，只用标记其free了
}
void SuperBlock::bsetOccupy(BlkNum blkNum)
{
    disk_block_bitmap.setBit(blkNum);
}
