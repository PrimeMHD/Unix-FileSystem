#include "../include/Ext2.h"

void Ext2::updateDiskInode(int inodeID, DiskInode diskInode)
{
}

void Ext2::format()
{
}

int Ext2::registerFs()
{
    /**
 * mount的前一步在vfs.cpp中完成
 * 
 * 
 * 
 */
    bufferCache->mount(); //②DiskDriver打开虚拟磁盘img，mmap，进入就绪状态
    //③ext模块中的指针赋值，指向img文件内存映射的地址。
    //④装载SuperBlock到VFS的SuperBlock缓存(这一步需要经过缓存层)
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

/**
 * VFS在inodeDirectoryCache失效的时候，会调用本函数，在磁盘上根据路径确定inode号
 */
InodeId Ext2::locateInode(Path path)
{
    return OK;
}

/**
 * VFS在inodeDirectoryCache失效的时候，会调用本函数，在磁盘上根据路径确定inode号
 */
InodeId Ext2::locateDir(Path path)
{
    return OK;
}

//根据目录的inode和文件名，在目录表中查找
InodeId Ext2::getInodeIdInDir(InodeId dirInodeId, FileName fileName)
{
    return OK;
}

int Ext2::bmap(int inodeNum, int logicBlockNum)
{

    return OK;
} //文件中的地址映射。查混合索引表，确定物理块号。
  //逻辑块号bn=u_offset/512