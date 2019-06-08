#ifndef DIRECTORY_CACHE_H
#define DIRECTORY_CACHE_H
#include "define.h"
#include "DirectoryEntry.h"
#include "Bitmap.h"
#include "Path.h"

/**
 * 缓存最近使用过的目录项，减少访问磁盘的次数
 */

class DirectoryCache
{

private:
  DirectoryEntry directoryEntryCacheArea[DIRECTORY_ENTRY_CACHE_SIZE];
  Bitmap directoryEntryCacheBitmap;

public:
  DirectoryCache();
  InodeId findInodeIdByPath(Path path); //根据filepath来查找，如果有的话，返回该目录的inode号(若没有返回-1)
};

#endif