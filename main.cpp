#include "include/define.h"
#include "include/Shell.h"
#include "include/VFS.h"
#include "include/Ext2.h"
#include "include/DiskDriver.h"
#include "include/BufferCache.h"
#include "include/Kernel.h"

int main()
{
   int i;
   Shell shell;

   //这些模块是依赖关系，不是组合关系。

   shell.setVFS(&Kernel::instance()->getVFS());

   shell.readUserInput();

   return 0;
}