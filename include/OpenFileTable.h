#ifndef OPEN_FILE_TABLE_H
#define OPEN_FILE_TABLE_H
#include "define.h"
#include "File.h"

class OpenFileTable
{
public:
    /* static consts */
    //static const int NINODE	= 100;	/* 内存Inode的数量 */
    static const int NFILE = 100; /* 打开文件控制块File结构的数量 */

    /* Functions */
public:
    /* Constructors */
    OpenFileTable();
    /* Destructors */
    ~OpenFileTable();

    // /*
    // * @comment 根据用户系统调用提供的文件描述符参数fd，
    // * 找到对应的打开文件控制块File结构
    // */
    // File* GetF(int fd);
    /* 
	 * @comment 在系统打开文件表中分配一个空闲的File结构
	 */
    File *FAlloc();
    /* 
	 * @comment 对打开文件控制块File结构的引用计数f_count减1，
	 * 若引用计数f_count为0，则释放File结构。
	 */
    void CloseF(File *pFile);

    /* Members */
public:
    File m_File[NFILE]; /* 系统打开文件表，为所有进程共享，进程打开文件描述符表
								中包含指向打开文件表中对应File结构的指针。*/
};

#endif