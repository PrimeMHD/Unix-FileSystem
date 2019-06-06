#ifndef VFS_H
#define VFS_H
#include "define.h"
#include "Ext2.h"
#include "SuperBlockCache.h"
#include "InodeCache.h"
#include "DirectoryCache.h"
class VFS
{
private:
  bool Mounted = false;
  Ext2 *p_ext2;                     //绑定的Ext2操纵模块。
  SuperBlockCache *superBlockCache; //VFS中的内存超级块，来自磁盘超级块在装载的时候刷入，卸载的时候刷回。
  InodeCache *inodeCache;           //VFS中内存Inode缓存
  DirectoryCache *directoryCache;   //与vfs绑定的目录项缓存

public:
  ~VFS();
  VFS();

  void mount();
  void unmount();
  int format();
  int createFile(const char *fileName); //返回分配的Inode编号
  int mkDir(const char *dirName);       //返回分配的Inode编号
  int cd(const char *dirName);          //返回进入的dir的Inode

  void ls(int dirInodeID);
  int open(Path path);
  int close(int fd);
  int read(int fd, u_int8_t *content, int length);  //用户层面，文件必须先打开才可读
  int write(int fd, u_int8_t *content, int length); //用户层面，文件必须先打开才可写
  void registerExt2(Ext2 *ext2);                    //注册文件系统，载入SuperBlock
  void unregisterExt2();                            //注销加载的文件系统，要刷回脏inode和superblock
  void bindSuperBlockCache(SuperBlockCache *superBlockCache);
  void bindInodeCache(InodeCache *inodeCache);
  void bindDirectoryInodeCache(DirectoryCache *directoryCache);
  bool isMounted();
};

#endif