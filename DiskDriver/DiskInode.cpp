#include "../include/DiskInode.h"

DiskInode::DiskInode()
{
    inode_id = 0;
    memset(i_addr, 0, 10 * sizeof(int));
}

DiskInode::DiskInode(Inode inode)
{

    inode_id = inode.inode_id;                      //inode号
    uid = inode.uid;                                //uid
    file_size = inode.file_size;                    //文件大小
    fileType = inode.fileType;                      //文件类型
    memcpy(i_addr, inode.i_addr, 10 * sizeof(int)); //混合索引表
} //转换构造函数