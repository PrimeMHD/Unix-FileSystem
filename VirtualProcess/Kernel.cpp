#include "../include/Kernel.h"

Kernel Kernel::kernelInstance;

Kernel::Kernel()
{
#ifdef IS_DEBUG
    std::cout << "Construct Kernel" << std::endl;
#endif
    vfs.registerExt2(&ext2);
    vfs.bindDirectoryInodeCache(&directoryCache);
    vfs.bindInodeCache(&inodeCache);
    vfs.bindSuperBlockCache(&superBlockCache);
    ext2.setBufferCache(&bufferCache);
    bufferCache.setDiskDriver(&diskDriver);
}
Kernel *Kernel::instance()
{

    return &Kernel::kernelInstance;
}
VFS &Kernel::getVFS()
{
    return vfs;
}
Ext2 &Kernel::getExt2()
{
    return ext2;
}
DiskDriver &Kernel::getDiskDriver()
{
    return diskDriver;
}
BufferCache &Kernel::getBufferCache()
{
    return bufferCache;
}

SuperBlockCache &Kernel::getSuperBlockCache()
{
    return superBlockCache;
}

InodeCache &Kernel::getInodeCache()
{
    return inodeCache;
}
