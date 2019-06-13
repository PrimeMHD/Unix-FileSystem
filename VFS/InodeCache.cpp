#include "../include/InodeCache.h"
#include "../include/Kernel.h"
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
    if (inodeCacheBitmap.getBitStat(i) && inodeCacheArea[i].i_number == inodeID) //首先需要这个inodeCache是有效的
    {
      return &inodeCacheArea[i];
    }
  }

  //没有在inodeCache中找到，需要从ext要，写入inodeCache
  return &inodeCacheArea[addInodeCache((Kernel::instance()->getExt2().getDiskInodeByNum(inodeID)), inodeID)];

} //返回inodeCache块的缓存

/**
 * 将磁盘Inode拷贝到InodeCache中，注意DiskInode和内存Inode数据结构的区别
 * 若InodeCache未满，则直接放入
 * 否则,发生替换.
 * addInodeCache返回值的含义：放入位置的下标
 * 
 */
int InodeCache::addInodeCache(DiskInode inode, InodeId inodeId)
{
  int pos = inodeCacheBitmap.getAFreeBitNum();
  if (pos < 0)
  {
    //空间不够，需要替换某个inode
    //替换哪个呢？暂时采取随机替换的策略。TODO
    srand((unsigned)time(NULL));
    int ramdom_i = (rand() % (INODE_CACHE_SIZE - 10)) + 10;
    //①确定替换的下标。保留前几个inode不替换，因为比较常用

    if ((inodeCacheArea[ramdom_i].i_flag & (Inode::IUPD | Inode::IACC)) != 0)
    {
      Kernel::instance()->getExt2().updateDiskInode(inodeCacheArea[ramdom_i].i_number, inodeCacheArea[ramdom_i]);
    }
    //②可能发生脏inode写回

    inodeCacheArea[pos] = Inode(inode);
    inodeCacheArea[pos].i_number = inodeId;
    inodeCacheBitmap.setBit(pos);
    //③用新的inode覆盖掉

    pos = ramdom_i;
  }
  else
  {

    inodeCacheArea[pos] = Inode(inode);
    inodeCacheArea[pos].i_number = inodeId;
    inodeCacheBitmap.setBit(pos);
  }
  return pos;
}

/**
 * 修改指定inodeID的缓存
 */
void InodeCache::replaceInodeCache(DiskInode inode, int replacedInodeID)
{
  //TODO 暂时好像没有需要
}

//TODO 这个函数的动机存疑。
/**
 * 释放某指定inodeID的缓存.
 * 返回值：若为-1表示没找到要释放的
 * 否则，返回值表示被释放的inodeCache的数组下标
 */
int InodeCache::freeInodeCache(int inodeID)
{
  //首先要找到这个inodeID的缓存放在哪个位置
  //然后修改bitmap
  for (int i = 0; i < INODE_CACHE_SIZE; i++)
  {
    if (inodeCacheBitmap.getBitStat(i) && inodeCacheArea[i].i_number == inodeID) //首先需要这个inodeCache是有效的
    {
      inodeCacheBitmap.unsetBit(i);
      return i;
    }
  }

  return -1; //表示没有找到要释放的。
}

/**
 * 刷回所有的Inode缓存回磁盘（可能所谓刷回只是留在缓存中，
 * 所以如果是卸载磁盘的话，刷回缓存是最后做的）
 */
int InodeCache::flushAllCacheDirtyInode()
{
  //遍历inodeCache，查找存在的并且是脏的inode
  for (int i = 0; i < inodeCacheBitmap.getElemNum(); i++)
  {
    if (inodeCacheBitmap.getBitStat(i))
    { //该inode缓存有意义
      if (inodeCacheArea[i].i_flag & (Inode::IUPD | Inode::IACC))
      {
        Kernel::instance()->getExt2().updateDiskInode(inodeCacheArea[i].i_number, inodeCacheArea[i]);
      }
    }
  }
  return OK;
}
