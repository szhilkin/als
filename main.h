#ifndef __MAIN_H__
#define __MAIN_H__

#include "trm.h"

// типы файлов для cmdstring

#define FT_ANY		0
#define FT_DIR		0x1
#define FT_LINK		0x2
#define FT_EXEC		0x4

#define CLOCK_INTERVAL	1

struct attrib {
  int attr;
  int fg;
  int bg;
};

class cmdstring {
  private:
    int key;		// код клавиши
	int tag;		// выполнять только для помеченных
	int ftypes;		// выполнять только для указанных типов файлов
	int now;		// немедленное выполнение без редактирования cmd
	int prep;		// требуется ли дополнительный препроцессинг
	char *fmask;	// выполнять только для файлов по маске
	char *cmd;		// строка до препроцессинга
  public:
	int getkey() { return key; }
	int gettag() { return tag; }
	int getftypes() { return ftypes; }
	int getnow() { return now; }
	int getprep() { return prep; }
	char *getfmask() { return fmask; }
	char *getcmd() { return cmd; }
	cmdstring(int k, int tg, int ft, int n, int p, char *fm, char *cm);
	~cmdstring();
};

class alsconfig
{
  private:
    trm *term;
	char *username;		// login
	char *hostname;		// host
	int ok;
  public:
	int maxlines;
	int ansicolor;
	int colordirs;
	int clock;			// show clock or not
	int flinks;			// follow symlinks
	int hidden;			// show / hide hidden files (with .)

	struct attrib a_normal, a_help, a_marker, a_dir, a_link,
				  a_exec, a_err, a_tag;
	cmdstring *cmdstr[512];		// keyborad commands
	int cmdcount;				

	void addfunc(int k, int tg, int ft, int n, int p, char *fm, char *cm);
	int readrc(char *fn);
	void setattr(struct attrib *at, char *a, char *f, char *b);
	void setkey(char *k, char *m, char *c, char *f);
	char *gethostname() { return hostname; }
	char *getusername() { return username; }
	int isgood() { return ok; }

	alsconfig(trm *t);
	~alsconfig();	
};

#endif
