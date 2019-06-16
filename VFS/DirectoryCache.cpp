#include "../include/DirectoryCache.h"
DirectoryCache::DirectoryCache() : directoryEntryCacheBitmap(DIRECTORY_ENTRY_CACHE_SIZE)
{
}

/**
 * 根据filepath来查找，如果有的话，返回该目录的inode号(若没有返回-1)
 */
InodeId DirectoryCache::findInodeIdByPath(Path path)
{
    return -1;
}