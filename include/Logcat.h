#ifndef LOGCAT_H
#define LOGCAT_H
#include "define.h"
class Logcat
{
private:
  enum LOGCAT_STYLE
  {
    DEFAULT
  };

public:
  static void log(const char *str);
  static void log(const char *tag, const char *str);
  static void devlog(const char *str);
  static void devlog(const char *tag, const char *str);
};

#endif