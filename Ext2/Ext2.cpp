#include "../include/Ext2.h"
#include "../include/Kernel.h"
#include "../include/VirtualProcess.h"
#include "../include/TimeHelper.h"

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
    //1,2,3# inodePool
    // 4~DISK_BLOCK_NUM-1# 放数据
    void *diskMemAddr = (void *)Kernel::instance()->getDiskDriver().getDiskMemAddr();

    //①构造一个superBlock结构，写入磁盘中
    SuperBlock tempSuperBlock;
    tempSuperBlock.total_block_num = DISK_BLOCK_NUM;
    tempSuperBlock.free_block_bum = DISK_BLOCK_NUM;
    tempSuperBlock.total_inode_num = MAX_INODE_NUM;
    tempSuperBlock.free_inode_num = MAX_INODE_NUM;
    tempSuperBlock.bsetOccupy(0); //0#盘块被superblock占据
    tempSuperBlock.bsetOccupy(1);
    tempSuperBlock.bsetOccupy(2);
    tempSuperBlock.bsetOccupy(3); //1~3#盘块被inodePool占据(即磁盘Inode区)
    tempSuperBlock.bsetOccupy(4); //4#盘块放根目录文件
    tempSuperBlock.bsetOccupy(5); //5#盘块放bin目录文件
    tempSuperBlock.bsetOccupy(6); //6#盘块放etc目录文件
    tempSuperBlock.bsetOccupy(7); //7#盘块放home目录文件
    tempSuperBlock.bsetOccupy(8); //8#盘块放dev目录文件
    tempSuperBlock.free_block_bum -= 9;
    tempSuperBlock.free_inode_num -= 5;
    SuperBlock *p_superBlock = (SuperBlock *)diskMemAddr;
    *p_superBlock = tempSuperBlock; //没有动态申请，不用管深浅拷贝
    p_superBlock++;
    diskMemAddr = (void *)p_superBlock;
    //②构造DiskInode,修改InodePool,将InodePool写入磁盘img
    int tempAddr[10]={4,0,0,0,0,0,0,0,0,0};
    DiskInode tempDiskInode = DiskInode(0, 1, VirtualProcess::Instance()->Getuid(), VirtualProcess::Instance()->Getgid(),6*sizeof(DirectoryEntry),tempAddr,TimeHelper::getCurTime(),TimeHelper::getCurTime());

        //③数据区写入目录文件

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
