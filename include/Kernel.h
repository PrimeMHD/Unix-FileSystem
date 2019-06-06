#ifndef KERNEL_H
#define KERNEL_H
#include "define.h"
#include "VFS.h"
#include "Ext2.h"
#include "DiskDriver.h"
#include "BufferCache.h"
#include "DirectoryCache.h"

class Kernel
{
private:
  Kernel();
  static Kernel kernelInstance;
  VFS vfs;
  Ext2 ext2;
  DiskDriver diskDriver;
  BufferCache bufferCache;

  InodeCache inodeCache;
  DirectoryCache directoryCache;
  SuperBlock superBlockCache;

public:
  static Kernel *instance();
  VFS &getVFS();
  Ext2 &getExt2();
  DiskDriver &getDiskDriver();
  BufferCache &getBufferCache();
};

#endif