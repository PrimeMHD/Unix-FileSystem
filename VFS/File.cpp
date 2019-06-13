#include "../include/File.h"
#include "../include/VirtualProcess.h"

/**
 * @comment:本文件的代码主要来自Unix V6++ File.cpp
 * 进行了一些删改
 * */

/*==============================class File===================================*/
File::File()
{
	this->f_count = 0;
	this->f_flag = 0;
	this->f_offset = 0;
	//this->f_inode = NULL;
	this->f_inode_id = 0;
}

File::~File()
{
	//nothing to do here
}

/*==============================class OpenFiles===================================*/
OpenFiles::OpenFiles()
{
}

OpenFiles::~OpenFiles()
{
}

int OpenFiles::AllocFreeSlot()
{
	int i;
	const User &u = VirtualProcess::Instance()->getUser();

	for (i = 0; i < OpenFiles::NOFILES; i++)
	{
		/* 进程打开文件描述符表中找到空闲项，则返回之 */
		if (this->ProcessOpenFileTable[i] == NULL)
		{
			/* 设置核心栈现场保护区中的EAX寄存器的值，即系统调用返回值 */
			return i;
		}
	}

	return -1;
}

File *OpenFiles::GetF(int fd)
{
	File *pFile;
	const User &u = VirtualProcess::Instance()->getUser();

	/* 如果打开文件描述符的值超出了范围 */
	if (fd < 0 || fd >= OpenFiles::NOFILES)
	{
		// u.u_error = User::EBADF;
		return NULL;
	}

	pFile = this->ProcessOpenFileTable[fd];
	if (pFile == NULL)
	{
		// u.u_error = User::EBADF;
	}

	return pFile; /* 即使pFile==NULL也返回它，由调用GetF的函数来判断返回值 */
}

void OpenFiles::SetF(int fd, File *pFile)
{
	if (fd < 0 || fd >= OpenFiles::NOFILES)
	{
		return;
	}
	/* 进程打开文件描述符指向系统打开文件表中相应的File结构 */
	this->ProcessOpenFileTable[fd] = pFile;
}

/*==============================class IOParameter===================================*/
IOParameter::IOParameter()
{
	this->m_Base = 0;
	this->m_Count = 0;
	this->m_Offset = 0;
}

IOParameter::~IOParameter()
{
	//nothing to do here
}
