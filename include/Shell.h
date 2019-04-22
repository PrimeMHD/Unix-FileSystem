#ifndef SHELL_H
#define SHELL_H
#include "define.h"

#define TTY_BUFFER_SIZE 4096
#define MAX_CMD_LEN 4096
#define MAX_PARAM_NUM 32
#define MAX_SINGLE_PARAM_LEN 128 

class Shell
{
    //处理用户输入，解析指令，调用相关的内核函数
  private:
    char tty_buffer[TTY_BUFFER_SIZE];  
    char split_cmd[MAX_PARAM_NUM][MAX_SINGLE_PARAM_LEN]{};
  public:
    int readUserInput();
    int parseCmd();
    INSTRUCT getInstType();
    //shell调用的功能
      //显式调用
    void mount();//挂载磁盘
    void unmount();//卸载磁盘
    void format();
    void mkdir();
    void cat();
    void touch();
    void help();
    void version();
    void man();
    void exit();
      //隐式调用
    void creat();
    void open();
    void close();
    void read();
    void write();
    void lseek();
      
};
#endif