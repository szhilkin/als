#ifndef DIRCTL_H
#define DIRCTL_H

#include "trm.h"
#include "main.h"
#include "dirlst.h"

// общие атрибуты вывода

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
  int cols;			// столбцов для вывода
  int rows;			// строк для вывода
  int s_right,s_left;		// признаки скроллинга
  int files;			// кол-во файлов в директории - 1 (кроме .)
  int xinc;			// величина увеличения по x
  int items;			// кол-во позиций для вывода
  int bx;			// начальная позиция для x
  int by;			// начальная позиция для y
  int current;			// текущий выделенный файл
  int exist_tagged;		// 1 если есть помеченные
  int cx,cy;			// положение курсора
  int color_dirs;		// расцвечивать или нет директории
  int pos;
  int indirout;			// находимся в режиме отображения директории

  void checktag();		// проверка есть ли помеченные
  int cmdcheck(int key);	
//  int cmdcheckold(int key);	
  void setattr(int attr);	// установка атрибутов для вывода
  void setmono(int attr);	// установка mono атрибутов
  void getdir();
  void calcvars();
  void dirout(int pos, int out = 1);
  void showkeys();
  int preprocess();		// препроцессинг командной строки

  int cmdedit(int edit = 1);	// редактирование и выполнение команды
  int enter();			// обработка нажатия ENTER
  void edit();			// F4,4
  void copy();			// F5,5
  void move();			// F6,6
  void mkdir();			// F7,7
  void rm();			// F8,8
  void tag();			// SPACE
  void reread();
  void tagmask(int tag = 0);	// выделить фалы по введенной маске
  
  public:

  dirctl(trm *t, alsconfig *conf);
  ~dirctl();
  void run();
  void showclock();		// показывает часы

};

#endif // DIRCTL_H
