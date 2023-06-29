#include "dirlst.h"
#include <string.h>

static char xtbl[10] = "rwxrwxrwx";

dirlst::dirlst(alsconfig *conf)
{
  cnf = conf;
  files = maxsize = 0;
  prevdir = NULL;
  init = 0;
}

dirlst::~dirlst()
{
  if (files == 0) return;
  for (int i = 0; i < files; i++) {
    delete (fi+i)->fname;
    if ((fi+i)->sname != NULL) delete (fi+i)->sname;
  }
  delete fi;
  if (prevdir != NULL) delete prevdir;
  maxsize = files = 0;
}

void dirlst::scandir()
{
  clearlist();
  makelist();
  sort(SORT_BY_NAME);
}

void dirlst::clearlist()
{
  if (files == 0) return;
  for (int i = 0; i < files; i++) delete (fi+i)->fname;
  delete fi;
  files = maxsize = 0;
}

int dirlst::makelist()
{
  DIR *dirp;
  struct dirent *dp;
  struct stat s;
  struct fileinfo *fip;
  int fn_size = 0;

  dirp = opendir("."); if (dirp == NULL) return 1;
  if (!init || !cnf->flinks) {
    getcwd(cwd, PATH_MAX);
    init=1;
  }
  while ((dp = readdir(dirp)) != NULL) {
    if (strcmp(dp->d_name,".")==0) continue;
    if (!cnf->hidden)
      if (strcmp(dp->d_name,"..")!=0 && dp->d_name[0]=='.') continue;
    files++;
  }
  rewinddir(dirp);
  fi = new fileinfo[files];
  fip = fi; cfi = fi;
  while ((dp = readdir(dirp)) != NULL) {
    if (strcmp(dp->d_name,".")==0) continue;
    if (!cnf->hidden)
      if (strcmp(dp->d_name,"..")!=0 && dp->d_name[0]=='.') continue;
    if (lstat(dp->d_name, &s) == -1) fip->ftype = 'e';
    fip->flink = 0;
    fip->fmark = 0;
    switch (s.st_mode & S_IFMT) {
      case S_IFLNK: fip->flink = 1; 
      case S_IFDIR: fip->ftype = 'd'; break;
      case S_IFCHR: fip->ftype = 'c'; break;
      case S_IFBLK: fip->ftype = 'b'; break;
      case S_IFREG: fip->ftype = '-'; break;
      case S_IFIFO: fip->ftype = 'p'; break;
    }
    fip->fname = new char[strlen(dp->d_name)+2];
    if (strlen(dp->d_name) > 75) {
      char ts[80];
      strncpy(ts, dp->d_name, 73); ts[73] = '\0';
      strcat(ts, "->");
      fip->sname = new char[strlen(ts)+2];
      strcpy(fip->sname, ts);
    } else {
      fip->sname = NULL;
    }
    strcpy(fip->fname, dp->d_name);
    fn_size = (fip->sname != NULL) ? strlen(fip->sname) : strlen(fip->fname);
    maxsize = (maxsize < fn_size) ? fn_size : maxsize;
    //
    for (int i = 0, j = (1 << 8); i < 9; i++, j >>= 1)
      fip->fperm[i]=(s.st_mode & j) ? xtbl[i] : '-';
    if (s.st_mode & S_ISUID)
      fip->fperm[2] = (fip->fperm[2] == 'x') ? 'S' : 's';
    if (s.st_mode & S_ISGID)
      fip->fperm[5] = (fip->fperm[5] == 'x') ? 'G' : 'g';
    if (s.st_mode & S_ISVTX)
      fip->fperm[8] = (fip->fperm[8] == 'x') ? 'T' : 't';
    fip++;
  }
  closedir(dirp);
  return 0;
}

int dirlst::move(int pos)
{
  if ((pos < 0) || (pos >= files)) return 0;
  cfi = fi;
  cfi += pos;
  return 1;
}

char *dirlst::getdirname()
{
  return cwd;
}

void dirlst::namesort(struct fileinfo *fi, int low, int high)
{
  struct fileinfo *x;
  struct fileinfo t;
  int i, j;

  if (low < high) {
    i = low;
    x = (fi + i);
    for (j = low + 1; j <= high; j++)
      if (strcmp((fi + j)->fname, x->fname) < 0) {
        i++; t = *(fi + i); *(fi + i) = *(fi + j);
        *(fi + j) = t;
      }
    t = *(fi + low); *(fi + low) = *(fi + i); *(fi + i) = t;
    namesort(fi, low, i - 1);
    namesort(fi, i + 1, high);
  }
}

void dirlst::sort(int type)
{
  if (type == SORT_BY_NAME) namesort(fi, 1, files - 1);
}

//
// смена директории с проверкой прав доступа
//
int dirlst::chdir(char *s)
{
  int i = 0;
  char *p = (char *)&cwd;
  int ret = 0;

  if (access(s, R_OK | X_OK) == 0) {
    ret = 1;
    if (strcmp(s, "..") == 0) {
      for (i = strlen(cwd); i >= 0; i--) if (cwd[i] == '/') break;
      if ((i == 0) & (strlen(cwd) == 1)) return 0;
      if (prevdir != NULL) delete prevdir;
      prevdir = new char[strlen(cwd)-i];
      strcpy(prevdir, (p+i+1));
      ret = 2;
    }
    if (cnf->flinks) { 
      if (ret==2)  if (i==0) { cwd[0]='/'; cwd[1]='\0'; } else { cwd[i]='\0'; }
      else { 
        if (s[0]=='/') {
          if (strlen(s)!=1) {
            if (s[strlen(s)-1]=='/') s[strlen(s)-1]='\0';
          }
          cwd[0]='\0'; strncat(cwd, s, PATH_MAX); 
        } else {
          if ((strlen(cwd)+strlen(s)+2) < PATH_MAX) {
            if (strlen(cwd)!=1) strcat(cwd, "/");
            strcat(cwd, s);
          }
        }
      }
      ::chdir(cwd);
    } else {
      ::chdir(s);
    }
    
  }
  return ret;
}

char *dirlst::getprevdir()
{
  return prevdir;
}

//
// определение позиции предыдущей директории в листе
//
int dirlst::prevdirpos()
{
  if (files == 0) return 0;
  if (prevdir == NULL) return 0;
  for (int i = 0; i < files; i++)
    if (strcmp((fi+i)->fname, prevdir) == 0) return i;
  return 0;
}

char *dirlst::getname()
{
  if (cfi->sname != NULL) return cfi->sname;
  return cfi->fname;
}
