#ifndef EXT2_H
#define EXT2_H
#include "define.h"
#include "SuperBlock.h"
#include "InodePool.h"
#include "DiskInode.h"
#include "BufferCache.h"
#include "Path.h"


class Ext2{
  private:
    SuperBlock *mSuperBlock; 
    //磁盘SuperBlock的内存映像的指针。
    //因为神奇的mmap，这里可以用这个指针指向虚拟磁盘的superblock
    InodePool *mInodePool; //DiskInode池
    BufferCache *bufferCache; //绑定的BufferCache,ext2不直接和DiskDriver打交道，透过这个缓存层

  public:
    void  mount();
    void unmount();
    int setBufferCache(BufferCache* bufferCache);
    int allocNewInode();//分配一个新的inode
    DiskInode* getDiskInodeByNum(int inodeID);

    
    int getInodeNumByPath(Path path);
    int bmap(int inodeNum,int logicBlockNum);//文件中的地址映射。查混合索引表，确定物理块号。
    //逻辑块号bn=u_offset/512


};
#endif