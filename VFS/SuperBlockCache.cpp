#include "../include/SuperBlockCache.h"

SuperBlockCache::SuperBlockCache() : disk_block_bitmap(DISK_SIZE / DISK_BLOCK_SIZE)
{
}

/**
 * 分配一个空闲盘块.如果返回-1表示分配失败！
 */
BlkNum SuperBlockCache::balloc()
{
    dirty = true;
    int ret = disk_block_bitmap.getAFreeBitNum();
    if (ret != -1)
    {
        disk_block_bitmap.setBit(ret);
        free_block_bum--;
    }

    return ret;
}

/**
     * 回收一个盘块
     */
void SuperBlockCache::bfree(BlkNum blknum)
{
    dirty = true;
    free_block_bum++;
    disk_block_bitmap.unsetBit(blknum);
}
void SuperBlockCache::bsetOccupy(BlkNum blkNum)
{

    dirty = true;
    if (disk_block_bitmap.getBitStat(blkNum))
    {
    }
    else
    {
        disk_block_bitmap.setBit(blkNum);
        free_block_bum--;
    }
}
