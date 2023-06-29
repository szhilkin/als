#ifndef DIRCTL_H
#define DIRCTL_H

#include "trm.h"
#include "main.h"
#include "dirlst.h"

// ����� �������� ������

#define AT_NORMAL	0
#define AT_HELP		1
#define AT_MARKER	2
#define AT_DIR		3
#define AT_LINK		4
#define AT_EXEC		5
#define AT_ERR		6
#define AT_TAG		7

class dirctl
{
  private:

  trm *term;
  dirlst *dlst;
  alsconfig *cnf;
  int cols;			// �������� ��� ������
  int rows;			// ����� ��� ������
  int s_right,s_left;		// �������� ����������
  int files;			// ���-�� ������ � ���������� - 1 (����� .)
  int xinc;			// �������� ���������� �� x
  int items;			// ���-�� ������� ��� ������
  int bx;			// ��������� ������� ��� x
  int by;			// ��������� ������� ��� y
  int current;			// ������� ���������� ����
  int exist_tagged;		// 1 ���� ���� ����������
  int cx,cy;			// ��������� �������
  int color_dirs;		// ������������ ��� ��� ����������
  int pos;
  int indirout;			// ��������� � ������ ����������� ����������

  void checktag();		// �������� ���� �� ����������
  int cmdcheck(int key);	
//  int cmdcheckold(int key);	
  void setattr(int attr);	// ��������� ��������� ��� ������
  void setmono(int attr);	// ��������� mono ���������
  void getdir();
  void calcvars();
  void dirout(int pos, int out = 1);
  void showkeys();
  int preprocess();		// ������������� ��������� ������

  int cmdedit(int edit = 1);	// �������������� � ���������� �������
  int enter();			// ��������� ������� ENTER
  void edit();			// F4,4
  void copy();			// F5,5
  void move();			// F6,6
  void mkdir();			// F7,7
  void rm();			// F8,8
  void tag();			// SPACE
  void reread();
  void tagmask(int tag = 0);	// �������� ���� �� ��������� �����
  
  public:

  dirctl(trm *t, alsconfig *conf);
  ~dirctl();
  void run();
  void showclock();		// ���������� ����

};

#endif // DIRCTL_H
