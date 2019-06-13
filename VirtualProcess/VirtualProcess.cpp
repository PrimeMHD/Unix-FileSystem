#include "../include/VirtualProcess.h"
#include "../include/Path.h"
VirtualProcess VirtualProcess::instance;

VirtualProcess::VirtualProcess()
{
    //TODO 需要初始化VirtualProcess
    //strcpy(defaultUser.u_curdir, "/");
    //defaultUser.u_curDir = Path("/");
    defaultUser.curDirInodeId = 1;
}

VirtualProcess::~VirtualProcess()
{
    //TODO 需要初始化VirtualProcess
}

VirtualProcess *VirtualProcess::Instance()
{
    return &VirtualProcess::instance;
}

User &VirtualProcess::getUser()
{
    return defaultUser;
}

/* 获取用户ID，低16比特为真实用户ID(u_ruid)，高16比特为有效用户ID(u_uid) */
short VirtualProcess::Getuid()
{
    return defaultUser.u_uid;
}

/* 获取组ID, 低16比特为真实组ID(u_rgid)，高16比特为有效组ID(u_gid) */
short VirtualProcess::Getgid()
{
    return defaultUser.u_gid;
}