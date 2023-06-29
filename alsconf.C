#include "main.h"
#include "trm.h"
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>

alsconfig::alsconfig(trm *t)
{
  hostname = new char[MAXHOSTNAMELEN];
  ::gethostname(hostname,MAXHOSTNAMELEN);
  char *p = hostname;
  for (int i = 0; i < strlen(hostname); i++,p++)
    if ((char)*p == '.') { *p = 0; break; }
  uid_t uid = getuid();
  struct passwd *pwd = getpwuid(uid);
  username = new char[strlen(pwd->pw_name)];
  strcpy(username, pwd->pw_name);

  term = t;
  // установки по умолчанию
  maxlines = 10;
  ansicolor = 0;
  colordirs = 0;
  clock = 1;
  flinks = 0;
  hidden = 1;
  a_normal.attr = A_NORM; a_normal.fg = C_WHITE; a_normal.bg = C_BLACK;
  a_help.attr = A_REVERSE; a_help.fg = C_CYAN; a_help.bg = C_BLACK;
  a_marker.attr = A_REVERSE; a_marker.fg = C_WHITE; a_marker.bg = C_BLACK;
  a_dir.attr = A_BOLD; a_dir.fg = C_WHITE; a_dir.bg = C_BLACK;
  a_link.attr = A_BOLD; a_link.fg = C_CYAN; a_link.bg = C_BLACK;
  a_exec.attr = A_BOLD; a_exec.fg = C_GREEN; a_exec.bg = C_BLACK;
  a_err.attr = A_BOLD; a_err.fg = C_RED; a_err.bg = C_BLACK;
  a_tag.attr = A_BOLD; a_tag.fg = C_YELLOW; a_tag.bg = C_BLACK;
  cmdcount = 0;
  ok = 1;
  int src=1,urc=1;
  // разборка rc файлов
  // system /etc/alsrc
  if (!readrc("/etc/alsrc")) src = 0;
  // user ~/.alsrc 
  char fn[PATH_MAX];
  strcpy(fn, pwd->pw_dir); strcat(fn, "/.alsrc");
  if (!readrc(fn)) urc = 0;
  if (!src && !urc) ok = 0;
}

alsconfig::~alsconfig()
{
  delete hostname;
  delete username;
  for (int i = 0; i < cmdcount; i++)
    delete cmdstr[i];
}

void alsconfig::addfunc(int k, int tg, int ft, int n, int p, char *fm, char *cm)
{
  cmdstr[cmdcount] = new cmdstring(k, tg, ft, n, p, fm, cm);
  ++cmdcount;
  if (cmdcount > 512) --cmdcount;
}

cmdstring::cmdstring(int k, int tg, int ft, int n, int p, char *fm, char *cm)
{
  fmask = cmd = NULL;
  if (fm != NULL) { fmask = new char[strlen(fm)]; strcpy(fmask, fm); }
  if (cm != NULL) { cmd = new char[strlen(cm)]; strcpy(cmd, cm); }
  key = k; tag = tg; now = n; ftypes = ft; prep = p;
}

cmdstring::~cmdstring()
{
  if (fmask != NULL) delete fmask;
  if (cmd != NULL) delete cmd;
}

//
// чтение файла конфигурации и установка необходимых параметров
//

int alsconfig::readrc(char *fn)
{
  int ret = 1;
  char s[255], *sp;
  FILE *rc;
  char *p;
  char *pc[4];

  rc = fopen(fn, "r");
  if (rc == NULL) return 0; 
  while (fgets(s, 255, rc) != NULL) {
    if (strncmp(s, "#", 1)==0) continue;
    if (strncmp(s, "\n", 1) == 0) continue;
    sp = &s[0];
    if ((p = strsep(&sp, ":")) == NULL) continue;
    //
    // проверка общих параметров
    //
    if (strncmp(p, "maxlines", 8)==0)  
      if ((p=strsep(&sp, ":")) != NULL) {
        maxlines = atoi(p);
        if (maxlines < 3) maxlines = 3;
        if (maxlines > (term->li()-3)) maxlines = term->li()-3;
        continue;
      }
    if (strncmp(p, "ansicolor", 9)==0)
      if ((p=strsep(&sp, ":"))!=NULL) {
        ansicolor = (strncmp(p,"yes",3)==0) ? 1 : 0; 
        continue; 
      }
    if (strncmp(p, "colordirs", 9)==0)
      if ((p=strsep(&sp, ":"))!=NULL) {
        colordirs = (strncmp(p,"yes",3)==0) ? 1 : 0;
        continue;
      }
    if (strncmp(p, "clock", 5)==0)
      if ((p=strsep(&sp, ":"))!=NULL) {
        clock = (strncmp(p,"yes",3)==0) ? 1 : 0;
	    continue;
      }
    if (strncmp(p, "flinks", 6)==0)
      if ((p=strsep(&sp, ":"))!=NULL) {
        flinks = (strncmp(p,"yes",3)==0) ? 1 : 0;
	    continue;
      }
    if (strncmp(p, "hidden", 6)==0)
      if ((p=strsep(&sp, ":"))!=NULL) {
        hidden = (strncmp(p,"yes",3)==0) ? 1 : 0;
	    continue;
      }
    //
    // цвета и атрибуты
    //
    if (strncmp(p, "a_normal", 8)==0) {
      pc[0] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3] = p;
      setattr(&a_normal, pc[1], pc[2], pc[3]);
    }
    if (strncmp(p, "a_help", 6)==0) {
      pc[0] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3] = p;
      setattr(&a_help, pc[1], pc[2], pc[3]);
    }
    if (strncmp(p, "a_marker", 8)==0) {
      pc[0] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3] = p;
      setattr(&a_marker, pc[1], pc[2], pc[3]);
    }
    if (strncmp(p, "a_dir", 5)==0) {
      pc[0] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3] = p;
      setattr(&a_dir, pc[1], pc[2], pc[3]);
    }
    if (strncmp(p, "a_link", 6)==0) {
      pc[0] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3] = p;
      setattr(&a_link, pc[1], pc[2], pc[3]);
    }
    if (strncmp(p, "a_exec", 6)==0) {
      pc[0] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3] = p;
      setattr(&a_exec, pc[1], pc[2], pc[3]);
    }
    if (strncmp(p, "a_err", 5)==0) {
      pc[0] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3] = p;
      setattr(&a_err, pc[1], pc[2], pc[3]);
    }
    if (strncmp(p, "a_tag", 5)==0) {
      pc[0] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2] = p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3] = p;
      setattr(&a_tag, pc[1], pc[2], pc[3]);
    }
    //
    // клавиши
    //
    if (strncmp(p, "K_", 2)==0) {
      pc[0]=p;
      if ((p=strsep(&sp,":"))!=NULL) pc[1]=p;
      if ((p=strsep(&sp,":"))!=NULL) pc[2]=p;
      if ((p=strsep(&sp,":"))!=NULL) pc[3]=p;
      //printf("(%s)(%s)(%s)(%s)\n",pc[0],pc[1],pc[2],pc[3]); 
      setkey(pc[0],pc[1],pc[2],pc[3]);
    }
 //   printf("%s", s);
  }
  fclose(rc);  

  return ret;
}

void alsconfig::setattr(struct attrib *at, char *a, char *f, char *b)
{
  if (strncmp(a,"normal",6)==0) at->attr=A_NORM;
  else if (strncmp(a,"bold",4)==0) at->attr=A_BOLD;
  else if (strncmp(a,"reverse",7)==0) at->attr=A_REVERSE; 
  
  if (strncmp(f,"black",5)==0) at->fg=C_BLACK;
  else if (strncmp(f,"red",3)==0) at->fg=C_RED;
  else if (strncmp(f,"green",5)==0) at->fg=C_GREEN;
  else if (strncmp(f,"yellow",6)==0) at->fg=C_YELLOW;
  else if (strncmp(f,"blue",4)==0) at->fg=C_BLUE;
  else if (strncmp(f,"magenta",7)==0) at->fg=C_MAGENTA;
  else if (strncmp(f,"cyan",4)==0) at->fg=C_CYAN;
  else if (strncmp(f,"white",5)==0) at->fg=C_WHITE;

  if (strncmp(b,"black",5)==0) at->bg=C_BLACK;
  else if (strncmp(b,"red",3)==0) at->bg=C_RED;
  else if (strncmp(b,"green",5)==0) at->bg=C_GREEN;
  else if (strncmp(b,"yellow",6)==0) at->bg=C_YELLOW;
  else if (strncmp(b,"blue",4)==0) at->bg=C_BLUE;
  else if (strncmp(b,"magenta",7)==0) at->bg=C_MAGENTA;
  else if (strncmp(b,"cyan",4)==0) at->bg=C_CYAN;
  else if (strncmp(b,"white",5)==0) at->bg=C_WHITE;
}

void alsconfig::setkey(char *k, char *m, char *c, char *f)
{
  int found=0;
  char t;
  int f_now,f_prep,f_mark;
  int key;
  int ftype=FT_ANY;

  f_now=f_prep=f_mark=0;

  for (int i=0; i<NUMKEYS; i++) {
    if ((strcmp(k,term->keyname(i))==0)) { found=1; key=i; break; }
  }
  if (found) {  // если клавиша найдена
    if (strlen(m)==0) m=NULL;
    for (int i=0; i<strlen(f); i++) {
      t=f[i];
      switch (t) {
        case 'd': ftype |= FT_DIR; break;
        case 'l': ftype |= FT_LINK; break;
        case 'x': ftype |= FT_EXEC; break;
        case '!': f_now=1; break;
        case 'p': f_prep=1; break;
        case '+': f_mark=1; break;
      }
    }
    addfunc(key, f_mark, ftype, f_now, f_prep, m, c);
  }
}

