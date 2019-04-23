#ifndef EXT2_H
#define EXT2_H
#include "define.h"
#include "SuperBlock.h"
#include "InodePool.h"
#include "Inode.h"


class Ext2{
  private:
    SuperBlock mSuperBlock; //磁盘SuperBlock的内存副本
    InodePool mInodePool; //内存Inode池
    

};
#endif