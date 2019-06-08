#include "../include/DiskInode.h"

/**
 * 从v6++中搬过来的构造函数
 */
DiskInode::DiskInode()
{
	/* 
	 * 如果DiskInode没有构造函数，会发生如下较难察觉的错误：
	 * DiskInode作为局部变量占据函数Stack Frame中的内存空间，但是
	 * 这段空间没有被正确初始化，仍旧保留着先前栈内容，由于并不是
	 * DiskInode所有字段都会被更新，将DiskInode写回到磁盘上时，可能
	 * 将先前栈内容一同写回，导致写回结果出现莫名其妙的数据。
	 */
	this->d_mode = 0;
	this->d_nlink = 0;
	this->d_uid = -1;
	this->d_gid = -1;
	this->d_size = 0;
	for (int i = 0; i < 10; i++)
	{
		this->d_addr[i] = 0;
	}
	this->d_atime = 0;
	this->d_mtime = 0;
}
DiskInode::DiskInode(unsigned int d_mode, int d_nlink, short d_uid, short d_gid, int d_size, int d_addr[10], int d_atime, int d_mtime)
{
	this->d_mode = d_mode;
	this->d_atime = d_nlink;
	this->d_uid = d_uid;
	this->d_gid = d_gid;
	this->d_size = d_size;
	memcpy(this->d_addr, d_addr, MIXED_ADDR_TABLE_SIZE);
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
	memcpy(d_addr, inode.i_addr, MIXED_ADDR_TABLE_SIZE);

} //转换构造函数