#ifndef KERNEL_H
#define KERNEL_H
#include "define.h"
#include "VFS.h"
#include "Ext2.h"
#include "DiskDriver.h"
#include "BufferCache.h"

class Kernel
{
  private:
    static Kernel kernelInstance;
    VFS vfs;
    Ext2 ext2;
    DiskDriver diskDriver;
    BufferCache bufferCache;

  public:
    Kernel();
    static Kernel &instance();
    VFS &getVFS();
    Ext2 &getExt2();
    DiskDriver &getDiskDriver();
    BufferCache &getBufferCache();
};

#endif