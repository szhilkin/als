// управление драйвером терминала

#include "tty.h"
#include <unistd.h>

tty::tty()
{
  cur_mode = inited = 0;
  init();
}

tty::~tty()
{
}

void tty::init()
{
  // открыть терминал в режиме "чтение без ожидания"
  fdtty = open("/dev/tty", O_RDWR | O_NDELAY);
  // получить текущие режимы драйвера
#ifdef HAVE_TERMIO_H
  ioctl(fdtty, TCGETA, &oldt);
#else
  ioctl(fdtty, TIOCGETA, &oldt);
#endif
  newt = oldt;
  
  // input flags
  // отменить преобразование \r в \n на вводе
  newt.c_iflag &= ~ICRNL;
  if ((oldt.c_cflag & CSIZE) == CS8)	// 8-битный флаг
    newt.c_iflag &= ~ISTRIP;		// отменить & 0177 на вводе

  // output flags
  // отменить замену TAB3 замену табуляций \t на пробелы
  // отменить ONCLR замену \n на пару \r\n на выводе
#ifdef HAVE_TERMIO_H
  newt.c_oflag &= ~(TAB3 | ONLCR);
#else
  newt.c_oflag &= ~(OXTABS | ONLCR);
#endif

  // local flags
  // выключить режим ICANON, включить CBREAK
  // выключить эхо
  newt.c_lflag &= ~(ICANON | ECHO);
  
  // control chars
  // при вводе с клавиш ждать не более...
  newt.c_cc[VMIN] = 1;		// 1 символа
  newt.c_cc[VTIME] = 0;		// 0 секунд
  // это соответствует CBREAK
  
  // символы, нажатие которых заставляет драйвер терминала послать
  // сигнал либо отредактировать набранную строку. 0 означает,
  // что соответствующего сигнала не будет
  newt.c_cc[VINTR] = ctrl('C');		// SIGINT
  newt.c_cc[VQUIT] = '\0';		// SIGQUIT
  newt.c_cc[VERASE] = '\0';
  newt.c_cc[VKILL] = '\0';
  
  inited = 1;
}

void tty::openv()
{
  if (!inited) return;
  if (cur_mode) return;
#ifdef HAVE_TERMIO_H
  ioctl(fdtty, TCSETAW, &newt);
#else
  ioctl(fdtty, TIOCSETA, &newt);
#endif
  cur_mode = 1;
}

void tty::closev()
{
  if (!inited) return;
  if (cur_mode == 0) return;
#ifdef HAVE_TERMIO_H
  ioctl(fdtty, TCSETAW, &oldt);
#else
  ioctl(fdtty, TIOCSETA, &oldt);
#endif
  cur_mode = 0;
}
