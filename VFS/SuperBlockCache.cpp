#include "../include/SuperBlockCache.h"

SuperBlockCache::SuperBlockCache() : disk_block_bitmap(DISK_SIZE / DISK_BLOCK_SIZE)
{
}

/**
     * 分配一个空闲盘块
     */
BlkNum SuperBlockCache::balloc()
{
    int ret = disk_block_bitmap.getAFreeBitNum();
    disk_block_bitmap.setBit(ret);
    free_block_bum--;
    return ret;
}

/**
     * 回收一个盘块
     */
void SuperBlockCache::bfree(BlkNum blknum)
{
    free_block_bum++;
    disk_block_bitmap.unsetBit(blknum);
}
void SuperBlockCache::bsetOccupy(BlkNum blkNum)
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
