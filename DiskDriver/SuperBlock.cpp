#include "../include/SuperBlock.h"

SuperBlock::SuperBlock() : disk_block_bitmap(DISK_SIZE / DISK_BLOCK_SIZE)
{
}