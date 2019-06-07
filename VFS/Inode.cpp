#include "../include/Inode.h"
#include "../include/Kernel.h"
Inode::Inode()
{
    // dirty = false;
    // inode_id = 0;
    // memset(i_addr, 0, 10 * sizeof(int));
}

/**
 * 转换构造函数,
 * 将磁盘inode结构转换为内存inode结构
 * NOTE:需要对i_number进行单独的赋值
 */
Inode::Inode(DiskInode d_inode)
{
    this->i_mode = d_inode.d_mode;
    this->i_nlink = d_inode.d_nlink;
    this->i_uid = d_inode.d_uid;
    this->i_gid = d_inode.d_gid;
    this->i_size = d_inode.d_size;
    memcpy(this->i_addr, d_inode.d_addr, sizeof(d_inode.d_addr));
    this->i_flag = 0;
    this->i_count = 0;
    this->i_dev = 0;
    //this->i_number = ? ;s  注意！DISKINODE是没有INODE号这个属性的，一个DISKINODE的号是固定的，可以根据其位置算出来
    this->i_lastr = -1;
}

/**
 * 根据混合索引表，用逻辑块号，查出物理盘块号.
 * NOTE 功能不止查。
 * TODO bmap暂时不做
 */
int Inode::Bmap(int lbn)
{

    int phyBlkno; /* 转换后的物理盘块号 */
    int *iTable;  /* 用于访问索引盘块中一次间接、两次间接索引表 */
    int index;
    return OK;

    //User &u = Kernel::Instance().GetUser();

    // /**
    //  * 超出支持的最大文件块数
    //  */
    // if (lbn >= Inode::HUGE_FILE_BLOCK)
    // {
    //     //u.u_error = User::EFBIG;
    //     return 0;
    // }

    // if (lbn < 6) /* 如果是小型文件，从基本索引表i_addr[0-5]中获得物理盘块号即可 */
    // {
    //     phyBlkno = this->i_addr[lbn];

    //     /*
    // 	 * 如果该逻辑块号还没有相应的物理盘块号与之对应，则分配一个物理块。
    // 	 * 这通常发生在对文件的写入，当写入位置超出文件大小，即对当前
    // 	 * 文件进行扩充写入，就需要分配额外的磁盘块，并为之建立逻辑块号
    // 	 * 与物理盘块号之间的映射。
    // 	 */
    //     if (phyBlkno == 0 && (pFirstBuf = fileSys.Alloc(this->i_dev)) != NULL)
    //     {
    //         /*
    // 		 * 因为后面很可能马上还要用到此处新分配的数据块，所以不急于立刻输出到
    // 		 * 磁盘上；而是将缓存标记为延迟写方式，这样可以减少系统的I/O操作。
    // 		 */
    //         bufMgr.Bdwrite(pFirstBuf);
    //         phyBlkno = pFirstBuf->b_blkno;
    //         /* 将逻辑块号lbn映射到物理盘块号phyBlkno */
    //         this->i_addr[lbn] = phyBlkno;
    //         this->i_flag |= Inode::IUPD;
    //     }
    //     /* 找到预读块对应的物理盘块号 */
    //     Inode::rablock = 0;
    //     if (lbn <= 4)
    //     {
    //         /*
    // 		 * i_addr[0] - i_addr[5]为直接索引表。如果预读块对应物理块号可以从
    // 		 * 直接索引表中获得，则记录在Inode::rablock中。如果需要额外的I/O开销
    // 		 * 读入间接索引块，就显得不太值得了。漂亮！
    // 		 */
    //         Inode::rablock = this->i_addr[lbn + 1];
    //     }

    //     return phyBlkno;
    // }
    // else /* lbn >= 6 大型、巨型文件 */
    // {
    //     /* 计算逻辑块号lbn对应i_addr[]中的索引 */

    //     if (lbn < Inode::LARGE_FILE_BLOCK) /* 大型文件: 长度介于7 - (128 * 2 + 6)个盘块之间 */
    //     {
    //         index = (lbn - Inode::SMALL_FILE_BLOCK) / Inode::ADDRESS_PER_INDEX_BLOCK + 6;
    //     }
    //     else /* 巨型文件: 长度介于263 - (128 * 128 * 2 + 128 * 2 + 6)个盘块之间 */
    //     {
    //         index = (lbn - Inode::LARGE_FILE_BLOCK) / (Inode::ADDRESS_PER_INDEX_BLOCK * Inode::ADDRESS_PER_INDEX_BLOCK) + 8;
    //     }

    //     phyBlkno = this->i_addr[index];
    //     /* 若该项为零，则表示不存在相应的间接索引表块 */
    //     if (0 == phyBlkno)
    //     {
    //         this->i_flag |= Inode::IUPD;
    //         /* 分配一空闲盘块存放间接索引表 */
    //         if ((pFirstBuf = fileSys.Alloc(this->i_dev)) == NULL)
    //         {
    //             return 0; /* 分配失败 */
    //         }
    //         /* i_addr[index]中记录间接索引表的物理盘块号 */
    //         this->i_addr[index] = pFirstBuf->b_blkno;
    //     }
    //     else
    //     {
    //         /* 读出存储间接索引表的字符块 */
    //         pFirstBuf = bufMgr.Bread(this->i_dev, phyBlkno);
    //     }
    //     /* 获取缓冲区首址 */
    //     iTable = (int *)pFirstBuf->b_addr;

    //     if (index >= 8) /* ASSERT: 8 <= index <= 9 */
    //     {
    //         /*
    // 		 * 对于巨型文件的情况，pFirstBuf中是二次间接索引表，
    // 		 * 还需根据逻辑块号，经由二次间接索引表找到一次间接索引表
    // 		 */
    //         index = ((lbn - Inode::LARGE_FILE_BLOCK) / Inode::ADDRESS_PER_INDEX_BLOCK) % Inode::ADDRESS_PER_INDEX_BLOCK;

    //         /* iTable指向缓存中的二次间接索引表。该项为零，不存在一次间接索引表 */
    //         phyBlkno = iTable[index];
    //         if (0 == phyBlkno)
    //         {
    //             if ((pSecondBuf = fileSys.Alloc(this->i_dev)) == NULL)
    //             {
    //                 /* 分配一次间接索引表磁盘块失败，释放缓存中的二次间接索引表，然后返回 */
    //                 bufMgr.Brelse(pFirstBuf);
    //                 return 0;
    //             }
    //             /* 将新分配的一次间接索引表磁盘块号，记入二次间接索引表相应项 */
    //             iTable[index] = pSecondBuf->b_blkno;
    //             /* 将更改后的二次间接索引表延迟写方式输出到磁盘 */
    //             bufMgr.Bdwrite(pFirstBuf);
    //         }
    //         else
    //         {
    //             /* 释放二次间接索引表占用的缓存，并读入一次间接索引表 */
    //             bufMgr.Brelse(pFirstBuf);
    //             pSecondBuf = bufMgr.Bread(this->i_dev, phyBlkno);
    //         }

    //         pFirstBuf = pSecondBuf;
    //         /* 令iTable指向一次间接索引表 */
    //         iTable = (int *)pSecondBuf->b_addr;
    //     }

    //     /* 计算逻辑块号lbn最终位于一次间接索引表中的表项序号index */

    //     if (lbn < Inode::LARGE_FILE_BLOCK)
    //     {
    //         index = (lbn - Inode::SMALL_FILE_BLOCK) % Inode::ADDRESS_PER_INDEX_BLOCK;
    //     }
    //     else
    //     {
    //         index = (lbn - Inode::LARGE_FILE_BLOCK) % Inode::ADDRESS_PER_INDEX_BLOCK;
    //     }

    //     if ((phyBlkno = iTable[index]) == 0 && (pSecondBuf = fileSys.Alloc(this->i_dev)) != NULL)
    //     {
    //         /* 将分配到的文件数据盘块号登记在一次间接索引表中 */
    //         phyBlkno = pSecondBuf->b_blkno;
    //         iTable[index] = phyBlkno;
    //         /* 将数据盘块、更改后的一次间接索引表用延迟写方式输出到磁盘 */
    //         bufMgr.Bdwrite(pSecondBuf);
    //         bufMgr.Bdwrite(pFirstBuf);
    //     }
    //     else
    //     {
    //         /* 释放一次间接索引表占用缓存 */
    //         bufMgr.Brelse(pFirstBuf);
    //     }
    //     /* 找到预读块对应的物理盘块号，如果获取预读块号需要额外的一次for间接索引块的IO，不合算，放弃 */
    //     Inode::rablock = 0;
    //     if (index + 1 < Inode::ADDRESS_PER_INDEX_BLOCK)
    //     {
    //         Inode::rablock = iTable[index + 1];
    //     }
    //     return phyBlkno;
    // }
} //根据逻辑块号查混合索引表，得到物理块号。
