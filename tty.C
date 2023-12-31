// ���������� ��������� ���������

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
  // ������� �������� � ������ "������ ��� ��������"
  fdtty = open("/dev/tty", O_RDWR | O_NDELAY);
  // �������� ������� ������ ��������
#ifdef HAVE_TERMIO_H
  ioctl(fdtty, TCGETA, &oldt);
#else
  ioctl(fdtty, TIOCGETA, &oldt);
#endif
  newt = oldt;
  
  // input flags
  // �������� �������������� \r � \n �� �����
  newt.c_iflag &= ~ICRNL;
  if ((oldt.c_cflag & CSIZE) == CS8)	// 8-������ ����
    newt.c_iflag &= ~ISTRIP;		// �������� & 0177 �� �����

  // output flags
  // �������� ������ TAB3 ������ ��������� \t �� �������
  // �������� ONCLR ������ \n �� ���� \r\n �� ������
#ifdef HAVE_TERMIO_H
  newt.c_oflag &= ~(TAB3 | ONLCR);
#else
  newt.c_oflag &= ~(OXTABS | ONLCR);
#endif

  // local flags
  // ��������� ����� ICANON, �������� CBREAK
  // ��������� ���
  newt.c_lflag &= ~(ICANON | ECHO);
  
  // control chars
  // ��� ����� � ������ ����� �� �����...
  newt.c_cc[VMIN] = 1;		// 1 �������
  newt.c_cc[VTIME] = 0;		// 0 ������
  // ��� ������������� CBREAK
  
  // �������, ������� ������� ���������� ������� ��������� �������
  // ������ ���� ��������������� ��������� ������. 0 ��������,
  // ��� ���������������� ������� �� �����
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
