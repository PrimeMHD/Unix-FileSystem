#ifndef INODE_POOL_H
#define INODE_POOL_H
#include "define.h"
#include "Inode.h"
#include "Bitmap.h"

/**
 * 磁盘Inode区，
 * 大小是一定的，Inode号是有限的。
 * 
 */
class InodePool{
  //TODO
  private:
    Inode inodeBlock[MAX_INODE_NUM];  //INODE数组存放区域
    Bitmap inodePoolBitmap;

  public:
    InodePool();
    int ialloc();
    void ifree(int InodeID);
    Inode* getInode(int InodeID);

};


#endif
/**
 * 可以升级的地方：
 * 现代UNIX操作系统使用铭记inode，可以节省IO,creat系统调用不会因为ialloc入睡。
 * 有时间再升级。
 */

