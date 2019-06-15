#include "../include/OpenFileTable.h"
#include "../include/Kernel.h"
#include "../include/VirtualProcess.h"
/*==============================class OpenFileTable===================================*/
/* 系统全局打开文件表对象实例的定义 */
OpenFileTable g_OpenFileTable;

OpenFileTable::OpenFileTable()
{
    //nothing to do here
}

OpenFileTable::~OpenFileTable()
{
    //nothing to do here
}

/*作用：进程打开文件描述符表中找的空闲项  之 下标  写入 u_ar0[EAX]*/
File *OpenFileTable::FAlloc()
{

    for (int i = 0; i < OpenFileTable::NFILE; i++)
    {
        /* f_count==0表示该项空闲 */
        if (this->m_File[i].f_count == 0)
        {
            /* 建立描述符和File结构的勾连关系 */
            //u.u_ofiles.SetF(fd, &this->m_File[i]);
            /* 增加对file结构的引用计数 */
            this->m_File[i].f_count++;
            /* 清空文件读、写位置 */
            this->m_File[i].f_offset = 0;
            return (&this->m_File[i]);
        }
    }

    printf("No Free File Struct\n");
    //u.u_error = User::ENFILE;
    return NULL;
}

void OpenFileTable::CloseF(File *pFile)
{
    Inode *pNode;
    //ProcessManager &procMgr = Kernel::Instance().GetProcessManager();

    if (pFile->f_count <= 1)
    {
        /*
    	 * 如果当前进程是最后一个引用该文件的进程，
    	 * 对特殊块设备、字符设备文件调用相应的关闭函数
    	 */
        // pFile->f_inode->CloseI(pFile->f_flag & File::FWRITE);
        // g_InodeTable.IPut(pFile->f_inode);
        pFile->f_inode_id = 0;
        pFile->f_offset = 0;
    }

    /* 引用当前File的进程数减1 */
    pFile->f_count--;
}