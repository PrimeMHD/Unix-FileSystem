#ifndef DIRECTORY_CACHE_H
#define DIRECTORY_CACHE_H
#include "define.h"
#include "DirectoryEntry.h"
#include "Bitmap.h"

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
};

#endif