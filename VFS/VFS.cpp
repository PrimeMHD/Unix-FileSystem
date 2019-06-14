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
int VFS::mkDir(const char *dirName)
{
    int newDirInodeId=createFile(dirName);
    Inode* p_inode=inodeCache->getInodeByID(newDirInodeId);
    p_inode->i_mode=Inode::IFDIR;

    DirectoryEntry tempDirectoryEntry;
    Buf *pBuf;

    BlkNum blkno=p_inode->Bmap(0);
    pBuf = Kernel::instance()->getBufferCache().Bread(blkno);
    DirectoryEntry *p_directoryEntry=(DirectoryEntry *)pBuf->b_addr;

    strcpy(tempDirectoryEntry.m_name,".");
    tempDirectoryEntry.m_ino=newDirInodeId;
    *p_directoryEntry=tempDirectoryEntry;
    p_directoryEntry++;
    strcpy(tempDirectoryEntry.m_name,"..");
    tempDirectoryEntry.m_ino=VirtualProcess::Instance()->getUser().curDirInodeId;
    *p_directoryEntry=tempDirectoryEntry;
    Kernel::instance()->getBufferCache().Bdwrite(pBuf);
    return OK;
}
int VFS::cd(const char *dirName)
{
    Path path(dirName);
    VirtualProcess::Instance()->getUser().curDirInodeId = p_ext2->locateInode(path);
    //df
    return OK;
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

    ls(dirInodeId);
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
        return ERROR_OPEN_ILLEGAL;
    }
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
int VFS::close(int fd)
{
    return OK;
}
int VFS::read(int fd, u_int8_t *content, int length)
{
    return OK;
}
int VFS::write(int fd, u_int8_t *content, int length)
{
    return OK;
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
