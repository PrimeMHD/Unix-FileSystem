#include "../include/VFS.h"
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
    if (OK == ext2->registerFs())
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
void VFS::registerExt2(Ext2 *ext2)
{
}
void VFS::unregisterExt2()
{
}

void VFS::bindSuperBlock(SuperBlock *superblock)
{
    this->superBlock = superblock;
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
