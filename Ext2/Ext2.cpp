#include "../include/Ext2.h"

void Ext2::updateDiskInode(int inodeID, DiskInode diskInode)
{
}


/**
 * mount的前一步在vfs.cpp中完成
 * ②DiskDriver打开虚拟磁盘img，mmap，进入就绪状态
 * ③装载SuperBlock到VFS的SuperBlock缓存
 * 
 */
void Ext2::mount()
{



}
void Ext2::unmount()
{
}
int Ext2::setBufferCache(BufferCache *bufferCache)
{
    return OK;
}
int Ext2::allocNewInode()
{
    return OK;
}
DiskInode *Ext2::getDiskInodeByNum(int inodeID)
{
    return nullptr;
}

int Ext2::getInodeNumByPath(Path path)
{
    return OK;
}
int Ext2::bmap(int inodeNum, int logicBlockNum)
{

    return OK;
} //文件中的地址映射。查混合索引表，确定物理块号。
  //逻辑块号bn=u_offset/512