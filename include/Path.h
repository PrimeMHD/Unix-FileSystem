#ifndef PATH_H
#define PATH_H
#include "define.h"

/**
 * Path是一个路径类，是将原始路径字符串解析构造而来
 */

class Path
{
private:
    char *path_str, *temp_str;
    int sec_len, i_len, l_len;
    //上面都是临时的，外部不要用
public:
    char path[MAX_PATH_LEVEL][MAX_FILENAME_LEN];
    bool from_root;
    int level;
    //Operations:
    Path();
    explicit Path(const char *raw_path); //explicit关键字避免隐式类型转换
    bool isSingleName() const;
    std::string toString();
    const char *getInodeName() const;
};
#endif //PATH_H