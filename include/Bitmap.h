#ifndef BITMAP_H
#define BITMAP_H
#include "define.h"
/**
 * BitMap标记的是盘块的使用情况，不是字节的。
 * //修正上面的，将Bitmap改造为更加通用的，在inode区管理也可以用用
 * 如果是盘块Bitmap，则构造Bitmap(DISK_SIZE/DISK_BLOCK_SIZE/BITMAP_PERBLOCK_SIZE)
 */
class Bitmap
{
private:
  uint8_t bitmap[MAX_BITMAP_ELEM_NUM / BITMAP_PERBLOCK_SIZE + 1]{0};
  int bitmapSize;
  //按位操作，又快又省空间
public:
  Bitmap(int elemNum);
  ~Bitmap();
  int setBit(int elemID);
  int unsetBit(int elemID);
  bool getBitStat(int elemID);
  int getAFreeBitNum();
  void clear();
  int getElemNum();
};

#endif