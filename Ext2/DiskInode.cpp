#include "../include/DiskInode.h"

// DiskInode::DiskInode()
// {
//     //TODO
//     //inode_id = 0;
//     //memset(i_addr, 0, 10 * sizeof(int));
// }
DiskInode::DiskInode(unsigned int d_mode, int d_nlink, short d_uid, short d_gid, int d_size, int d_addr[10], int d_atime, int d_mtime)
{
    this->d_mode = d_mode;
    this->d_atime = d_nlink;
    this->d_uid = d_uid;
    this->d_gid = d_gid;
    this->d_size = d_size;
    memcpy(this->d_addr, d_addr, sizeof(d_addr));
    this->d_atime = d_atime;
    this->d_mtime = d_mtime;
}

/**
 * 将内存inode刷回磁盘的时候，需要用这个进行构造
 * NOTE: DiskInode的d_atime和d_mtime属性，不是从内存inode中转换的，而是根据系统当前时间。
 */
DiskInode::DiskInode(Inode inode)
{
    d_mode = inode.i_mode;
    d_nlink = inode.i_nlink;
    d_uid = inode.i_uid;
    d_gid = inode.i_gid;
    d_size = inode.i_size;
    memcpy(d_addr, inode.i_addr, sizeof(inode.i_addr));

} //转换构造函数