#include "../include/VirtualProcess.h"

VirtualProcess VirtualProcess::instance;

VirtualProcess::VirtualProcess()
{
    //TODO 需要初始化VirtualProcess
}

VirtualProcess::~VirtualProcess()
{
    //TODO 需要初始化VirtualProcess
}

VirtualProcess &VirtualProcess::Instance()
{
    return VirtualProcess::instance;
}

User &VirtualProcess::getUser()
{
    return defaultUser;
}