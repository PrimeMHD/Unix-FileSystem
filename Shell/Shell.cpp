#include "../include/Shell.h"

void Shell::help()
{

    system("cat help");
}

int Shell::readUserInput()
{

    while (true)
    {
        //Step0:
        //显示命令提示符
        //TODO

        //Step1:获取用户输入放到缓冲区

        std::cin.getline(tty_buffer, MAX_CMD_LEN, '\n');

        //Step2:先将tab转换为space
        for (char *checker = strrchr(tty_buffer, '\t'); checker != NULL; checker = strrchr(checker, '\t'))
        {
            *checker = ' ';
        }

        //Step3:以空格、tab为界，分解命令参数，存到Shell::split_cmd中
        char *dupl_tty_buffer = strdup(tty_buffer);
        /**
         * NOTE strdup创建的字符串是在堆上的，需要自己delete释放
         *@comment:这里拷贝一份tty_buffer的副本，因为后面用strtok函数的时候，会改变参数的字符串
         *当然也不是非要调用strtok，但是方便啊
         * 
         */

        //splitCmd先清空一下
        memset(split_cmd, 0x0, sizeof(split_cmd));
        int cmd_param_seq = 0;
        for (char *p = strtok(dupl_tty_buffer, " "); p != nullptr; p = strtok(NULL, " "), cmd_param_seq++)
        {
            strcpy(split_cmd[cmd_param_seq], p);
        }
        param_num = cmd_param_seq;
#ifdef IS_DEBUG
        for (int i = 0; i < param_num; i++)
        {
            std::cout << "看一下刚输入的参数：" << split_cmd[i] << ' ';
        }
        std::cout << std::endl;
#endif
        //TODO

        //Step4:解析执行指令
        parseCmd();
        delete dupl_tty_buffer;
    }
}

void Shell::parseCmd()
{
    switch (getInstType())
    {
    case MOUNT:
        mount();
        break;
    case UNMOUNT:
        unmount();
        break;
    case FORMAT:
        format();
        break;
    case CD:
        // cd();
        break;
    case LS:
        // ls();
        break;
    case RM:
        // rm();
        break;
    case MKDIR:
        mkdir();
        break;
    case TOUCH:
        touch();
        break;
    case CLEAR:
        //clear();
        break;
    case HELP:
        help();
        break;
    case EXIT:
        mexit();
        break;
    case VERSION:
        version();
        break;
    default:
        printf("CMD NOT SUPPORTED!\n");
        break;
    }
}

/**
 * @comment:实际上是做字符串到枚举类型的转化，为了switch case
 */
INSTRUCT Shell::getInstType()
{
    char *instStr = getInstStr();
#ifdef IS_DEBUG
    Logcat::log(TAG, "命令行命令字为:");
    Logcat::log(TAG, instStr);

#endif
    //为什么从1开始
    for (int i = 1; i < INST_NUM; i++)
    {
        //这里要加感叹号，注意strcmp在相等时返回的是0
        if (!strcmp(instructStr[i], instStr))
        {

#ifdef IS_DEBUG
            //std::cout<<INSTRUCT(i)<<std::endl;
#endif
            return INSTRUCT(i - 1);
        }
    }
    return ERROR_INST;
}

/**
 * @comment:命令缓冲区→命令参数字符数组→第一个参数得到命令字符串
 * 此函数的功能就是读出第一个字符串，亦即InstStr
 */
char *Shell::getInstStr()
{
    return split_cmd[0];
    //很简单，数组首个就是命令关键字
}

/**
 * @comment:这个是getInstStr更通用的情况
 */
char *Shell::getParam(int i)
{
    return split_cmd[i];
}

void Shell::mount()
{
    Logcat::log(TAG, "MOUNT EXEC");
    /**
     * 装载磁盘的最上层命令调用函数：
     * 硬盘装载的步骤：
     * ①内存inodeCache初始化
     * ②DiskDriver打开虚拟磁盘img，mmap，进入就绪状态
     * ③装载SuperBlock到VFS的SuperBlock缓存
     * 
     *  */
    bounded_VFS->mount();
}

void Shell::unmount()
{
    Logcat::log(TAG, "unmount EXEC");
}

/**
 * 对装载的磁盘镜像做格式化
 */
void Shell::format()
{

    if (bounded_VFS->isMounted())
    {
        bounded_VFS->format();
        Logcat::log(TAG, "format EXEC");
    }
    else
    {
        Logcat::log(TAG, "ERROR,DISK NOT MOUNTED!");
    }
}
void Shell::mkdir()
{
    Logcat::log(TAG, "mkdir EXEC");
}
void Shell::cat()
{
    Logcat::log(TAG, "cat EXEC");
}
void Shell::touch()
{
    Logcat::log(TAG, "touch EXEC");
}

void Shell::version()
{
    Logcat::log(TAG, "version EXEC");
}
void Shell::man()
{
    Logcat::log(TAG, "man EXEC");
}
void Shell::mexit()
{
    Logcat::log(TAG, "exit EXEC");
}
//隐式调用
void Shell::creat()
{
    Logcat::log(TAG, "creat EXEC");
}
void Shell::open()
{
    Logcat::log(TAG, "open EXEC");
}
void Shell::close()
{
    Logcat::log(TAG, "close EXEC");
}
void Shell::read()
{
    Logcat::log(TAG, "read EXEC");
}
void Shell::write()
{
    Logcat::log(TAG, "write EXEC");
}
void Shell::lseek()
{
    Logcat::log(TAG, "lseek EXEC");
}
Shell::Shell()
{
    TAG = strdup("Shell");
}
Shell::~Shell()
{
    delete TAG;
}

void Shell::setVFS(VFS *vfs)
{
    bounded_VFS = vfs;
}
