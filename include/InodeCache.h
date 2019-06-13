#ifndef INODE_CACHE_H
#define INODE_CACHE_H

#include "Inode.h"
#include "Bitmap.h"
#include "DiskInode.h"

/**
 * 内存inode缓存区域.
 * 可选的inode缓存区域的结构：
 * ①链表
 * ②动态数组
 * ③静态数组+bitmap
 * 我选择③；
 * 注意，刷回磁盘的操作没有放在这里，因为需要外层协调。
 * 刷回磁盘需要做的一些工作，可以在这里完成。
 * //TEMP 目前反正inodeCache也不多，卸载刷回的时候，不管脏不脏都刷回
 */
class InodeCache
{

  //TODO
private:
  Inode inodeCacheArea[INODE_CACHE_SIZE];
  Bitmap inodeCacheBitmap;

public:
  InodeCache() : inodeCacheBitmap(INODE_CACHE_SIZE) {}
  void clearCache();
  Inode *getInodeByID(int inodeID); //返回inodeCache块的缓存
  int addInodeCache(DiskInode inode, InodeId inodeId);
  int freeInodeCache(int inodeID);
  void replaceInodeCache(DiskInode inode, int replacedInodeID);
  int flushAllCacheDirtyInode();
};

#endif