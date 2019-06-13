#include "../include/SuperBlockCache.h"
#include "../include/SuperBlock.h"
#include "../include/Kernel.h"

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
void SuperBlockCache::flushBack()
{
    SuperBlock tempSuperBlock;
    tempSuperBlock.disk_block_bitmap = this->disk_block_bitmap;
    tempSuperBlock.free_block_bum = this->free_block_bum;
    tempSuperBlock.free_inode_num = this->free_inode_num;
    tempSuperBlock.total_block_num = this->total_block_num;
    tempSuperBlock.total_inode_num = this->total_inode_num;
    tempSuperBlock.SuperBlockBlockNum = this->SuperBlockBlockNum;
    memcpy(tempSuperBlock.s_inode, this->s_inode, sizeof(this->s_inode));
    Buf *pBuf = Kernel::instance()->getBufferCache().GetBlk(0);
    SuperBlock *p_superBlock = (SuperBlock *)pBuf->b_addr;
    *p_superBlock = tempSuperBlock; //没有动态申请，不用管深浅拷贝
    Kernel::instance()->getBufferCache().Bdwrite(pBuf);

    //下面是硬写入（不经过缓存）
    // DiskBlock *diskMemAddr = Kernel::instance()->getDiskDriver().getDiskMemAddr();
    // SuperBlock *p_superBlock = (SuperBlock *)diskMemAddr;
    // *p_superBlock = tempSuperBlock; //没有动态申请，不用管深浅拷贝
}

InodeId SuperBlockCache::ialloc()
{
    dirty = true;
    if (free_inode_num != 0)
    {
        return s_inode[--free_inode_num];
    }
    else
    {
        return ERROR_OUTOF_INODE;
    }
}
void SuperBlockCache::ifree(InodeId inodeId)
{
    dirty = true;

    s_inode[free_inode_num++] = inodeId;
}
