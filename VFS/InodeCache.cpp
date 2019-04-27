#include "../include/InodeCache.h"

void InodeCache::clearCache()
{
  /**
   * 很简单，不需要把InodeCache区覆盖什么的，
   * 只需要清空bitmap
   */
  inodeCacheBitmap.clear();
}

Inode *InodeCache::getInodeByID(int inodeID)
{
  /**
   * 工序：
   * 遍历查找InodeCache区域看有没有inodeID吻合的inode，若有则取出
   * 若没有，先向磁盘取这个inode，放入inodeCacheArea，（可能会出现替换）
   */
  for (int i = 0; i < INODE_CACHE_SIZE; i++)
  {
    if (inodeCacheBitmap.getBitStat(i) && inodeCacheArea[i].inode_id == inodeID) //首先需要这个inodeCache是有效的
    {
      return &inodeCacheArea[i];
    }
  }

  //没有在inodeCache中找到，

  return nullptr;

} //返回inodeCache块的缓存

/**
 * 将磁盘Inode拷贝到InodeCache中，注意DiskInode和内存Inode数据结构的区别
 * 若InodeCache未满，则直接放入
 * 否则，通知VFS进行替换.
 * addInodeCache返回值的含义：
 * 返回-1：加入cache成功
 * 其他数值：需要被替换的inode号。
 * VFS应对返回其他数值的方式：getInodeByID检查dirty标志，
 *                        如果为false，通过ext2获得DiskInode直接replaceInodeCache
 *                        如果为true,将这个InodeCache刷回，然后再replaceInodeCache
 */
int InodeCache::addInodeCache(DiskInode inode)
{
  int pos = inodeCacheBitmap.getAFreeBitNum();
  if (pos < 0)
  {
    //空间不够，需要替换某个inode
    //替换哪个呢？暂时采取随机替换的策略。TODO
    srand((unsigned)time(NULL));
    int ramdom_i = (rand() % (INODE_CACHE_SIZE - 10)) + 10;
    //保留前几个inode不替换，因为比较常用
    return ramdom_i;
  }
  else
  {
    //TODO  需要定义DiskInode到Inode的转换构造函数
    inodeCacheArea[pos] = Inode(inode);
    inodeCacheBitmap.setBit(pos);
  }
}

/**
 * 修改指定inodeID的缓存
 */
void InodeCache::replaceInodeCache(DiskInode inode, int replacedInodeID)
{
}

/**
 * 释放某指定inodeID的缓存
 */
void InodeCache::freeInodeCache(int inodeID)
{
  //首先要找到这个inodeID的缓存放在哪个位置
  //然后修改bitmap
}