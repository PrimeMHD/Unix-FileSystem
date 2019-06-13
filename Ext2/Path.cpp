#include "../include/Path.h"
Path::Path(){
    memset(path,0,sizeof(path));
    strcpy(path[0],"/");
    from_root=true;
    level=0;
}
Path::Path(const char *raw_path)
{
    path_str = strdup(raw_path);
    if (path_str[0] == '/')
    {
        temp_str = path_str + 1; //跳过正斜
        from_root = true;
    }
    else
    {
        temp_str = path_str;
        from_root = false;
    }
    l_len = strlen(path_str);
    i_len = 0;
    char *p = strtok(temp_str, "/");
    int i;
    for (i = 0; p != nullptr && i_len < l_len; i++)
    {
        strcpy(path[i], p);
        sec_len = strlen(p) + 1; //这次从路径str取出的字符数（+1是因为算上/）
        i_len += sec_len;
        temp_str += sec_len;
        p = strtok(temp_str, "/");
    }
    level = i; /*类似于"/home"这样的属于level=1*/
    delete path_str;
}

bool Path::isSingleName() const
{
    return level == 1;
}
std::string Path::toString()
{
    std::string path_str;
    if (from_root)
    {
        path_str.append("/");
    }
    int i;
    for (i = 0; i < level - 1; i++)
    {
        path_str.append(path[i]).append("/");
    }
    path_str.append(path[i]);
    return path_str;
}

const char *Path::getInodeName() const
{
    return path[level - 1];
}
