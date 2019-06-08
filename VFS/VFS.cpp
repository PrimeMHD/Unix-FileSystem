#include "../include/VFS.h"
#include "../include/Logcat.h"
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
int VFS::createFile(const char *fileName)
{
    return OK;
}
int VFS::mkDir(const char *dirName)
{
    return OK;
}
int VFS::cd(const char *dirName)
{
    return OK;
}
void VFS::ls(int dirInodeID)
{
}


void VFS::ls(const char *dirName)
{
    //首先要根据目录名，确定inode号
    //step1 在DirectoryEntry中查找有没有现成的

}





int VFS::open(Path path)
{
    return OK;
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
