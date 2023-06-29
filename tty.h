// управление драйвером терминала

#ifndef TTY_H
#define TTY_H

#include "config.h"

#ifdef HAVE_TERMIO_H
  #include <termio.h>
#else
  #include <termios.h>
#endif
#include <fcntl.h>

#define ctrl(c) ((c) & 037)

class tty
{
  private:
  
  int cur_mode;
  int inited;
  
#ifdef HAVE_TERMIO_H
  struct termio oldt,newt;
#else
  struct termios oldt,newt;
#endif
  int fdtty;
  
  void init();
  
  public:
  
  tty();
  ~tty();
  void openv();
  void closev();
  int mode() { return cur_mode; }
};

#endif // TTY_H
