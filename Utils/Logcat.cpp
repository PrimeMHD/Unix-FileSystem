#include "../include/Logcat.h"

void Logcat::log(const char *str)
{
    std::cout << str << std::endl;
}

void Logcat::log(const char *tag, const char *str)
{
    std::cout << tag << ":" << str << std::endl;
}

