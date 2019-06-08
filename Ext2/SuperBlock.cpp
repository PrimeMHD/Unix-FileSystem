#include "../include/SuperBlock.h"

SuperBlock::SuperBlock() : disk_block_bitmap(DISK_SIZE / DISK_BLOCK_SIZE)
{
}

/**
     * 分配一个空闲盘块号
     */
BlkNum SuperBlock::balloc()
{
    int ret = disk_block_bitmap.getAFreeBitNum();
    disk_block_bitmap.setBit(ret);
    free_block_bum--;
    return ret;
}

/**
     * 回收一个盘块
     */
void SuperBlock::bfree(BlkNum blknum)
{
    free_block_bum++;
    disk_block_bitmap.unsetBit(blknum);
    //暂时不用对inode块做什么，只用标记其free了
}
void SuperBlock::bsetOccupy(BlkNum blkNum)
{
    if (disk_block_bitmap.getBitStat(blkNum))
    {
    }
    else
    {
        disk_block_bitmap.setBit(blkNum);
        free_block_bum--;
    }
}
