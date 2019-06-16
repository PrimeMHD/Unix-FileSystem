#include "../include/VFS.h"
#include "../include/Logcat.h"
#include "../include/Kernel.h"
#include "../include/VirtualProcess.h"
VFS::VFS()
{
}

VFS::~VFS()
{
}

void VFS::mount()
{
    /**
     * 装载磁盘的最上层命令调用函数：
     * 硬盘装载的步骤：
     * ①内存inodeCache初始化
     * ②DiskDriver打开虚拟磁盘img，mmap，进入就绪状态
     * ③装载SuperBlock到VFS的SuperBlock缓存
     * 
     *  */
    inodeCache->clearCache(); //完成①
    if (OK == p_ext2->registerFs())
    {
        Mounted = true;
    } //完成②③
    // 成功的话将Mounted设置为true
}
void VFS::unmount()
{
    if (!Mounted)
    {
        printf("ERROR!磁盘未装载，无需卸载！\n");
    }
    else
    {
        //刷回InodeCache,SuperBlockCache
        inodeCache->flushAllCacheDirtyInode();
        if (superBlockCache->dirty)
        {
            superBlockCache->flushBack();
        }
        p_ext2->unregisterFs();
        Mounted = false;
        //刷回磁盘缓存
    }
}

int VFS::format()
{
    if (!Mounted)
    {
        printf("ERROR!磁盘未装载！\n");
        return ERROR_NOTSPEC;
    }
    else
    {
        switch (p_ext2->getExt2Status())
        {
        case Ext2_UNINITIALIZED:
            printf("ERROR!磁盘装载错误！\n");
            break;
        case Ext2_NOFORM:
            p_ext2->format();
            break;
        case Ext2_READY:
            printf("WARNING!磁盘可能已有数据！确定要格式化吗？\n");
            printf("Press \"y\" for yes, \"n\" for no:");
            char temp_cmd;
            while (temp_cmd = getchar())
            {
                if (temp_cmd == 'y')
                {
                    p_ext2->format();
                    break;
                }
                else if (temp_cmd == 'n')
                {
                    return ERROR_CANCEL;
                    break;
                }
                else
                {
                    printf("\nPress \"y\" for yes, \"n\" for no:");
                }
            }
            break;
        default:
            break;
        }
    }
    return OK;
}
/**
 * 在当前目录下创建文件，
 * 文件名为fileName,返回新创建文件的inodeId
 */
InodeId VFS::createFile(const char *fileName)
{
    InodeId newFileInode = -1;

    //Step0:查看有无同名的，若有则创建失败
    Path path(fileName);
    InodeId checkExsistInodeId = p_ext2->locateInode(path);
    if (checkExsistInodeId > 0)
    {
        return ERROR_FILENAME_EXSIST;
    }

    //Step1:为新文件分配新inode
    newFileInode = superBlockCache->ialloc(); //得到inode号
    if (newFileInode <= 0)
    {
        return newFileInode;
    }
    Inode *p_inode = inodeCache->getInodeByID(newFileInode); //并将这个inode写入inodeCache
    p_inode->i_flag = Inode::IUPD | Inode::IACC;
    p_inode->i_size = 0;
    p_inode->i_mode = 0;
    p_inode->i_nlink = 1;
    p_inode->i_uid = VirtualProcess::Instance()->Getuid();
    p_inode->i_gid = VirtualProcess::Instance()->Getgid();
    p_inode->i_number = newFileInode;
    //Step2:在当前目录文件中写入新的目录项
    Inode *p_dirInode = inodeCache->getInodeByID(VirtualProcess::Instance()->getUser().curDirInodeId);
    int blkno = p_dirInode->Bmap(0); //Bmap查物理块号
    Buf *pBuf;
    pBuf = Kernel::instance()->getBufferCache().Bread(blkno);
    DirectoryEntry *p_directoryEntry = (DirectoryEntry *)pBuf->b_addr;

    int i;
    for (i = 0; i < DISK_BLOCK_SIZE / sizeof(DirectoryEntry); i++)
    {
        if ((p_directoryEntry->m_ino == 0)) //找到目录文件中可以见缝插针的地方，填入县创建的inode信息
        {

            p_directoryEntry->m_ino = newFileInode;
            strcpy(p_directoryEntry->m_name, fileName);
            //std::cout << p_directoryEntry->m_name << " ";
            break;
        } //ino==0表示该文件被删除

        p_directoryEntry++;
    }
    if (i == DISK_BLOCK_SIZE / sizeof(DirectoryEntry))
    {
        return ERROR_NOTSPEC;
    }
    Kernel::instance()->getBufferCache().Bdwrite(pBuf);
    //Kernel::instance()->getBufferCache().Brelse(pBuf);

    //Step3:暂时未分配盘块

    return newFileInode;
}

InodeId VFS::deleteDir(const char *dirName)
{
    //目录文件和普通文件要分别处理！
    Path path(dirName);
    InodeId deleteFileInode = p_ext2->locateInode(path);
    if (deleteFileInode < 0)
    {
        return deleteFileInode;
    }

    Inode *p_delete_inode = inodeCache->getInodeByID(deleteFileInode);
    Inode *p_dirInode = inodeCache->getInodeByID(p_ext2->locateDir(path));
    if ((p_delete_inode->i_mode & Inode::IFMT) == Inode::IFDIR) //目录文件
    {
        //递归删除该目录下的所有文件
        int blkno = p_delete_inode->Bmap(0); //Bmap查物理块号
        Buf *pBuf;
        pBuf = Kernel::instance()->getBufferCache().Bread(blkno);
        DirectoryEntry *p_directoryEntry = (DirectoryEntry *)pBuf->b_addr;

        int de_i;
        for (de_i = 0; de_i < DISK_BLOCK_SIZE / sizeof(DirectoryEntry); de_i++)
        {
            if ((p_directoryEntry->m_ino != 0)) //找到目录文件中可以见缝插针的地方，填入县创建的inode信息
            {
                if (!strcmp(p_directoryEntry->m_name, ".") || !strcmp(p_directoryEntry->m_name, ".."))
                {
                    continue;
                }
                else
                {
                    if ((inodeCache->getInodeByID(p_directoryEntry->m_ino)->i_mode & Inode::IFMT) == Inode::IFDIR)
                    {
                        deleteDir(p_directoryEntry->m_name);
                    }
                    else
                    {
                        deleteFile(p_directoryEntry->m_name);
                    }
                }

            } //ino==0表示该文件被删除

            p_directoryEntry++;
        }
        Kernel::instance()->getBufferCache().Bdwrite(pBuf);
        //删除该目录本身
        deleteDirect(dirName);
    }
    else
    {
        Logcat::log("非法删除!");
        return ERROR_DELETE_FAIL;
    }
    return deleteFileInode;
}

/**
 * 删除文件
 */
InodeId VFS::deleteFile(const char *fileName)
{

    //目录文件和普通文件要分别处理！
    Path path(fileName);
    InodeId deleteFileInode = p_ext2->locateInode(path);
    if (deleteFileInode < 0)
    {
        return deleteFileInode;
    }
    Inode *p_delete_inode = inodeCache->getInodeByID(deleteFileInode);
    Inode *p_dirInode = inodeCache->getInodeByID(p_ext2->locateDir(path));
    if ((p_delete_inode->i_mode & Inode::IFMT) == 0) //普通文件
    {

        return deleteDirect(fileName);
    }
    else
    {
        Logcat::log("非法删除!");
        return ERROR_DELETE_FAIL;
    }
}

/**
 * 直接删除
 */
InodeId VFS::deleteDirect(const char *fileName)
{

    Path path(fileName);
    InodeId deleteFileInode = p_ext2->locateInode(path);
    if (deleteFileInode < 0)
    {
        return ERROR_DELETE_FAIL;
    }

    Inode *p_delete_inode = inodeCache->getInodeByID(deleteFileInode);
    Inode *p_dirInode = inodeCache->getInodeByID(p_ext2->locateDir(path));

    BlkNum phyno;
    //Step1 释放盘块
    for (int lbn = 0; (phyno = p_delete_inode->Bmap(lbn)) <= 0; lbn++)
    {
        superBlockCache->bfree(phyno);
    }
    //Step2 删除目录项
    int dirblkno = p_dirInode->Bmap(0); //Bmap查物理块号
    Buf *pBuf;
    pBuf = Kernel::instance()->getBufferCache().Bread(dirblkno);
    DirectoryEntry *p_directoryEntry = (DirectoryEntry *)pBuf->b_addr;

    int de_i;
    for (de_i = 0; de_i < DISK_BLOCK_SIZE / sizeof(DirectoryEntry); de_i++)
    {
        if ((p_directoryEntry->m_ino == p_delete_inode->i_number)) //找到目录文件中可以见缝插针的地方，填入县创建的inode信息
        {

            p_directoryEntry->m_ino = 0;
            break;
        } //ino==0表示该文件被删除

        p_directoryEntry++;
    }
    if (de_i == DISK_BLOCK_SIZE / sizeof(DirectoryEntry))
    {
        return ERROR_DELETE_FAIL;
    }
    Kernel::instance()->getBufferCache().Bdwrite(pBuf);
    //Step3 释放inode
    p_delete_inode->i_flag = 0; //这里是为了不再把删除的inode刷回，只用在superblock中标记inode删除即可
    superBlockCache->ifree(deleteFileInode);
    return deleteFileInode;
}

/**
 * 创建目录
 */
int VFS::mkDir(const char *dirName)
{
    int newDirInodeId = createFile(dirName);
    if (newDirInodeId < 0)
    {
        return ERROR_FILENAME_EXSIST;
    }

    Inode *p_inode = inodeCache->getInodeByID(newDirInodeId);
    p_inode->i_mode = Inode::IFDIR;

    DirectoryEntry tempDirectoryEntry;
    Buf *pBuf;

    BlkNum blkno = p_inode->Bmap(0);
    pBuf = Kernel::instance()->getBufferCache().Bread(blkno);
    DirectoryEntry *p_directoryEntry = (DirectoryEntry *)pBuf->b_addr;

    strcpy(tempDirectoryEntry.m_name, ".");
    tempDirectoryEntry.m_ino = newDirInodeId;
    *p_directoryEntry = tempDirectoryEntry;
    p_directoryEntry++;
    strcpy(tempDirectoryEntry.m_name, "..");
    tempDirectoryEntry.m_ino = VirtualProcess::Instance()->getUser().curDirInodeId;
    *p_directoryEntry = tempDirectoryEntry;
    Kernel::instance()->getBufferCache().Bdwrite(pBuf);
    return OK;
}
int VFS::cd(const char *dirName)
{
    Path path(dirName);
    InodeId targetInodeId = p_ext2->locateInode(path);
    if (targetInodeId <= 0)
    {
        Logcat::log("目录查找失败！");
    }
    else if ((inodeCache->getInodeByID(targetInodeId)->i_mode & Inode::IFMT) != Inode::IFDIR)
    {
        Logcat::log("ERROR! cd 命令的参数必须是目录！");
    }
    else
    {
        VirtualProcess::Instance()->getUser().curDirInodeId = targetInodeId;
    }

    //df
    return targetInodeId;
}

void VFS::ls(InodeId dirInodeID)
{
    //首先要获得这个inode->访问这个目录文件
    //step1: 检查inodeCache中有没有，有则直接用，没有则向Ext2模块要
    Inode &inode = *inodeCache->getInodeByID(dirInodeID);
    if (inode.i_mode & Inode::IFMT != Inode::IFDIR)
    {
        printf("ERROR! ls的参数只能为空或者目录名！\n");
        return;
    }

    inode.i_flag |= Inode::IACC;
    //Step2：读这个目录文件到缓存块中（可能已经存在于缓存块中,规定目录文件不能超过4096B）
    int blkno = inode.Bmap(0); //Bmap查物理块号
    Buf *pBuf;
    pBuf = Kernel::instance()->getBufferCache().Bread(blkno);
    DirectoryEntry *p_directoryEntry = (DirectoryEntry *)pBuf->b_addr;
    //Step3：访问这个目录文件中的entry，打印出来（同时缓存到dentryCache中）
    //TODO 缓存到dentryCache中
    for (int i = 0; i < DISK_BLOCK_SIZE / sizeof(DirectoryEntry); i++)
    {
        if ((p_directoryEntry->m_ino != 0))
        {
            std::cout << p_directoryEntry->m_name << " ";
        } //ino==0表示该文件被删除

        p_directoryEntry++;
    }
    std::cout << std::endl;
    Kernel::instance()->getBufferCache().Brelse(pBuf);
}

void VFS::ls(const char *dirName)
{
    //首先要根据目录名，确定inode号
    //step1 在DirectoryEntry中查找有没有现成的
    InodeId dirInodeId;

    Path path(dirName); //解析dirName转化为Path对象
    //先查一下directoryCache中有没有存dirName的目录项
    //TODO 先暂时不做VFS层的dentry缓存

    //没有，则向Ext模块要
    dirInodeId = p_ext2->locateInode(path);
    if ((inodeCache->getInodeByID(dirInodeId)->i_mode & Inode::IFMT) == Inode::IFDIR)
    {
        ls(dirInodeId);
    }
    else
    {
        Logcat::log("ERROR!ls指令只能对目录");
    }
}

/**
 * 打开一个普通文件,返回文件的句柄
 */
FileFd VFS::open(Path path, int mode)
{
    FileFd fd;
    //Step1. 查找该文件的inode号
    InodeId openFileInodeId = p_ext2->locateInode(path);
    //Step2. 检查打开合法性(省略了文件本身读写的限定)
    Inode *p_inodeOpenFile = inodeCache->getInodeByID(openFileInodeId);
    if (p_inodeOpenFile->i_mode & Inode::IFMT != 0)
    {
        return ERROR_OPEN_ILLEGAL; //在本程序中，只有普通文件可以open
    }
    p_inodeOpenFile->i_flag |= Inode::IACC;
    //Step3. 分配FILE结构
    File *pFile = Kernel::instance()->m_OpenFileTable.FAlloc();
    if (pFile == NULL)
    {
        //分配失败
        return ERROR_OUTOF_OPENFILE;
    }
    //Step4. 建立钩连关系,u_ofile[]中的一项指向FILE
    User &u = VirtualProcess::Instance()->getUser();
    /* 在进程打开文件描述符表中获取一个空闲项 */
    fd = u.u_ofiles.AllocFreeSlot();
    if (fd < 0) /* 如果寻找空闲项失败 */
    {
        return ERROR_OUTOF_FILEFD;
    }
    u.u_ofiles.SetF(fd, pFile);
    pFile->f_flag = mode & (File::FREAD | File::FWRITE);
    pFile->f_inode_id = openFileInodeId; //NOTE 这里有没有问题？如果inode被替换出内存了呢？
    return fd;
}
int VFS::close(FileFd fd)
{

    User &u = VirtualProcess::Instance()->getUser();

    /* 获取打开文件控制块File结构 */
    File *pFile = u.u_ofiles.GetF(fd);
    if (NULL == pFile)
    {
        return ERROR_CLOSE_FAIL;
    }

    /* 释放打开文件描述符fd，递减File结构引用计数 */
    u.u_ofiles.SetF(fd, NULL);
    Kernel::instance()->m_OpenFileTable.CloseF(pFile);
    return OK;
}

/**
 * 从文件fd中读出length字节放到content缓冲区中。
 * 返回读出的字节数，如果fd剩下的字节小于length，则只把剩下的读出
 */
int VFS::read(int fd, u_int8_t *content, int length)
{
    //分析：length可能大于、小于、等于盘块的整数倍
    int readByteCount = 0;

    User &u = VirtualProcess::Instance()->getUser();
    File *p_file = u.u_ofiles.GetF(fd);
    Inode *p_inode = inodeCache->getInodeByID(p_file->f_inode_id);
    p_inode->i_flag |= Inode::IUPD;
    Buf *pBuf;

    if (length > p_inode->i_size - p_file->f_offset + 1)
    {
        length = p_inode->i_size - p_file->f_offset + 1;
    }

    while (readByteCount < length && p_file->f_offset <= p_inode->i_size) //NOTE 这里是<还是<=再考虑一下
    {
        BlkNum logicBlkno = p_file->f_offset / DISK_BLOCK_SIZE; //逻辑盘块号
        BlkNum phyBlkno = p_inode->Bmap(logicBlkno);            //物理盘块号
        int offsetInBlock = p_file->f_offset % DISK_BLOCK_SIZE; //块内偏移
        pBuf = Kernel::instance()->getBufferCache().Bread(phyBlkno);
        u_int8_t *p_buf_byte = (u_int8_t *)pBuf->b_addr;
        p_buf_byte += offsetInBlock;
        if (length - readByteCount <= DISK_BLOCK_SIZE - offsetInBlock + 1)
        { //要读大小<=当前盘块剩下的,读需要的大小

            memcpy(content, p_buf_byte, length - readByteCount);
            p_file->f_offset += length - readByteCount;
            readByteCount = length;
            content += length - readByteCount;
            //修改offset
        }
        else
        { //把剩下的全部读出来
            memcpy(content, p_buf_byte, DISK_BLOCK_SIZE - offsetInBlock + 1);
            p_file->f_offset += DISK_BLOCK_SIZE - offsetInBlock + 1;
            readByteCount += DISK_BLOCK_SIZE - offsetInBlock + 1;
            content += DISK_BLOCK_SIZE - offsetInBlock + 1;
            //修改offset
        }
        Kernel::instance()->getBufferCache().Brelse(pBuf);
    }

    return readByteCount;
}
int VFS::write(int fd, u_int8_t *content, int length)
{
    //分析：length可能大于、小于、等于盘块的整数倍
    int writeByteCount = 0;

    User &u = VirtualProcess::Instance()->getUser();
    File *p_file = u.u_ofiles.GetF(fd);
    Inode *p_inode = inodeCache->getInodeByID(p_file->f_inode_id);
    p_inode->i_flag |= Inode::IUPD;

    Buf *pBuf;
    while (writeByteCount < length) //NOTE 这里是<还是<=再考虑一下
    {
        BlkNum logicBlkno = p_file->f_offset / DISK_BLOCK_SIZE; //逻辑盘块号
        if (logicBlkno == 1030)
        {
            printf("暂时停下");
        }
        BlkNum phyBlkno = p_inode->Bmap(logicBlkno);            //物理盘块号
        int offsetInBlock = p_file->f_offset % DISK_BLOCK_SIZE; //块内偏移
        //NOTE:可能要先读后写！！！
        //当写不满一个盘块的时候，就要先读后写
        if (offsetInBlock == 0 && length - writeByteCount >= DISK_BLOCK_SIZE)
        {

            //这种情况不需要先读后写
            pBuf = Kernel::instance()->getBufferCache().GetBlk(phyBlkno);
        }
        else
        {
            //先读后写
            pBuf = Kernel::instance()->getBufferCache().Bread(phyBlkno);
        }

        u_int8_t *p_buf_byte = (u_int8_t *)pBuf->b_addr;
        p_buf_byte += offsetInBlock;
        if (length - writeByteCount <= DISK_BLOCK_SIZE - offsetInBlock + 1)
        { //要读大小<=当前盘块剩下的,读需要的大小

            memcpy(p_buf_byte, content, length - writeByteCount);
            p_file->f_offset += length - writeByteCount;
            writeByteCount = length;
            //修改offset
        }
        else
        { //把剩下的全部读出来
            memcpy(p_buf_byte, content, DISK_BLOCK_SIZE - offsetInBlock + 1);
            p_file->f_offset += DISK_BLOCK_SIZE - offsetInBlock + 1;
            writeByteCount += DISK_BLOCK_SIZE - offsetInBlock + 1;

            //修改offset
        }
        Kernel::instance()->getBufferCache().Bdwrite(pBuf);
    }

    return writeByteCount;
}

/**
 * 判断是否到达文件尾部
 */
bool VFS::eof(FileFd fd)
{
    User &u = VirtualProcess::Instance()->getUser();
    File *p_file = u.u_ofiles.GetF(fd);
    Inode *p_inode = inodeCache->getInodeByID(p_file->f_inode_id); //TODO错误处理?
    if (p_file->f_offset == p_inode->i_size + 1)
        return true;
    else
        return false;
}

void VFS::registerExt2(Ext2 *p_ext2)
{
    this->p_ext2 = p_ext2;
}
void VFS::unregisterExt2()
{
}

void VFS::bindSuperBlockCache(SuperBlockCache *superBlockCache)
{
    this->superBlockCache = superBlockCache;
}
void VFS::bindInodeCache(InodeCache *inodeCache)
{
    this->inodeCache = inodeCache;
}
void VFS::bindDirectoryInodeCache(DirectoryCache *directoryCache)
{
    this->directoryCache = directoryCache;
}

bool VFS::isMounted()
{
    return Mounted;
}

// Path VFS::convertPathToAbsolute(Path &path){
//     if(path.from_root){
//         return path;
//     }else{

//     }
// }
