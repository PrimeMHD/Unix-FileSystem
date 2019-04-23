#ifndef OS_MAIN_DEFINE  //确保不重复编译
#define OS_MAIN_DEFINE
#define IS_DEBUG   //调试状态

#include <iostream>  //大家都懂
#include <cstring>   //用到str函数
#include <string> //沈坚不让用的String类
#include <stdint.h>

//const和define的区别》
#define DISK_BLOCK_SIZE 4096
#define DISK_IMG_FILEPATH "../disk.img"
enum INSTRUCT {
    ERROR_INST = -1,
    MOUNT,
    UNMOUNT,
    FORMAT,
    CD,
    LS,
    RM,
    MKDIR,
    TOUCH,
    CLEAR,
    HELP,
    EXIT,
    VERSION
};
const int INST_NUM=13;
//NOTE 注意，如果改了上面的枚举类型，那么下面的这个数字也需要相应修改

static const char *instructStr[]{
    "error",  //实际上会从下表1开始查找，这个"error"是为了占位置
    "mount",
    "unmount",
    "format",
    "cd",
    "ls",
    "rm",
    "mkdir",
    "touch",
    "clear",
    "help",
    "exit",
    "version"
};


#endif