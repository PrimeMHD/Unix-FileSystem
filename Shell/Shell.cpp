#include "Shell.h"

void Shell::help(){
    system("cat help");
}


int Shell::readUserInput(){
    //Step1:获取用户输入放到缓冲区
    std::cin.getline(tty_buffer,MAX_CMD_LEN,'\n');

    //Step2:以空格、tab为界，分解命令参数，存到Shell::split_cmd中
    //TODO


}

int Shell::parseCmd(){
    switch (getInstType())
    {
        case MOUNT:
            /* code */
            break;
        case UNMOUNT:
            break;
        case FORMAT:
            break;
        case CD:
            break;
        case LS:
            break;
        case RM:
            break;
        case MKDIR:
            break;
        case TOUCH:
            break;
        case CLEAR:
            break;
        case HELP:
            break;
        case EXIT:
            break;
        case VERSION:
            break;
        default:
            break;
    }
}


/**
 * comment:实际上是做字符串到枚举类型的转化，为了switch case
 */
INSTRUCT Shell::getInstType(){

}