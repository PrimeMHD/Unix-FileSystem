#ifndef EXT2_H
#define EXT2_H
#include "define.h"
#include "SuperBlock.h"
#include "InodePool.h"
#include "DiskInode.h"
#include "BufferCache.h"
#include "Path.h"

#define ROOT_INODE_ID 1
enum Ext2_Status
{
  Ext2_UNINITIALIZED,
  Ext2_NOFORM,
  Ext2_READY
}; //未初始化，完成挂载但是未格式化，挂载且格式化（或事先有格式）
class Ext2
{
private:
  //SuperBlock *p_superBlock;
  //磁盘SuperBlock的内存映像的指针。
  //因为神奇的mmap，这里可以用这个指针指向虚拟磁盘的superblock
  //InodePool *p_InodePool;     //DiskInode池
  BufferCache *p_bufferCache; //绑定的BufferCache,ext2不直接和DiskDriver打交道，透过这个缓存层
  Ext2_Status ext2_status = Ext2_UNINITIALIZED;

public:
  void format(); //格式化
  int registerFs();
  int unregisterFs();
  Ext2_Status getExt2Status();
  int setBufferCache(BufferCache *p_bufferCache);
  int allocNewInode(); //分配一个新的inode
  DiskInode getDiskInodeByNum(int inodeID);
  void updateDiskInode(int inodeID, DiskInode diskInode);

  InodeId locateInode(Path &path);
  InodeId locateDir(Path &path);
  InodeId getInodeIdInDir(InodeId dirInodeId, FileName fileName);

  int bmap(int inodeNum, int logicBlockNum); //文件中的地址映射。查混合索引表，确定物理块号。
  //逻辑块号bn=u_offset/512
  void loadSuperBlock(SuperBlock &superBlock);
};
#endif