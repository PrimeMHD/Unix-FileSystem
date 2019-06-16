#ifndef SHELL_H
#define SHELL_H
#include "define.h"
#include "Logcat.h"
#include "Path.h"
#include "VFS.h"

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
  int param_num = 0;
  char const *TAG;
  VFS *bounded_VFS;

public:
  Shell();
  ~Shell();
  int readUserInput();
  void parseCmd();
  INSTRUCT getInstType();
  char *getInstStr();
  char *getParam(int i);
  int getParamAmount();
  void setVFS(VFS *vfs);

  //shell调用的功能
  //显式调用
  void mount();   //挂载磁盘
  void unmount(); //卸载磁盘
  void format();
  void mkdir();
  void cat();
  void touch();
  void rm();
  void rmdir();
  void help();
  void version();
  void man();
  void mexit();
  void ls();
  void cd();
  void store();
  void withdraw();
  void clear();
  //隐式调用
  //void creat();
  //void open();
  //void close();
  //void read();
  //void write();
  //void lseek();
  //下面这几个func不应该是文件系统的东西，因为它们粗暴的连接了外部真实文件系统。
  //void writeExternalFileIntoIMG(const char *outsideFilePath);
  //void readIMGFileToExtern(const char *internalFilePath); //根据文件的类型
};
#endif