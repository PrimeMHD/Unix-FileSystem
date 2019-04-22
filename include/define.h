#ifndef OS_MAIN_DEFINE  //确保不重复编译
#define OS_MAIN_DEFINE


#include <iostream>  //大家都懂
#include <cstring>   //用到str函数

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

static const char *instructStr[]{
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