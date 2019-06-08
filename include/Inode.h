#ifndef INODE_H
#define INODE_H
#include "define.h"
#include "DiskBlock.h"
//#include "DiskInode.h"

class DiskInode;

/**
 * 内存Inode结构，一个内存Inode占76B
 * 
 */
// const unsigned int INODE_LOCKED = 0x00000001;
// const unsigned int INODE_DIRTY = 0x00000002;
// const unsigned int INODE_INIT = 0x00000000;
class Inode
{
private:
  //NONE
public:
  enum INodeFlag
  {
    ILOCK = 0x1,  /* 索引节点上锁 */
    IUPD = 0x2,   /* 内存inode被修改过，需要更新相应外存inode */
    IACC = 0x4,   /* 内存inode被访问过，需要修改最近一次访问时间 */
    IMOUNT = 0x8, /* 内存inode用于挂载子文件系统 */
    IWANT = 0x10, /* 有进程正在等待该内存inode被解锁，清ILOCK标志时，要唤醒这种进程 */
    ITEXT = 0x20  /* 内存inode对应进程图像的正文段 */
  };
  unsigned int i_mode; //文件工作方式信息,通过&操作可以知道文件是属于什么文件类型
  int i_nlink;         //文件联结计数，即该文件在目录树中不同路径名的数量
  short i_uid;
  short i_gid;
  int i_size;     //文件大小
  int i_addr[10]; //混合索引表

  unsigned int i_flag; //状态的标志位，定义见enum INodeFlag
  int i_count;         //引用计数
  short i_dev;
  int i_number; //文件的inode号
  int i_lastr;  //存放最近一次读取文件的逻辑块号，用于判断是否需要预读

  //DiskBlock *d_addr[10];  这样有点不地道，因为真实的磁盘必须要根据块号寻址
  static const unsigned int IALLOC = 0x8000; /* 文件被使用 */
  static const int ADDRESS_PER_INDEX_BLOCK = DISK_BLOCK_SIZE / sizeof(int);
  static const int SMALL_FILE_BLOCK = 6;                                                                                      /* 小型文件：直接索引表最多可寻址的逻辑块号 */
  static const int LARGE_FILE_BLOCK = ADDRESS_PER_INDEX_BLOCK * 2 + 6;                                                        /* 大型文件：经一次间接索引表最多可寻址的逻辑块号 */
  static const int HUGE_FILE_BLOCK = ADDRESS_PER_INDEX_BLOCK * ADDRESS_PER_INDEX_BLOCK * 2 + ADDRESS_PER_INDEX_BLOCK * 2 + 6; /* 巨型文件：经二次间接索引最大可寻址文件逻辑块号 */
  static const unsigned int IFMT = 0x6000;                                                                                    /*文件类型掩码*/
  static const unsigned int IFDIR = 0x4000;                                                                                   //文件类型：目录文件
  static const unsigned int IFCHR = 0x2000;                                                                                   //文件类型：字符设备
  static const unsigned int IFBLK = 0x6000;                                                                                   //块设备特殊类型文件，为0表示常规数据文件

  Inode();                  //构造函数
  Inode(DiskInode d_inode); //转换构造函数
  int Bmap(int lbn);        //根据逻辑块号查混合索引表，得到物理块号。
};

#endif