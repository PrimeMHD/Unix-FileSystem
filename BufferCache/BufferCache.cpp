#include "../include/BufferCache.h"
#include "../include/Kernel.h"
#include "../include/VirtualProcess.h"
#include "../include/Logcat.h"

void BufferCache::setDiskDriver(DiskDriver *diskDriver)
{
    this->diskDriver = diskDriver;
}
int BufferCache::mount()
{
    initialize();
    return diskDriver->mount();
}
void BufferCache::unmount()
{
    Bflush();
    diskDriver->unmount();
} //unmount的时候需要把脏缓存刷回

/**
 * 缓存控制块的初始化。
 */
void BufferCache::initialize()
{
    int i;
    Buf *bp;

    this->bFreeList.b_forw = this->bFreeList.b_back = &(this->bFreeList);
    this->bFreeList.av_forw = this->bFreeList.av_back = &(this->bFreeList);

    for (i = 0; i < BUFFER_CACHE_NUM; i++)
    {
        bp = &(this->m_Buf[i]);
        bp->b_blkno = -1;
        bp->b_dev = -1;
        bp->b_addr = &Buffer[i];
        /* 初始化NODEV队列 */
        //bp->b_back = &(this->bFreeList);
        //bp->b_forw = this->bFreeList.b_forw;
        //this->bFreeList.b_forw->b_back = bp;
        //this->bFreeList.b_forw = bp;
        /* 初始化自由队列 */
        bp->b_flags = Buf::B_BUSY;
        Brelse(bp); //放入自有缓存队列
    }
}

/**
 * 将物理盘块一整块读入diskBlockPool
 */
Buf *BufferCache::Bread(int blkno)
{
    Buf *bp;
    /* 根据块号申请缓存 */
    bp = this->GetBlk(blkno);

    /* 如果在设备队列中找到所需缓存，即B_DONE已设置，就不需进行I/O操作 */
    if (bp->b_flags & Buf::B_DONE) //DONE是数据一致性的表示
    {
        return bp; //说明太好了，有现成的缓存块（副本和磁盘一致）
    }

    /* 没有找到相应缓存，构成I/O读请求块 */
    bp->b_flags |= Buf::B_READ;
    bp->b_wcount = DISK_BLOCK_SIZE;
    diskDriver->readBlk(blkno, bp->b_addr);
    bp->b_flags |= Buf::B_DONE;

    return bp;
}

/**
 * 将一个缓存块中的东西写回磁盘
 */
void BufferCache::Bwrite(Buf *bp)
{
    unsigned int flags;

    flags = bp->b_flags;
    bp->b_flags &= ~(Buf::B_READ | Buf::B_DONE | Buf::B_ERROR | Buf::B_DELWRI);
    bp->b_wcount = DISK_BLOCK_SIZE;

    //this->m_DeviceManager->GetBlockDevice(Utility::GetMajor(bp->b_dev)).Strategy(bp);

    if ((flags & Buf::B_DELWRI) == 0)
    {
        /*
     * 如果不是延迟写，则检查错误；否则不检查。
     * 这是因为如果延迟写，则很有可能当前进程不是
     * 操作这一缓存块的进程，而在GetError()主要是
     * 给当前进程附上错误标志。
     */
        Logcat::log("BufferCache", "bwrite出错！");
    }
    else
    {
        diskDriver->writeBlk(bp->b_blkno, *bp->b_addr);
        bp->b_flags |= Buf::B_DONE;
        std::cout << bp->b_blkno << std::endl;
    }

    return;
}

/**
 * 延迟写磁盘块
 */
void BufferCache::Bdwrite(Buf *bp)
{
    /* 置上B_DONE允许其它进程使用该磁盘块内容 */
    bp->b_flags |= (Buf::B_DELWRI | Buf::B_DONE);
    this->Brelse(bp);
    return;
}

/**
 * 将dev指定设备队列中延迟写的缓存全部输出到磁盘.可能是在卸载磁盘的时候，需要全部刷回
 */
void BufferCache::Bflush()
{
    Buf *bp;

    for (bp = this->bFreeList.b_forw; bp != &(this->bFreeList); bp = bp->b_forw)
    {
        /* 找出自由队列中所有延迟写的块 */
        if ((bp->b_flags & Buf::B_DELWRI))
        {
            //bwrite方法会清除B_DELWRI标志的
            //this->NotAvail(bp);
            this->Bwrite(bp);
            //this->Brelse(bp);
            //std::cout << bp->b_blkno << std::endl;
        }
    }

    return;
}
//void writeBlk(int blkno, const DiskBlock &contentToWrite); //将内存的一块区域，写入缓冲区（如果不在缓冲区的话，需要先读）

/**
 * 申请一块缓存，用于读写设备dev上的字符块blkno
 */
Buf *BufferCache::GetBlk(int blkno)
{
    Buf *bp;
    //Devtab *dp;
    User &u = VirtualProcess::Instance()->getUser();

    /* 
	 * 如果设备队列中已经存在相应缓存，则返回该缓存；
	 * 否则从自由队列中分配新的缓存用于字符块读写。
	 */

    /* 表示请求NODEV设备中字符块 */

    /* 首先在该设备队列中搜索是否有相应的缓存 */
    for (bp = bFreeList.b_forw; bp != &bFreeList; bp = bp->b_forw)
    {
        /* 不是要找的缓存，则继续 */
        if (bp->b_blkno != blkno)
            continue;

        /* 从自由队列中抽取出来 */
        this->NotAvail(bp);
        return bp; //可能会有延迟写标志
    }

    /* 取自由队列第一个空闲块 */
    bp = this->bFreeList.av_forw;
    this->NotAvail(bp);

    /* 如果该字符块是延迟写，将其异步写到磁盘上 */
    if (bp->b_flags & Buf::B_DELWRI)
    {
        this->Bwrite(bp);
    }
    /* 注意: 这里清除了所有其他位，只设了B_BUSY */
    bp->b_flags = Buf::B_BUSY; //若有延迟写bit，也一并消除了
    bp->b_blkno = blkno;
    memset(bp->b_addr, 0, DISK_BLOCK_SIZE);
    if (bp->b_dev != devno)
    {
        //加入设备缓存队列
        bp->b_back = &(this->bFreeList);
        bp->b_forw = this->bFreeList.b_forw;
        this->bFreeList.b_forw->b_back = bp;
        this->bFreeList.b_forw = bp;
        bp->b_dev = devno;
    }

    return bp;
}

/**
 * 获取自由缓存队列控制块Buf对象引用
 */
Buf &BufferCache::GetBFreeList()
{
    return this->bFreeList;
}

//TODO 这里有bug
void BufferCache::NotAvail(Buf *bp)
{

    /* 从自由队列中取出 */
    bp->av_back->av_forw = bp->av_forw;
    bp->av_forw->av_back = bp->av_back;
    /* 设置B_BUSY标志 */
    bp->b_flags |= Buf::B_BUSY;
    return;
}

/**
 * 将一个缓存控制块放到自有缓存队列中，
 * 注意以下操作并没有清除B_DELWRI、B_WRITE、B_READ、B_DONE标志
 * B_DELWRI表示虽然将该控制块释放到自由队列里面，但是有可能还没有些到磁盘上。
 * B_DONE则是指该缓存的内容正确地反映了存储在或应存储在磁盘上的信息 
 */
void BufferCache::Brelse(Buf *bp)
{

    bp->b_flags &= ~(Buf::B_WANTED | Buf::B_BUSY | Buf::B_ASYNC); //消除这些位的符号（这里其实没用到）
    (this->bFreeList.av_back)->av_forw = bp;
    bp->av_back = this->bFreeList.av_back;
    bp->av_forw = &(this->bFreeList);
    this->bFreeList.av_back = bp;
    return;
}
