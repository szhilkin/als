#ifndef TRM_H
#define TRM_H

#include "tty.h"

#define COLUMNS			"co"
#define LINES			"li"
#define MAX_COLORS		"Co"
#define MAX_PAIRS		"pa"
#define MAX_ATTRIBUTES		"ma"

#define BACK_TAB		"bt"
#define BELL			"bl"
#define CARRIAGE_RETURN		"cr"
#define CLEAR_SCREEN		"cl"
#define CLR_BOL			"cb"
#define CLR_EOL			"ce"
#define CLR_EOS			"cd"
#define CURSOR_ADDRESS		"cm"
#define CURSOR_DOWN		"do"
#define CURSOR_HOME		"ho"
#define CURSOR_INVISIBLE	"vi"
#define CURSOR_LEFT		"le"
#define CURSOR_NORMAL		"ve"
#define CURSOR_RIGHT		"nd"
#define CURSOR_TO_LL		"ll"
#define CURSOR_UP		"up"
#define CURSOR_VISIBLE		"vs"
#define DELETE_CHARACTER	"dc"
#define DELETE_LINE		"dl"
#define ENTER_BOLD_MODE		"md"
#define ENTER_INSERT_MODE	"im"
#define ENTER_REVERSE_MODE	"mr"
#define ENTER_STANDOUT_MODE	"so"
#define EXIT_INSERT_MODE	"ei"
#define EXIT_STANDOUT_MODE	"se"
#define EXIT_ATTRIBUTE_MODE	"me"
#define INITIALIZE_COLOR	"Ic"
#define INITIALIZE_PAIR		"Ip"
#define INSERT_CHARACTER	"ic"
#define INSERT_LINE		"al"
#define KEY_BACKSPACE		"kb"
#define KEY_BTAB		"kB"
#define KEY_DC			"kD"
#define KEY_DOWN		"kd"
#define KEY_END			"@7"
#define KEY_ENTER		"@8"
#define KEY_F1			"k1"
#define KEY_F10			"k;"
#define KEY_F11			"F1"
#define KEY_F12			"F2"
#define KEY_F2			"k2"
#define KEY_F3			"k3"
#define KEY_F4			"k4"
#define KEY_F5			"k5"
#define KEY_F6			"k6"
#define KEY_F7			"k7"
#define KEY_F8			"k8"
#define KEY_F9			"k9"
#define KEY_HOME		"kh"
#define KEY_IC			"kI"
#define KEY_LEFT		"kl"
#define KEY_SDC			"*4"
#define KEY_SHOME		"#2"
#define KEY_SIC			"#3"
#define KEY_SLEFT		"#4"
#define KEY_SRIGHT		"%i"
#define KEY_STAB		"kT"
#define KEY_UP			"ku"
#define META_OFF		"mo"
#define META_ON			"mm"
#define NEWLINE			"nw"
#define ORIG_COLORS		"oc"
#define ORIG_PAIR		"op"
#define RESTORE_CURSOR		"rc"
#define SAVE_CURSOR		"sc"
#define SET_ATTRIBUTES		"sa"
#define SET_BACKGROUND		"Sb"
#define SET_COLOR_PAIR		"sp"
#define SET_FOREGROUND		"Sf"
#define SET_A_FOREGROUND	"AF"
#define SET_A_BACKGROUND	"AB"

#define KEY_PPAGE		"kP"
#define KEY_NPAGE		"kN"
#define KEY_RIGHT		"kr"

// виртуальные клавиши

#define K_ESC		0
#define K_F1		1
#define K_F2		2
#define K_F3		3
#define K_F4		4
#define K_F5		5
#define K_F6		6
#define K_F7		7
#define K_F8		8
#define K_F9		9
#define K_F10		10
#define K_F11		11
#define K_F12		12
#define K_TAB		13
#define K_BSPACE	14
#define K_SPACE		15
#define K_INS		16
#define K_DEL		17
#define	K_HOME		18
#define K_END		19
#define K_PUP		20
#define K_PDN		21
#define K_UP		22
#define K_DOWN		23
#define K_LEFT		24
#define K_RIGHT		25
#define K_ENTER		26
#define K_0	        27
#define K_1		28
#define K_2		29
#define K_3		30
#define K_4		31
#define K_5		32
#define K_6		33
#define K_7		34
#define K_8		35
#define K_9		36
#define K_R		37
#define K_CTRL_L	38
#define K_L		39
#define K_F		40
#define K_Q		41
#define K_W		42
#define K_E		43
#define K_T		44
#define K_Y		45
#define K_U		46
#define K_I		47
#define K_O		48
#define K_P		49
#define K_A		50
#define K_S		51
#define K_D		52
#define K_G		53
#define K_H		54
#define K_J		55
#define K_K		56
#define K_Z		57
#define K_X		58
#define K_C		59
#define K_V		60
#define K_B		61
#define K_N		62
#define K_M		63
#define K_CTRL_W	64
#define K_CTRL_E	65
#define K_CTRL_R	66
#define K_CTRL_T	67
#define K_CTRL_Y	68
#define K_CTRL_U	69
#define K_CTRL_I	70
#define K_CTRL_O	71
#define K_CTRL_P	72
#define K_CTRL_A	73
#define K_CTRL_D	74
#define K_CTRL_F	75
#define K_CTRL_G	76
#define K_CTRL_H	77
#define K_CTRL_J	78
#define K_CTRL_K	79
#define K_CTRL_X	80
#define K_CTRL_V	81
#define K_CTRL_B	82
#define K_CTRL_N	83
#define K_CTRL_M	84

#define K_ALT_Q		85
#define K_ALT_W		86
#define K_ALT_E		87
#define K_ALT_R		88
#define K_ALT_T		89
#define K_ALT_Y		90
#define K_ALT_U		91
#define K_ALT_I		92
#define K_ALT_O		93
#define K_ALT_P		94
#define K_ALT_A		95
#define K_ALT_S		96
#define K_ALT_D		97
#define K_ALT_F		98
#define K_ALT_G		99
#define K_ALT_H		100
#define K_ALT_J		101
#define K_ALT_K		102
#define K_ALT_L		103
#define K_ALT_Z		104
#define K_ALT_X		105
#define K_ALT_C		106
#define K_ALT_V		107
#define K_ALT_B		108
#define K_ALT_N		109
#define K_ALT_M		110
#define K_PLUS		111
#define K_MINUS		112

#define NUMKEYS	113

// цвета

#define C_BLACK		0
#define C_RED		1
#define C_GREEN		2
#define C_YELLOW	3
#define C_BLUE		4
#define C_MAGENTA	5
#define C_CYAN		6
#define C_WHITE		7

// атрибуты для монохрома

#define A_NORM		0
#define A_BOLD		1
#define A_REVERSE	2
 

struct tcaps
{
  int _columns;
  int _lines;
  int _max_colors;
  int _max_pairs;
  int _max_attributes;

  char *_back_tab;
  char *_bell;
  char *_carriage_return;
  char *_clear_screen;
  char *_clr_bol;
  char *_clr_eol;
  char *_clr_eos;
  char *_cursor_address;
  char *_cursor_down;
  char *_cursor_home;
  char *_cursor_invisible;
  char *_cursor_left;
  char *_cursor_normal;
  char *_cursor_right;
  char *_cursor_to_ll;
  char *_cursor_up;
  char *_cursor_visible;
  char *_delete_character;
  char *_delete_line;
  char *_enter_bold_mode;
  char *_enter_insert_mode;
  char *_enter_reverse_mode;
  char *_enter_standout_mode;
  char *_exit_attribute_mode;
  char *_exit_insert_mode;
  char *_exit_standout_mode;
  char *_initialize_color;
  char *_initialize_pair;
  char *_insert_character;
  char *_insert_line;
  char *_key_backspace;
  char *_key_btab;
  char *_key_dc;
  char *_key_down;
  char *_key_end;
  char *_key_enter;
  char *_key_f1;
  char *_key_f10;
  char *_key_f11;
  char *_key_f12;
  char *_key_f2;
  char *_key_f3;
  char *_key_f4;
  char *_key_f5;
  char *_key_f6;
  char *_key_f7;
  char *_key_f8;
  char *_key_f9;
  char *_key_home;
  char *_key_ic;
  char *_key_left;
  char *_key_sdc;
  char *_key_shome;
  char *_key_sic;
  char *_key_sleft;
  char *_key_sright;
  char *_key_stab;
  char *_key_up;
  char *_meta_off;
  char *_meta_on;
  char *_newline;
  char *_orig_colors;
  char *_orig_pair;
  char *_restore_cursor;
  char *_save_cursor;
  char *_set_attributes;
  char *_set_background;
  char *_set_color_pair;
  char *_set_foreground;
  char *_set_a_foreground;
  char *_set_a_background;
  char *_key_ppage;
  char *_key_npage;
  char *_key_right;
};

struct skeys
{
  char esc[7];
  int len;
  char name[10];
};



static struct skeys kb[NUMKEYS];

class trm
{
  private:
  
  tty *ptty;
  char tbuf[2048], str[512], *p;
  char *tname;
  struct tcaps tc;
  char kbuf[32];
  int nrd; // кол-во последних считанных символов
  int err;
  
  int init();
  void setkey(char *k, char *s);
  void define_keys();
  void initkeys();
  
  public:
  
  trm(tty *p);
  ~trm();
  int status() { return err; }
  char *getname() { return tname; }
  int readkey();
  char lastchar();
  int charcount() { return nrd; }
  void openv() { ptty->openv(); }
  void closev() { ptty->closev(); }
  int is_visual() { return ptty->mode(); }
  char getbuf(int i) { return kbuf[i]; }
  void move(int y, int x);
  void print(int y, int x, char *s);
 // void setattr(int attr);
  int has_colors() { return tc._max_colors; }
  void setfg(int c);
  void setbg(int c);
  void setcolor(int fg,int bg);
  void cleos(int y, int x);
  void cleol(int line);
  void bold();
  void normal();
  void reverse();
  char *keyname(int num);
  
  int co() { return tc._columns; }
  int li() { return tc._lines; }

  // тест - перенести в private
  int max_colors() { return tc._max_colors; }
  int max_pairs() { return tc._max_pairs; }
  int max_attributes() { return tc._max_attributes; }

  char *back_tab() { return tc._back_tab; }
  char *bell() { return tc._bell; }
  char *carriage_return() { return tc._carriage_return; }
  char *clear_screen() { return tc._clear_screen; }
  char *clr_bol() { return tc._clr_bol; }
  char *clr_eol() { return tc._clr_eol; }
  char *clr_eos() { return tc._clr_eos; }
  char *cursor_address() { return tc._cursor_address; }
  char *cursor_down() { return tc._cursor_down; }
  char *cursor_home() { return tc._cursor_home; }
  char *cursor_invisible() { return tc._cursor_invisible; }
  char *cursor_left() { return tc._cursor_left; }
  char *cursor_normal() { return tc._cursor_normal; }
  char *cursor_right() { return tc._cursor_right; }
  char *cursor_to_ll() { return tc._cursor_to_ll; }
  char *cursor_up() { return tc._cursor_up; }
  char *cursor_visible() { return tc._cursor_visible; }
  char *delete_character() { return tc._delete_character; }
  char *delete_line() { return tc._delete_line; }
  char *enter_bold_mode() { return tc._enter_bold_mode; }
  char *enter_insert_mode() { return tc._enter_insert_mode; }
  char *enter_reverse_mode() { return tc._enter_reverse_mode; }
  char *enter_standout_mode() { return tc._enter_standout_mode; }
  char *exit_attribute_mode() { return tc._exit_attribute_mode; }
  char *exit_insert_mode() { return tc._exit_insert_mode; }
  char *exit_standout_mode() { return tc._exit_standout_mode; }
  char *initialize_color() { return tc._initialize_color; }
  char *initialize_pair() { return tc._initialize_pair; }
  char *insert_character() { return tc._insert_character; }
  char *insert_line() { return tc._insert_line; }
  char *key_backspace() { return tc._key_backspace; }
  char *key_btab() { return tc._key_btab; }
  char *key_dc() { return tc._key_dc; }
  char *key_down() { return tc._key_down; }
  char *key_end() { return tc._key_end; }
  char *key_enter() { return tc._key_enter; }
  char *key_f1() { return tc._key_f1; }
  char *key_f10() { return tc._key_f10; }
  char *key_f11() { return tc._key_f11; }
  char *key_f12() { return tc._key_f12; }
  char *key_f2() { return tc._key_f2; }
  char *key_f3() { return tc._key_f3; }
  char *key_f4() { return tc._key_f4; }
  char *key_f5() { return tc._key_f5; }
  char *key_f6() { return tc._key_f6; }
  char *key_f7() { return tc._key_f7; }
  char *key_f8() { return tc._key_f8; }
  char *key_f9() { return tc._key_f9; }
  char *key_home() { return tc._key_home; }
  char *key_ic() { return tc._key_ic; }
  char *key_left() { return tc._key_left; }
  char *key_sdc() { return tc._key_sdc; }
  char *key_shome() { return tc._key_shome; }
  char *key_sic() { return tc._key_sic; }
  char *key_sleft() { return tc._key_sleft; }
  char *key_sright() { return tc._key_sright; }
  char *key_stab() { return tc._key_stab; }
  char *key_up() { return tc._key_up; }
  char *meta_off() { return tc._meta_off; }
  char *meta_on() { return tc._meta_on; }
  char *newline() { return tc._newline; }
  char *orig_colors() { return tc._orig_colors; }
  char *orig_pair() { return tc._orig_pair; }
  char *restore_cursor() { return tc._restore_cursor; }
  char *save_cursor() { return tc._save_cursor; }
  char *set_attributes() { return tc._set_attributes; }
  char *set_background() { return tc._set_background; }
  char *set_color_pair() { return tc._set_color_pair; }
  char *set_foreground() { return tc._set_foreground; }
  char *set_a_foreground() { return tc._set_a_foreground; }
  char *set_a_background() { return tc._set_a_background; }
  char *key_ppage() { return tc._key_ppage; }
  char *key_npage() { return tc._key_npage; }
  char *key_right() { return tc._key_right; }
  
};

#endif // TRM_H
