#include "../include/TimeHelper.h"
#include <ctime>

int TimeHelper::getCurTime()
{
    time_t now = time(0);
    return int(now);
} //获取当前时间
