#include "../define.h"

#define TTY_BUFFER_SIZE 512

class Shell
{
    //处理用户输入，解析指令，调用相关的内核函数
  private:
    char tty_buffer[TTY_BUFFER_SIZE];

  public:
    int readUserInput();
    
}