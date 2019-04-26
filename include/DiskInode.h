#ifndef DISK_INODE_H
#define DISK_INODE_H
#include "define.h"
#include "DiskBlock.h"

struct DiskInode{
    int inode_id;  //inode号
    int uid;  //uid
    size_t file_size;//文件大小
    FileType fileType; //文件类型
    DiskBlock* d_addr[10]; //混合索引表
};



#endif