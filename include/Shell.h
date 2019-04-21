#include "../define.h"

#define TTY_BUFFER_SIZE 512

class Shell
{
    //处理用户输入，解析指令，调用相关的内核函数
  private:
    char tty_buffer[TTY_BUFFER_SIZE];  

  public:
    int readUserInput();
    
    //shell调用的功能
      //显式调用
    void mount();//挂载磁盘
    void unmount();//卸载磁盘
    void format();
    void mkdir();
    void cat();
    void touch();
    void help();
      //隐式调用
    void creat();
    void open();
    void close();
    void read();
    void write();
    void lseek();
      
}