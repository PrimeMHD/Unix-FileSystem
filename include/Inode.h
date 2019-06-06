#ifndef INODE_H
#define INODE_H
#include "define.h"
#include "DiskBlock.h"
//#include "DiskInode.h"

class DiskInode;

/**
 * 内存Inode结构
 * 
 */
class Inode
{
private:
  //NONE
public:
  bool dirty;        //脏位
  int inode_id;      //inode号
  int uid;           //uid
  size_t file_size;  //文件大小
  FileType fileType; //文件类型
  int i_addr[10];    //混合索引表
  //DiskBlock *d_addr[10];  这样有点不地道，因为真实的磁盘必须要根据块号寻址
  static const int ADDRESS_PER_INDEX_BLOCK = DISK_BLOCK_SIZE / sizeof(int);
  static const int SMALL_FILE_BLOCK = 6;                                                                                      /* 小型文件：直接索引表最多可寻址的逻辑块号 */
  static const int LARGE_FILE_BLOCK = ADDRESS_PER_INDEX_BLOCK * 2 + 6;                                                        /* 大型文件：经一次间接索引表最多可寻址的逻辑块号 */
  static const int HUGE_FILE_BLOCK = ADDRESS_PER_INDEX_BLOCK * ADDRESS_PER_INDEX_BLOCK * 2 + ADDRESS_PER_INDEX_BLOCK * 2 + 6; /* 巨型文件：经二次间接索引最大可寻址文件逻辑块号 */

  Inode();                  //构造函数
  Inode(DiskInode d_inode); //转换构造函数
  int Bmap(int lbn);        //根据逻辑块号查混合索引表，得到物理块号。
};

#endif