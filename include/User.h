#ifndef USER_H
#define USER_H
#include "define.h"
#include "DirectoryEntry.h"
#include "Inode.h"
#include "File.h"
#include "Path.h"
class User
{

public:
	/**
	 * @comment:User类是从Unix v6++中copy过来的，只保留与文件相关的数据结构
	 * 
	 */
	//User();
	/* 文件系统相关成员 */
	Inode *u_cdir; /* 指向当前目录的Inode指针 */
	Inode *u_pdir; /* 指向父目录的Inode指针 */

	DirectoryEntry u_dent; /* 当前目录的目录项 */
	//char u_dbuf[DirectoryEntry::DIRSIZ]; /* 当前路径分量 */
	//char u_curdir[128];					 /* 当前工作目录完整路径 */
	//Path u_curDir;
	InodeId curDirInodeId;

	/* 进程的用户标识 */
	/* 因为本应用没有多用户的概念，这里写死也无妨 */
	static const short u_uid = 1;  /* 有效用户ID */
	static const short u_gid = 1;  /* 有效组ID */
	static const short u_ruid = 1; /* 真实用户ID */
	static const short u_rgid = 1; /* 真实组ID */

	/* 文件系统相关成员 */
	OpenFiles u_ofiles; /* 进程打开文件描述符表对象 */

	/* 文件I/O操作 */
	IOParameter u_IOParam; /* 记录当前读、写文件的偏移量，用户目标区域和剩余字节数参数 */
};

#endif