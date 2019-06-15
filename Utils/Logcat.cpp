#include "../include/Logcat.h"

void Logcat::log(const char *str)
{
    std::cout << str << std::endl;
}

void Logcat::log(const char *tag, const char *str)
{
    std::cout << tag << ":" << str << std::endl;
}

void Logcat::devlog(const char *tag, const char *str)
{
#ifdef IS_DEBUG
    std::cout << tag << ":" << str << std::endl;
#endif
}
void Logcat::devlog(const char *str)
{
#ifdef IS_DEBUG
    std::cout << str << std::endl;
#endif
}
