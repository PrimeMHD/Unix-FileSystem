#include "../include/Kernel.h"

Kernel::Kernel()
{
#ifdef IS_DEBUG
    std::cout << "Construct Kernel" << std::endl;
#endif
    vfs.registerExt2(&ext2);
    vfs.bindDirectoryInodeCache(&directoryCache);
    vfs.bindInodeCache(&inodeCache);
    vfs.bindSuperBlock(&superBlockCache);
    ext2.setBufferCache(&bufferCache);
    bufferCache.setDiskDriver(&diskDriver);
}
Kernel *Kernel::instance()
{
    static Kernel kernelInstance;
    return &kernelInstance;
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