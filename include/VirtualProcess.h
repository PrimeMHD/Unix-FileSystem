#ifndef VIRTUAL_PROCESS_H
#define VIRTUAL_PROCESS_H
#include "define.h"
#include "User.h"

/**VitualProcess类说明：
 * 
 * 我们写的用户级文件系统没有进程，但是却要模拟进程做的事情，比如打开文件open creat lseek等。
 * 所以我们这里创建这样一个虚假的进程结构，里面存放的数据能够支持打开文件结构。
 * 
 */

class VirtualProcess
{
private:
  User defaultUser;
  static VirtualProcess instance;

public:
  VirtualProcess();
  ~VirtualProcess();

  static VirtualProcess *Instance(); //单例模式，记得考虑线程安全

  // /* 根据系统调用参数uid设置有效用户ID，真实用户ID，进程用户ID(p_uid) */
  // void Setuid();

  /* 获取用户ID，低16比特为真实用户ID(u_ruid)，高16比特为有效用户ID(u_uid) */
  short Getuid();

  // /* 根据系统调用参数gid设置有效组ID，真实组ID */
  // void Setgid();

  /* 获取组ID, 低16比特为真实组ID(u_rgid)，高16比特为有效组ID(u_gid) */
  short Getgid();

  // /* 获取当前用户工作目录 */
  // void Pwd();

  /* 获取User结构 */
  User &getUser();
};

#endif