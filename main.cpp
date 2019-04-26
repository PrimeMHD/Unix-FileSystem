#include "include/define.h"
#include "include/Shell.h"
#include "include/VFS.h"
#include "include/Ext2.h"
#include "include/DiskDriver.h"
#include "include/BufferCache.h"



int main(){
   int i;
   Shell shell;
   VFS vfs;
   Ext2 ext2;
   DiskDriver diskDriver;
   BufferCache bufferCache;
   //这些模块是依赖关系，不是组合关系。

   shell.setVFS(&vfs);
   vfs.registerExt2(&ext2);
   ext2.setBufferCache(&bufferCache);
   bufferCache.setDiskDriver(&diskDriver);


   shell.readUserInput();

   return 0;

}