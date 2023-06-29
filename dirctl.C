#include "dirctl.h"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <fnmatch.h>

#include "config.h"

#ifdef LINUX
#include <time.h>
#endif

static char *cmd;		// командная строка
static char *prep;		// командная строка после препроцессора

//
// показ часов по таймеру
//
void dirctl::showclock()
{
  if (!term->is_visual()) return;	// выходим если не в визуал моде
  if (!indirout) return;		// выходим если не в режиме директ.
  char s[8];
  time_t timv = time(0);
  struct tm *ltm = localtime(&timv);
  sprintf(s,"%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
  setattr(AT_TAG);
  term->print(1,72,s);
  setattr(AT_NORMAL);
  term->move(term->li()-1,0);
  fflush(stdout);
}

static void inr()
{
  strcpy(rl_line_buffer, cmd);
  rl_point = rl_end = strlen(rl_line_buffer);
}

dirctl::dirctl(trm *t, alsconfig *conf)
{
  cnf = conf;
  dlst = new dirlst(cnf);
  cx = cy = -1;
  pos = 0;
  current = 0;
  term = t;
  getdir();
  rl_line_buffer = new char[4096];
  rl_startup_hook = (Function *)inr; 
  cmd = new char[4096];
  prep = new char[4096];
  color_dirs = cnf->colordirs; //признак расцвечивания директорий
  indirout = 1;
}

dirctl::~dirctl()
{
  s_left = s_right = 0;
  term = NULL;
  delete rl_line_buffer;
  delete prep;
  delete cmd;
  delete dlst;
}

void dirctl::getdir()
{
  static int first=1;
  s_left = s_right = 0;
  int y=by;
  int diff;
  dlst->scandir();
  calcvars();
  if (first) {
    char *s = new char[30];
 //   sprintf(s,"Welcome to al-shell v.%d.%d",ALSVER_MAJ,ALSVER_MIN);
    sprintf(s,"Welcome to al-shell v.0.3");
    term->print(by-1,0,s);
    delete s;
    first=0;
  }
  diff = y - by;
  if (diff > 0) {
   term->move(term->li()-1,0);
   term->closev(); for (int i=0; i<diff; i++) printf("\n"); fflush(stdout);
   term->openv();
  }
}

void dirctl::calcvars()
{
  files = dlst->getfiles();		// кол-во файлов в директории
  cols = term->co() / (dlst->getmaxsize() + 3);  // стобцов для вывода
  rows = files / cols;	// строк для вывода
  if ((files % cols) != 0) {
    rows++;	// +1 неполный
    //проверка, можно ли уместиться в меньшее кол-во строк
    if ((rows * cols - files) == rows) cols--; 
  }
  int rem = term->co() - (dlst->getmaxsize() + 3) * cols;
  xinc = (rem / cols) + (dlst->getmaxsize() + 3);
  // проверка, есть ли скроллинг
  // по умолчанию 10 строк из конфига
  if (rows > cnf->maxlines) {
    s_right =1; 	// скроллируется вправо
    rows = cnf->maxlines;
  }
  items = rows * cols;		// кол-во позиций для вывода
  by = term->li() - rows - 2;	// начальные позиции для вывода
  bx = 0;
  while (1) {
    if (current < 0) { current=0; break; }
    if (current >= files) current=files-1;
    else break;
  }
  // текущий выделенный файл
}

void dirctl::dirout(int pos, int out = 1)
{
  char s[MAXNAMLEN];
  int x = bx;
  int y = by;
  static int dirpos=term->co();
  char dirs[MAXNAMLEN];

  // стираем предыдущую текущую директорию 
  setattr(AT_NORMAL);
  term->cleol(0);

  // определить новую позицию для вывода и вывести текущую директорию
  strcpy(s, dlst->getdirname()); dirpos=term->co()-2-strlen(s);
  // 30 позиций резервируется для вывода user@hostname
  if (dirpos < 30) {
    strcpy(dirs, &s[32-dirpos]);
    dirpos = 30; 
    sprintf(s,"[..%s]", dirs);
  } else { 
    strcpy(dirs, s);
    sprintf(s,"[%s]", dirs);
  }
  // выводим текущую директорию
  setattr(AT_HELP); 
  term->print(0,dirpos,s); 

  // выводим user@hostname
  sprintf(s,"[%s@%s]",cnf->getusername(),cnf->gethostname());
  if (strlen(s) > 29) s[29] = 0;
  term->print(0,0,s);
  setattr(AT_NORMAL);

  // гасим предыдущий маркер
  setattr(AT_NORMAL); 
  term->print(cy, cx, " "); 
  if (out) term->cleos(by, bx);
  if (out) showkeys();
  for (int i = pos; i < items; i++) {
    if (!dlst->move(i)) break;
    switch (dlst->getftype()) {
      case 'd': if (dlst->getfmark()) {
                  sprintf(s, " #%s/", dlst->getname()); setattr(AT_TAG); 
                } else {
                  sprintf(s, "  %s/", dlst->getname()); setattr(AT_DIR); 
                }
                break;
      case 'e': if (dlst->getfmark()) { 
                  sprintf(s, " #%s?", dlst->getname()); setattr(AT_TAG); 
                } else {
                  sprintf(s, "  %s?", dlst->getname()); setattr(AT_ERR); 
                }
                break;
      default: if (dlst->getfmark()) {
                 sprintf(s, " #%s ", dlst->getname()); setattr(AT_TAG); 
               } else {
                 sprintf(s, "  %s ", dlst->getname()); setattr(AT_NORMAL); 
               }
               break;
    }
    if ((dlst->getftype() != 'd') & (dlst->getflink() != 1) & 
        (dlst->getfperm(2) == 'x')) {
      if (dlst->getfmark()) {
        sprintf(s, " #%s*", dlst->getname()); setattr(AT_TAG);
      } else {
        sprintf(s, "  %s*", dlst->getname()); setattr(AT_EXEC);
      }
    }
    else if (dlst->getflink() == 1) {
      if (dlst->getfmark()) {
        sprintf(s, " #%s@", dlst->getname()); setattr(AT_TAG);
      } else {
        sprintf(s, "  %s@", dlst->getname()); setattr(AT_LINK);
      }
    }
    if (out) {
      if (!color_dirs) setattr(AT_NORMAL);
      term->print(y, x, s);
    }
    if (i == current) {
      cx = x; cy = y;
    }
    y++;
    if (y > (term->li() - 3)) {
      y = term->li() - rows - 2; x += xinc;
    }
  }
  // устанавливаем курсор
  setattr(AT_MARKER);
  term->print(cy, cx, " ");
  setattr(AT_NORMAL);
  term->move(term->li()-1, 0);
  fflush(stdout);
}

void dirctl::showkeys()
{
  int y = term->li();
  int x = term->co();
  int inc = x / 10;

  y--; x = 0;
  setattr(AT_NORMAL);
  term->print(y, x, "1"); x += inc;
  term->print(y, x, "2"); x += inc;
  term->print(y, x, "3"); x += inc;
  term->print(y, x, "4"); x += inc;
  term->print(y, x, "5"); x += inc;
  term->print(y, x, "6"); x += inc;
  term->print(y, x, "7"); x += inc;
  term->print(y, x, "8"); x += inc;
  term->print(y, x, "9"); x += inc;
  term->print(y, x, "0"); x += inc;
  setattr(AT_HELP);
  x = 1;
  term->print(y, x, "sh>   "); x += inc;
  term->print(y, x, "Menu  "); x += inc;
  term->print(y, x, "View  "); x += inc;
  term->print(y, x, "Edit  "); x += inc;
  term->print(y, x, "Copy  "); x += inc;
  term->print(y, x, "MovRen"); x += inc;
  term->print(y, x, "MkDir "); x += inc;
  term->print(y, x, "Delete"); x += inc;
  term->print(y, x, "Setup "); x += inc;
  term->print(y, x, "Quit  "); x += inc;
  setattr(AT_NORMAL);
}

void dirctl::run()
{
  int key;
  int bye = 0;
  char s[PATH_MAX];
  int draw;

  pos = 0;
  dirout(pos);
  while (!bye) {
    //curs_set(2)
    key = term->readkey();
//    term->closev();
//    printf("\n%d", key);
//    term->openv();
//    term->readkey();
    //curs_set(1)

    if (!cmdcheck(key)) {
      switch (key) {
        case K_F10: 
        case K_0:      term->cleos(by,bx); bye = 1; break; // quit
        case K_R:      reread(); break;  // reread
        case K_CTRL_L: term->cleos(0,0); getdir(); pos = 0; dirout(pos);
                       break;
        case K_DOWN:
          draw = 0;
          key_down:
          if (++current >= items) {
            if (s_right) {
              pos += rows;
              items += rows;
              items = (items > files) ? files : items;
              s_left = 1;
              if (items == files) s_right = 0;
              draw = 1;
            } else current--;
          }
          current = (current >= files) ? files-1 : current;
          dirout(pos, draw);
          break;
        case K_UP:
          draw = 0;
          if (--current < pos) {
            if (s_left) {
              pos -= rows;
              items = (pos + rows * cols);
              s_right = (items == files) ? 0 : 1;
              if (pos == 0) s_left = 0;
              draw = 1;
            } else current++;
          }
          current = (current <= pos) ? pos : current;
          dirout(pos, draw);
          break;
        case K_RIGHT:
          draw = 0;
          current += rows;
          if (current >= items) {
            if (s_right) {
              pos += rows;
              items += rows;
              items = (items > files) ? files : items;
              s_left = 1;
              if (items == files) s_right = 0;
              draw = 1;
            } else current = files-1;
          }
          current = (current >= files) ? files-1 : current;
          dirout(pos, draw);
          break;
        case K_LEFT:
          draw = 0;
          current -= rows;
          if (current <= pos) {
            if (s_left) {
              pos -= rows;
              items = pos + rows * cols;
              s_right = (items == files) ? 0 : 1;
              if (pos == 0) s_left = 0;
              draw = 1;
            } else current = pos;
          }
          current = (current < pos) ? pos : current;
          dirout(pos, draw);
          break;
        case K_BSPACE:
          if (dlst->chdir("..") == 2) { // переход в предыдущую директорию
            term->cleos(by, bx); getdir();
            int i = dlst->prevdirpos(); pos = 0;
            do {
              if (i <= items) {
                current = i; break;
              }
              pos += rows;
              items += rows;
              items = (items > files) ? files : items;
              s_left = 1;
              if (items == files) s_right = 0;
            } while (1);
            dirout(pos);
          }
          break;
        case K_ENTER:
          switch (enter()) {
	    case 3: reread(); break;
                    // getdir(); dirout(pos); break;
	    case 1: // смена директории другим способом
	          term->cleos(by, bx); getdir();
                  current = 0;
		  pos = 0; dirout(pos);
                  break;
            case 2: // смена директории через ..
	          term->cleos(by, bx);
		  getdir(); int i = dlst->prevdirpos();
		  pos = 0;
		  do {
		    if (i <= items) {
		      current = i; break;
		    }
		    pos += rows;
		    items += rows;
                    items = (items > files) ? files : items;
                    s_left = 1;
                    if (items == files) s_right = 0;
		  } while (1);
		  dirout(pos);
		  break;
	      }
          break;
        case K_SPACE:
          tag(); draw = 1; goto key_down;
        case K_PLUS:
          tagmask(1); dirout(pos); break;
        case K_MINUS:
          tagmask(); dirout(pos); break;
        case K_CTRL_H:
          cnf->hidden = (cnf->hidden) ? 0 : 1;
	      term->cleos(by, bx); 
          reread();
          break;  
        default:
          sprintf(cmd, "%c", term->lastchar());
          cmdedit(); 
          reread();
      }
    } else reread();
  }
}

//
// перечитывание директории с правильным позиционированием курсора
//

void dirctl::reread()
{
 int i = current;
 getdir(); 
 pos = 0;
 if (i >= files) i = files-1;
 do {
   if (i < items) {
     current = i;
     break;
   }
   pos += rows;
   items += rows;
   items = (items > files) ? files : items;
   s_left = 1;
   if (items == files) s_right = 0;
 } while (1);
 dirout(pos); 
}

//
// проверка есть ли помеченные и установить exist_tagged
//
void dirctl::checktag()
{
  exist_tagged=0;
  for (int i=0; i<files; i++) {
    dlst->move(i);
    if (dlst->getfmark()) { exist_tagged=1; break; }
  }
}

//
// реакция на нажатие ENTER
//
int dirctl::enter()
{
  if (!dlst->move(current)) return 0;
  if (dlst->getftype() == 'd')
    return dlst->chdir(dlst->getfname()); // смена директории
  // тут д.б. обработка других событий
  if (dlst->getfperm(2) == 'x') {
    sprintf(cmd, "./%s", dlst->getfname());
    cmdedit();
    return 3;
  }
  return 0;
}

int dirctl::preprocess()
{
  char *s,*p;
  char *cmd_t = cmd;
  int cd=0,i;
  char t[100];

  strcpy(rl_line_buffer, cmd);
  strcpy(prep, "");
  while ((s=strsep(&cmd, " ")) != NULL) {
    // проверка на переменную среды
    if (s[0]=='$') {
      t[0]='\0'; 
      for (i=1; i<99; i++) {
        if (!isupper(s[i])) break;
        t[i-1]=s[i];
      }
      t[i-1]='\0'; p = getenv(t);
      if (p) strcat(prep, p);
      strcat(prep, &s[i]); strcat(prep, " ");
      continue;
    }
    if (strcmp(s, "#@") == 0) {
      dlst->move(current);
      strcat(prep, dlst->getfname()); strcat(prep, " ");
      continue;
    }
    if (strcmp(s, "##") == 0) {
      for (i=0; i<files; i++) {
	dlst->move(i);
        if (dlst->getfmark()) { 
          strcat(prep, dlst->getfname()); strcat(prep, " ");
        }
      }
      continue;
    }
    strcat(prep, s);
    strcat(prep, " ");
  }

  // если это cd в командной строке
  if (strncmp(prep, "cd ", 3)==0) {
    for (i=3; i<99; i++) {
      if (prep[i]==' ') break;
      t[i-3] = prep[i];
    }
    t[i-3]='\0';
    if (dlst->chdir(t)) current=1;
    cmd=cmd_t;
    strcpy(cmd, rl_line_buffer);
    return 1;  
  }

  int len = strlen(prep) - 1;
  if (len < 0) len = 0;
  prep[len] = '\0';
  cmd = cmd_t;
  strcpy(cmd, rl_line_buffer);
  return 0;
}

int dirctl::cmdedit(int edit = 1)
{
  int ret, ret0, ret1;
  int key;
  HIST_ENTRY *hs;

  while (1) {
    if (edit) {
      term->cleos(term->li()-1,0); term->move(term->li()-1,0); 
      term->closev(); 
      readline("sh> ");
      strcpy(cmd, rl_line_buffer);
      // если строка пустая ничего не делаем
      if (strlen(cmd)==0) {
        term->openv(); term->cleos(by-1, bx); term->move(by-1,bx);
        return 1;
      }
    }
    edit = 1;
    if (preprocess()) { // смена директории внутри препроцесса
      term->openv(); term->cleos(by-1, bx); term->move(by-1,bx);
      return 1;
    }
    term->openv(); term->cleos(by-1, bx); term->move(by-1,bx);
    term->closev();
    printf("sh> %s\n", prep);
    fflush(stdout);
    //
    indirout=0;
    signal(SIGINT, SIG_DFL);
    ret = system(prep);
    signal(SIGINT, SIG_IGN);
    //
    if (ret) {
      ret0 = ret & 0xff; ret1 = (ret >> 8) & 0xff;
//? нужно ли добавлять в history при ненулевом коде
      add_history(cmd);
      printf("[ Exit: (%d/%d) ]", ret1, ret0);
    } else {
      //add_history(rl_line_buffer);
      add_history(cmd);
      printf("[ OK ]");
    }
    fflush(stdout);
    term->openv(); key = term->readkey(); term->closev();
    if (key == K_ENTER | key == K_SPACE) {
      printf("\r                  ");
      break;
    } 
    if (term->charcount() == 1)
      sprintf(cmd, "%c", term->lastchar());
  }

  for (int i=0; i<(term->li()-by); i++) printf("\n");
  term->openv();
  indirout = 1;
  return ret;
}

// выделение файлов по маске

void dirctl::tagmask(int tag = 0)
{
  sprintf(cmd,"*");
  term->cleol(term->li()-2); term->move(term->li()-2,0);
  term->closev();
  if (tag) readline("Enter file mask [+]> ");
  else readline("Enter file mask [-]> ");
  strcpy(cmd, rl_line_buffer);
  term->openv(); term->cleos(by-1,bx); term->move(by-1,bx);

  for (int i = 0; i < dlst->getfiles(); i++) {
    dlst->move(i);
    if (strcmp(dlst->getfname(),"..") == 0) continue;
    if (fnmatch(cmd,dlst->getfname(),0) == 0) 
      if (tag) dlst->setfmark(); else dlst->unsetfmark();
  }
}

//SPACE
void dirctl::tag()
{
  if (!dlst->move(current)) return;
  if (dlst->getfmark()) dlst->unsetfmark(); else dlst->setfmark();
}

void dirctl::setmono(int attr)
{
  switch(attr) {
    case A_NORM: term->normal(); break;
    case A_BOLD: term->bold(); break;
    case A_REVERSE: term->reverse(); break;
  }
}

void dirctl::setattr(int attr)
{
  if (cnf->ansicolor)
    switch (attr) {
      case AT_NORMAL: 
        setmono(cnf->a_normal.attr); 
        term->setcolor(cnf->a_normal.fg, cnf->a_normal.bg);
        break;
      case AT_HELP: 
        setmono(cnf->a_help.attr);
        term->setcolor(cnf->a_help.fg, cnf->a_help.bg);
        break;
      case AT_MARKER: 
        setmono(cnf->a_marker.attr);
        term->setcolor(cnf->a_marker.fg, cnf->a_marker.bg);
        break;
      case AT_DIR: 
        setmono(cnf->a_dir.attr);
        term->setcolor(cnf->a_dir.fg, cnf->a_dir.bg);
        break;
      case AT_LINK: 
        setmono(cnf->a_link.attr);
        term->setcolor(cnf->a_link.fg, cnf->a_link.bg);
        break;
      case AT_EXEC: 
        setmono(cnf->a_exec.attr);
        term->setcolor(cnf->a_exec.fg, cnf->a_exec.bg);
        break;
      case AT_ERR: 
        setmono(cnf->a_err.attr);
        term->setcolor(cnf->a_err.fg, cnf->a_err.bg);
        break;
      case AT_TAG: 
        setmono(cnf->a_tag.attr);
        term->setcolor(cnf->a_tag.fg, cnf->a_tag.bg);
        break;
    }
  else
    switch (attr) {
      case AT_NORMAL: setmono(cnf->a_normal.attr); break;
      case AT_HELP: setmono(cnf->a_help.attr); break;
      case AT_MARKER: setmono(cnf->a_marker.attr); break;
      case AT_DIR: setmono(cnf->a_dir.attr); break;
      case AT_LINK: setmono(cnf->a_link.attr); break;
      case AT_EXEC: setmono(cnf->a_exec.attr); break;
      case AT_ERR: setmono(cnf->a_err.attr); break;
      case AT_TAG: setmono(cnf->a_tag.attr); break;
    }
}

//
// проверка клавиатурных команд и предварительное заполнение
// командной строки
//

int dirctl::cmdcheck(int key)
{
  int found = 0;
  cmdstring *p;
  
  checktag();	// проверить есть ли помеченные и выставить флаг

  if (!dlst->move(current)) return 0;
  for (int i = 0; i < cnf->cmdcount; i++) {
    p = cnf->cmdstr[i];
    if (key == p->getkey()) {  // клвиша подходит, начинаем другие проверки
      // обработка помеченных клавиш
      if (p->gettag())
        if (!exist_tagged) continue;  // если нет помеченных файлов
      // проверка есть ли фильтрация по типам файлов
      if (p->getftypes() != 0) {
        if (p->getftypes() & FT_DIR)
          if (dlst->getftype() != 'd') continue;
        if (p->getftypes() & FT_LINK)
          if (dlst->getflink() != 1) continue;
        if (p->getftypes() & FT_EXEC)
          if (dlst->getftype() == 'd' | dlst->getflink() == 1 | 
	      dlst->getfperm(2) != 'x')
            continue;
      }
      // проверка есть ли фильтрация по внешему виду файла
      if (p->getfmask() != NULL) 
        if (fnmatch(p->getfmask(), dlst->getfname(), 0) != 0 ) continue;
      //
      found = 1; break;
    }
  }
  if (found) { // если найдено совпадение
    strcpy(cmd, p->getcmd());
    if (p->getprep()) {   // если нужен дополнительный препроцессинг
      if (preprocess()) {
        return 1; // смена директории внутри препроцесса
      }
      strcpy(cmd, prep);
    }
    cmdedit(!p->getnow());
  }
  return found;  
}

