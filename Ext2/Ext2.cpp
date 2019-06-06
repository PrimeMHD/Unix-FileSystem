#include "../include/Ext2.h"
#include "../include/Kernel.h"

void Ext2::updateDiskInode(int inodeID, DiskInode diskInode)
{
}

/**
 * 这个函数貌似是最格格不入的。
 * 为了简便，做硬写入。不经过缓存层，直接借助mmap对img进行写入完成初始化。
 */
void Ext2::format()
{
    //0# superblock
    //1,2# inodePool
    // 3~DISK_BLOCK_NUM# 放数据
    void *diskMemAddr = (void *)Kernel::instance()->getDiskDriver().getDiskMemAddr();
    //构造一个superBlock结构
    SuperBlock tempSuperBlock;
    //tempSuperBlock.
    SuperBlock* p_superBlock=(SuperBlock*)diskMemAddr;


    ext2_status = Ext2_READY;
    //如果格式话成功，将ext2_status置ready
}

int Ext2::registerFs()
{
    /**
 * mount的前一步在vfs.cpp中完成
 * 
 * 
 * 
 */
    int mountRes = this->p_bufferCache->mount(); //②DiskDriver打开虚拟磁盘img，mmap，进入就绪状态
    if (mountRes == -1)
    {
        ext2_status = Ext2_UNINITIALIZED;
    }
    else if (mountRes == 0) //有现成的(认为已经格式化)  //NOTE 这里可以提升
    {
        ext2_status = Ext2_READY;
    }
    else if (mountRes == 1)
    { // 新生成的img，还有待格式化

        //③ext模块中的指针赋值，指向img文件内存映射的地址。
        ext2_status = Ext2_NOFORM;
    }

    //④装载SuperBlock到VFS的SuperBlock缓存(这一步需要经过缓存层)
    return OK;
}

int Ext2::setBufferCache(BufferCache *p_bufferCache)
{
    this->p_bufferCache = p_bufferCache;
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

Ext2_Status Ext2::getExt2Status()
{
    return ext2_status;
}
