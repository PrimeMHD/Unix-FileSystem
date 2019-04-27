#ifndef INODE_H
#define INODE_H
#include "define.h"
#include "DiskBlock.h"
#include "DiskInode.h"

/**
 * 内存Inode结构
 * 本来想用类的，但是想想好像也没啥method，还是用结构体好了
 */
class Inode
{
  public:
    bool dirty; //脏位
    int inode_id;  //inode号
    int uid;  //uid
    size_t file_size;//文件大小
    FileType fileType; //文件类型
    DiskBlock* d_addr[10]; //混合索引表
    Inode(DiskInode d_inode);
};

#endif