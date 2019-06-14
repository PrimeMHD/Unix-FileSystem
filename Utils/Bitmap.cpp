#include "../include/Bitmap.h"
#include <cmath>

Bitmap::~Bitmap()
{
    // if(bitmap!=nullptr){
    //     delete bitmap;
    // }
}

Bitmap::Bitmap(int elemNum)
{
    //bitmap=new uint8_t[elemNum/BITMAP_PERBLOCK_SIZE+1]{0};
    bitmapSize = elemNum;
    //   memcpy(bitmap,0,DISK_SIZE/DISK_BLOCK_SIZE/8);
}

//NOTE 注意，盘块号是从0开始的。但是盘块数量是从1开始的
int Bitmap::setBit(int blockID)
{

    if (blockID > bitmapSize - 1 || blockID < 0)
        return ERROR_OFR; //范围非法

    int elemPos = blockID / BITMAP_PERBLOCK_SIZE;
    int innerPos = blockID % BITMAP_PERBLOCK_SIZE;
    //NOTE 显然这里也可以通过循环位移得到对应的掩码，但是不比下面的switch case快
    switch (innerPos)
    {
    case 0:
        bitmap[elemPos] |= 0x01;
        break;
    case 1:
        bitmap[elemPos] |= 0x02;
        break;
    case 2:
        bitmap[elemPos] |= 0x04;
        break;
    case 3:
        bitmap[elemPos] |= 0x08;
        break;
    case 4:
        bitmap[elemPos] |= 0x10;
        break;
    case 5:
        bitmap[elemPos] |= 0x20;
        break;
    case 6:
        bitmap[elemPos] |= 0x40;
        break;
    case 7:
        bitmap[elemPos] |= 0x80;
        break;
    default:
        //cannot be reached!
        break;
    }

    return OK;
}

int Bitmap::unsetBit(int blockID)
{
    if (blockID > bitmapSize - 1 || blockID < 0)
        return ERROR_OFR; //范围非法
    int elemPos = blockID / BITMAP_PERBLOCK_SIZE;
    int innerPos = blockID % BITMAP_PERBLOCK_SIZE;
    switch (innerPos)
    {
    case 0:
        bitmap[elemPos] &= 0xfe;
        break;
    case 1:
        bitmap[elemPos] &= 0xfd;
        break;
    case 2:
        bitmap[elemPos] &= 0xfb;
        break;
    case 3:
        bitmap[elemPos] &= 0xf7;
        break;
    case 4:
        bitmap[elemPos] &= 0xef;
        break;
    case 5:
        bitmap[elemPos] &= 0xdf;
        break;
    case 6:
        bitmap[elemPos] &= 0xbf;
        break;
    case 7:
        bitmap[elemPos] &= 0x7f;
        break;
    default:
        //cannot be reached!
        break;
    }
    return OK;
}
bool Bitmap::getBitStat(int blockID)
{
    if (blockID > bitmapSize - 1 || blockID < 0)
        return false; //范围非法
    int elemPos = blockID / BITMAP_PERBLOCK_SIZE;
    int innerPos = blockID % BITMAP_PERBLOCK_SIZE;
    bool ret = false;
    switch (innerPos)
    {
    case 0:
        ret = (bitmap[elemPos] & 0x01) ;
        break;
    case 1:
        ret = (bitmap[elemPos] & 0x02) ;
        break;
    case 2:
        ret = (bitmap[elemPos] & 0x04) ;
        break;
    case 3:
        ret = (bitmap[elemPos] & 0x08) ;
        break;
    case 4:
        ret = (bitmap[elemPos] & 0x10) ;
        break;
    case 5:
        ret = (bitmap[elemPos] & 0x20);
        break;
    case 6:
        ret = (bitmap[elemPos] & 0x40);
        break;
    case 7:
        ret = (bitmap[elemPos] & 0x80);
        break;
    default:
        //cannot be reached!
        break;
    }
    return ret;
}

/**
 * NOTE 特别注意，这里getAFreeBitNum之后并不会将这个比特位set1.
 * 如果需要获用的话，在getAFreeBitNum之后，要手动setBit
 * 
 */
int Bitmap::getAFreeBitNum()
{
    int ret = -1;
    for (int i = 0; i < ceil(bitmapSize / BITMAP_PERBLOCK_SIZE); i++)
    {
        if (bitmap[i] != 0xFF)
        { //存在空位
            if (!(bitmap[i] & 0x01)) 
            {
                ret = i * BITMAP_PERBLOCK_SIZE + 0;
                break;
            }
            else if (!(bitmap[i] & 0x02))
            {
                ret = i * BITMAP_PERBLOCK_SIZE + 1;
                break;
            }
            else if (!(bitmap[i] & 0x04))
            {
                ret = i * BITMAP_PERBLOCK_SIZE + 2;
                break;
            }
            else if (!(bitmap[i] & 0x08))
            {
                ret = i * BITMAP_PERBLOCK_SIZE + 3;
                break;
            }
            else if (!(bitmap[i] & 0x10))
            {
                ret = i * BITMAP_PERBLOCK_SIZE + 4;
                break;
            }
            else if (!(bitmap[i] & 0x20))
            {
                ret = i * BITMAP_PERBLOCK_SIZE + 5;
                break;
            }
            else if (!(bitmap[i] & 0x40))
            {
                ret = i * BITMAP_PERBLOCK_SIZE + 6;
                break;
            }
            else if (!(bitmap[i] & 0x80))
            {
                ret = i * BITMAP_PERBLOCK_SIZE + 7;
                break;
            }
            // else if (~(bitmap[i] & 0x80))
            // {
            //     ret = i * BITMAP_PERBLOCK_SIZE + 8;
            //     break;
            // }
        }
    }
    return ret;
}

void Bitmap::clear()
{
    memset(bitmap, 0, bitmapSize); //TODO 请确认这句话的正确性。
}
/**
 * 有多少个元素
 */
int Bitmap::getElemNum()
{
    return this->bitmapSize;
}