#include "../include/Shell.h"
#include "../include/VirtualProcess.h"
#include "../include/Kernel.h"

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
        mount(); //OK
        break;
    case UNMOUNT:
        unmount(); //OK
        break;
    case FORMAT:
        format(); //OK
        break;
    case CD:
        cd(); //OK
        break;
    case LS:
        ls(); //OK
        break;
    case RM:
        // rm();
        break;
    case MKDIR:
        mkdir();
        break;
    case TOUCH:
        touch(); //OK
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
    case STORE:
        store();
        break;
    case WITHDRAW:
        withdraw();
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

/**
 * 获得参数的个数
 */
int Shell::getParamAmount()
{
    for (int i = 0; i < MAX_PARAM_NUM; i++)
    {
        if (!strcmp(split_cmd[i], ""))
        {
            return i;
        }
    }
    return MAX_PARAM_NUM;
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
    bounded_VFS->unmount();
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
    if (getParamAmount() == 2)
    {
        if (bounded_VFS->mkDir(getParam(1)) < 0)
        {
            Logcat::log("ERROR!存在同名目录，创建失败！");
        }
    }
    else
    {
        Logcat::log("ERROR！MKDIR参数个数错误！");
    }
    Logcat::devlog(TAG, "mkdir EXEC");
}
void Shell::cat()
{
    Logcat::devlog(TAG, "cat EXEC");
    Logcat::devlog(TAG, "cat 暂不支持");
}
void Shell::touch()
{
    if (getParamAmount() != 2)
    {
        Logcat::log("ERROR!参数个数错误！");
        return;
    }
    else
    {
        if (0 > bounded_VFS->createFile(getParam(1)))
        {
            Logcat::log("ERROR!存在同名文件，创建失败！");
        }
    }

    Logcat::log(TAG, "touch EXEC");
}

void Shell::version()
{
    system("cat version");
    Logcat::devlog(TAG, "version EXEC");
}
void Shell::man()
{
    Logcat::log(TAG, "man EXEC");
}
void Shell::mexit()
{
    if (bounded_VFS->isMounted())
    {
        bounded_VFS->unmount();
    }
    Logcat::devlog(TAG, "exit EXEC");
    Logcat::devlog("程序结束！");
    exit(OK);
}
//隐式调用
void Shell::creat()
{
    Logcat::devlog(TAG, "creat EXEC");
}

/**
 * 临时的，不应该是一个用户接口
 */
void Shell::open()
{
    Path path(getParam(1));
    bounded_VFS->open(path, File::FREAD);
    Logcat::log(TAG, "open EXEC");
}
/**
 * 临时的，不应该是一个用户接口
 */
void Shell::close()
{
    Logcat::log(TAG, "close EXEC");
}

/**
 * 临时的，不应该是一个用户接口
 */
void Shell::read()
{
    Logcat::log(TAG, "read EXEC");
}

/**
 * 临时的，不应该是一个用户接口
 */
void Shell::write()
{
    Logcat::log(TAG, "write EXEC");
}

/**
 * 临时的，不应该是一个用户接口
 */
void Shell::lseek()
{
    Logcat::log(TAG, "lseek EXEC");
}

/**
 * 用户指令：更改当前目录
 */
void Shell::cd()
{

    //cd必须带参数
    if (getParamAmount() != 2)
    {
        printf("Error!cd命令参数个数错误！");
    }
    else
    {
        bounded_VFS->cd(getParam(1));
    }
}

/**
 * ls函数可以带参数，也可以不带（curDir）
 */
void Shell::ls()
{
    if (!strcmp(getParam(1), ""))
    {
        //不带参数的ls，以curDir为默认参数
        bounded_VFS->ls(VirtualProcess::Instance()->getUser().curDirInodeId);
    }
    else
    {
        bounded_VFS->ls(getParam(1)); //getParam(1)获得的是ls后面跟的目录名（可能是相对的也可能是绝对的）
    }
}

/**
 * 将外部文件考入虚拟磁盘.带两个命令参数
 * Usage: store [src path] [des filename]
 */
void Shell::store()
{
    if (getParamAmount() == 3)
    {
        InodeId desInodeId;
        //STORE的步骤
        //Step1：创建文件（如果有同名的返回失败）
        desInodeId = bounded_VFS->createFile(getParam(2));
        if (desInodeId < 0)
        {
            Logcat::log("ERROR!目标文件名已存在！");
            return;
        }
        //Step2：打开文件
        Path desPath(getParam(2));
        FileFd fd_des = bounded_VFS->open(desPath, File::FWRITE);
        //Step3：写入文件
        FILE *fd_src = fopen(getParam(1), "rb");
        if (fd_src == NULL)
        {
            Logcat::log("源文件打开失败！");
            return;
        }
        DiskBlock tempBuf;
        int file_size = 0;
        while (!feof(fd_src))
        {
            //int blkCount = 0;
            int readsize = fread(&tempBuf, 1, DISK_BLOCK_SIZE, fd_src);
            file_size += readsize;
            bounded_VFS->write(fd_des, (u_int8_t *)&tempBuf, readsize);
        }
        Inode *p_desInode = Kernel::instance()->getInodeCache().getInodeByID(desInodeId);
        p_desInode->i_size = file_size; //TODO这一块不太好，封装性差了点

        //Step4：关闭文件
        fclose(fd_src);
        bounded_VFS->close(fd_des);
    }
    else
    {
        Logcat::log("ERROR!store命令参数个数错误");
    }
}

/**
 * 将文件从虚拟磁盘中拷出
 * Usage: withdraw [src filename] [des outer_path]
 */
void Shell::withdraw()
{
    if (getParamAmount() == 3)
    {
        InodeId desInodeId;
        //WITHDRAW的步骤
        //Step1：创建文件（如果有同名的返回失败）
        FILE *fd_des = fopen(getParam(2), "wb");
        if (fd_des == NULL)
        {
            Logcat::log("目的文件创建失败！");
            return;
        }

        //Step2：打开文件
        Path srcPath(getParam(1));
        FileFd fd_src = bounded_VFS->open(srcPath, File::FREAD);
        if (fd_src < 0)
        {
            Logcat::log("源文件打开失败！");
            return;
        }
        //Step3：写入文件
        DiskBlock tempBuf;
        while (!bounded_VFS->eof(fd_src))
        {
            //int blkCount = 0;
            int writesize = bounded_VFS->read(fd_src, (u_int8_t *)&tempBuf, DISK_BLOCK_SIZE);
            fwrite(&tempBuf, 1, writesize, fd_des);
        }
        //Step4：关闭文件
        fclose(fd_des);
        bounded_VFS->close(fd_src);
    }
    else
    {
        Logcat::log("ERROR!store命令参数个数错误");
    }
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
