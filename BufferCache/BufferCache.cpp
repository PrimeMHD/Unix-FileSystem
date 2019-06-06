#include "../include/BufferCache.h"
void BufferCache::setDiskDriver(DiskDriver *diskDriver)
{
    this->diskDriver = diskDriver;
}
int BufferCache::mount()
{
    return diskDriver->mount();
}
void BufferCache::unmount()
{

} //unmount的时候需要把脏缓存刷回

void BufferCache::readBlk(int blkNum)
{

} //将物理盘块一整块读入diskBlockPool

void BufferCache::writeBlk(int blkNum, const DiskBlock &contentToWrite)
{

} //将内存的一块区域，写入缓冲区（如果不在缓冲区的话，需要先读）
void BufferCache::BDwrite()
{

} //TODO 延迟写，未生命完整