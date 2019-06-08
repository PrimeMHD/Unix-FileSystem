#ifndef INODE_POOL_H
#define INODE_POOL_H
#include "define.h"
#include "DiskInode.h"
#include "Bitmap.h"

/**
 * 磁盘Inode区，
 * 大小是一定的，Inode号是有限的。
 * 
 */
class InodePool{
  //TODO
  private:
    Bitmap inodePoolBitmap;
    char padding[2040];  //NOTE 这个是手工计算的，为的是让InodePool占满3个盘块
    DiskInode inodeBlock[MAX_INODE_NUM];  //INODE数组存放区域  Inode的大小为64字节
    

  public:
    InodePool();
    int ialloc();
    void ifree(InodeId inodeID);
    void iupdate(InodeId inodeId,DiskInode diskInode);  
    DiskInode* getInode(InodeId inodeID);

};


#endif
/**
 * 可以升级的地方：
 * 现代UNIX操作系统使用铭记inode，可以节省IO,creat系统调用不会因为ialloc入睡。
 * 有时间再升级。
 */

