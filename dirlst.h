#ifndef DIRLST_H
#define DIRLST_H

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include "main.h"

#define SORT_BY_NAME	1

struct fileinfo
{
  char *fname;
  char *sname;		// short name for names > 77 chars
  char ftype;
  char fperm[10];
  off_t fsize;
  char fexec;
  char flink;
  char fmark;
};

class dirlst
{
  private:

  struct fileinfo *fi, *cfi;
  int files;
  int maxsize;
  int init;
  char cwd[PATH_MAX];
  char *prevdir;
  alsconfig *cnf;
  //_POSIX_PATH_MAX

  void clearlist();
  int makelist();
  void namesort(struct fileinfo *fi, int low, int high);
  void sort(int type);

  public:

//  char errmsg[255]; //!debug

  dirlst(alsconfig *conf);
  ~dirlst();
  void scandir();
//  void show(); //!debug
  int getmaxsize() { return maxsize; }
  int getfiles() { return files; }
  int move(int pos);
  char *getdirname();
  char *getprevdir();
  int chdir(char *s);		//смена директории с проверкой прав 
  int prevdirpos();		//определение позиции предыдущей директории
  char *getname();		// returns sname if it exist or fname
  char *getfname() { return cfi->fname; }
  char getftype() { return cfi->ftype; }
  char getfperm(int i) { return cfi->fperm[i]; }
  off_t getfsize() { return cfi->fsize; }
  char getfexec() { return cfi->fexec; }
  char getflink() { return cfi->flink; }
  char getfmark() { return cfi->fmark; }
  void setfmark() { cfi->fmark = 1; }
  void unsetfmark() { cfi->fmark = 0; }
};

#endif // DIRLST_H
