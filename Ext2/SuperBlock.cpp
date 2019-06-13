#include "../include/SuperBlock.h"

SuperBlock::SuperBlock() : disk_block_bitmap(DISK_SIZE / DISK_BLOCK_SIZE)
{

    total_inode_num = MAX_INODE_NUM - 1; //总inode数  -1是因为0#inode不可用
    free_inode_num = total_inode_num;    //空闲inode
    for (int i = 0; i < total_inode_num; i++)
    {
        s_inode[i] = total_inode_num - i;
    }
    //初始化空闲inode栈
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

/**
 * 分配空闲inode
 */
InodeId SuperBlock::ialloc()
{
    if (free_inode_num != 0)
    {
        return s_inode[--free_inode_num];
    }
    else
    {
        return ERROR_OUTOF_INODE;
    }
}

/**
 * 回收inode
 */
void SuperBlock::ifree(InodeId inodeId)
{
    s_inode[free_inode_num++]=inodeId;
}