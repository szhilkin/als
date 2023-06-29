#include "trm.h"

#include <term.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>

static int put(int c)
{
  return putchar(c);
}

trm::trm(tty *p)
{
  err = init();
  ptty = p;
  initkeys();
  define_keys();
  ptty->openv();
}

trm::~trm()
{
  ptty->closev();
}

int trm::init()
{
  p = str;
  switch (tgetent(tbuf, tname = getenv("TERM"))) {
    case -1: return 1;
    case 0:  return 2;
    case 1:  break;
  }

  tc._columns = tgetnum(COLUMNS);
  tc._lines = tgetnum(LINES);
  tc._max_colors = tgetnum(MAX_COLORS);
  tc._max_pairs = tgetnum(MAX_PAIRS);
  tc._max_attributes = tgetnum(MAX_ATTRIBUTES);

  tc._back_tab = tgetstr(BACK_TAB, &p);
  tc._bell = tgetstr(BELL, &p);
  tc._carriage_return = tgetstr(CARRIAGE_RETURN, &p);
  tc._clear_screen = tgetstr(CLEAR_SCREEN, &p);
  tc._clr_bol = tgetstr(CLR_BOL, &p);
  tc._clr_eol = tgetstr(CLR_EOL, &p);
  tc._clr_eos = tgetstr(CLR_EOS, &p);
  tc._cursor_address = tgetstr(CURSOR_ADDRESS, &p);
  tc._cursor_down = tgetstr(CURSOR_DOWN, &p);
  tc._cursor_home = tgetstr(CURSOR_HOME, &p);
  tc._cursor_invisible = tgetstr(CURSOR_INVISIBLE, &p);
  tc._cursor_left = tgetstr(CURSOR_LEFT, &p);
  tc._cursor_normal = tgetstr(CURSOR_NORMAL, &p);
  tc._cursor_right = tgetstr(CURSOR_RIGHT, &p);
  tc._cursor_to_ll = tgetstr(CURSOR_TO_LL, &p);
  tc._cursor_up = tgetstr(CURSOR_UP, &p);
  tc._cursor_visible = tgetstr(CURSOR_VISIBLE, &p);
  tc._delete_character = tgetstr(DELETE_CHARACTER, &p);
  tc._delete_line = tgetstr(DELETE_LINE, &p);
  tc._enter_bold_mode = tgetstr(ENTER_BOLD_MODE, &p);
  tc._enter_insert_mode = tgetstr(ENTER_INSERT_MODE, &p);
  tc._enter_reverse_mode = tgetstr(ENTER_REVERSE_MODE, &p);
  tc._enter_standout_mode = tgetstr(ENTER_STANDOUT_MODE, &p);
  tc._exit_attribute_mode = tgetstr(EXIT_ATTRIBUTE_MODE, &p);
  tc._exit_insert_mode = tgetstr(EXIT_INSERT_MODE, &p);
  tc._exit_standout_mode = tgetstr(EXIT_STANDOUT_MODE, &p);
  tc._initialize_color = tgetstr(INITIALIZE_COLOR, &p);
  tc._initialize_pair = tgetstr(INITIALIZE_PAIR, &p);
  tc._insert_character = tgetstr(INSERT_CHARACTER, &p);
  tc._insert_line = tgetstr(INSERT_LINE, &p);
  tc._key_backspace = tgetstr(KEY_BACKSPACE, &p);
  tc._key_btab = tgetstr(KEY_BTAB, &p);
  tc._key_dc = tgetstr(KEY_DC, &p);
  tc._key_down = tgetstr(KEY_DOWN, &p);
  tc._key_end = tgetstr(KEY_END, &p);
  tc._key_enter = tgetstr(KEY_ENTER, &p);
  tc._key_f1 = tgetstr(KEY_F1, &p);
  tc._key_f10 = tgetstr(KEY_F10, &p);
  tc._key_f11 = tgetstr(KEY_F11, &p);
  tc._key_f12 = tgetstr(KEY_F12, &p);
  tc._key_f2 = tgetstr(KEY_F2, &p);
  tc._key_f3 = tgetstr(KEY_F3, &p);
  tc._key_f4 = tgetstr(KEY_F4, &p);
  tc._key_f5 = tgetstr(KEY_F5, &p);
  tc._key_f6 = tgetstr(KEY_F6, &p);
  tc._key_f7 = tgetstr(KEY_F7, &p);
  tc._key_f8 = tgetstr(KEY_F8, &p);
  tc._key_f9 = tgetstr(KEY_F9, &p);
  tc._key_home = tgetstr(KEY_HOME, &p);
  tc._key_ic = tgetstr(KEY_IC, &p);
  tc._key_left = tgetstr(KEY_LEFT, &p);
  tc._key_sdc = tgetstr(KEY_SDC, &p);
  tc._key_shome = tgetstr(KEY_SHOME, &p);
  tc._key_sic = tgetstr(KEY_SIC, &p);
  tc._key_sleft = tgetstr(KEY_SLEFT, &p);
  tc._key_sright = tgetstr(KEY_SRIGHT, &p);
  tc._key_stab = tgetstr(KEY_STAB, &p);
  tc._key_up = tgetstr(KEY_UP, &p);
  tc._meta_off = tgetstr(META_OFF, &p);
  tc._meta_on = tgetstr(META_ON, &p);
  tc._newline = tgetstr(NEWLINE, &p);
  tc._orig_colors = tgetstr(ORIG_COLORS, &p);
  tc._orig_pair = tgetstr(ORIG_PAIR, &p);
  tc._restore_cursor = tgetstr(RESTORE_CURSOR, &p);
  tc._save_cursor = tgetstr(SAVE_CURSOR, &p);
  tc._set_attributes = tgetstr(SET_ATTRIBUTES, &p);
  tc._set_background = tgetstr(SET_BACKGROUND, &p);
  tc._set_color_pair = tgetstr(SET_COLOR_PAIR, &p);
  tc._set_foreground = tgetstr(SET_FOREGROUND, &p);
  tc._set_a_foreground = tgetstr(SET_A_FOREGROUND, &p);
  tc._set_a_background = tgetstr(SET_A_BACKGROUND, &p);
  tc._key_ppage = tgetstr(KEY_PPAGE, &p);
  tc._key_npage = tgetstr(KEY_NPAGE, &p);
  tc._key_right = tgetstr(KEY_RIGHT, &p);
}

//
// load keytable with default values
//

void trm::initkeys()
{
  strcpy(kb[K_ESC].name, "K_ESC"); kb[K_ESC].len = 0;
  strcpy(kb[K_TAB].name, "K_TAB"); kb[K_TAB].len = 0;
  strcpy(kb[K_BSPACE].name, "K_BSPACE"); kb[K_BSPACE].len = 0;
  strcpy(kb[K_SPACE].name, "K_SPACE"); kb[K_SPACE].len = 0;
  strcpy(kb[K_ENTER].name, "K_ENTER"); kb[K_ENTER].len = 0;
  strcpy(kb[K_F1].name, "K_F1"); kb[K_F1].len = 0;
  strcpy(kb[K_F2].name, "K_F2"); kb[K_F2].len = 0;
  strcpy(kb[K_F3].name, "K_F3"); kb[K_F3].len = 0;
  strcpy(kb[K_F4].name, "K_F4"); kb[K_F4].len = 0;
  strcpy(kb[K_F5].name, "K_F5"); kb[K_F5].len = 0;
  strcpy(kb[K_F6].name, "K_F6"); kb[K_F6].len = 0;
  strcpy(kb[K_F7].name, "K_F7"); kb[K_F7].len = 0;
  strcpy(kb[K_F8].name, "K_F8"); kb[K_F8].len = 0;
  strcpy(kb[K_F9].name, "K_F9"); kb[K_F9].len = 0;
  strcpy(kb[K_F10].name, "K_F10"); kb[K_F10].len = 0;
  strcpy(kb[K_F11].name, "K_F11"); kb[K_F11].len = 0;
  strcpy(kb[K_F12].name, "K_F12"); kb[K_F12].len = 0;
  strcpy(kb[K_0].name, "K_0"); kb[K_0].len = 0;
  strcpy(kb[K_1].name, "K_1"); kb[K_1].len = 0;
  strcpy(kb[K_2].name, "K_2"); kb[K_2].len = 0;
  strcpy(kb[K_3].name, "K_3"); kb[K_3].len = 0;
  strcpy(kb[K_4].name, "K_4"); kb[K_4].len = 0;
  strcpy(kb[K_5].name, "K_5"); kb[K_5].len = 0;
  strcpy(kb[K_6].name, "K_6"); kb[K_6].len = 0;
  strcpy(kb[K_7].name, "K_7"); kb[K_7].len = 0;
  strcpy(kb[K_8].name, "K_8"); kb[K_8].len = 0;
  strcpy(kb[K_9].name, "K_9"); kb[K_9].len = 0;
  strcpy(kb[K_R].name, "K_R"); kb[K_R].len = 0;
  strcpy(kb[K_CTRL_L].name, "K_CTRL_L"); kb[K_CTRL_L].len = 0;
  strcpy(kb[K_L].name, "K_L"); kb[K_L].len = 0;
  strcpy(kb[K_F].name, "K_F"); kb[K_F].len = 0;
  strcpy(kb[K_Q].name, "K_Q"); kb[K_Q].len = 0;
  strcpy(kb[K_W].name, "K_W"); kb[K_W].len = 0;
  strcpy(kb[K_E].name, "K_E"); kb[K_E].len = 0;
  strcpy(kb[K_T].name, "K_T"); kb[K_T].len = 0;
  strcpy(kb[K_Y].name, "K_Y"); kb[K_Y].len = 0;
  strcpy(kb[K_U].name, "K_U"); kb[K_U].len = 0;
  strcpy(kb[K_I].name, "K_I"); kb[K_I].len = 0;
  strcpy(kb[K_O].name, "K_O"); kb[K_O].len = 0;
  strcpy(kb[K_P].name, "K_P"); kb[K_P].len = 0;
  strcpy(kb[K_A].name, "K_A"); kb[K_A].len = 0;
  strcpy(kb[K_S].name, "K_S"); kb[K_S].len = 0;
  strcpy(kb[K_D].name, "K_D"); kb[K_D].len = 0;
  strcpy(kb[K_G].name, "K_G"); kb[K_G].len = 0;
  strcpy(kb[K_H].name, "K_H"); kb[K_H].len = 0;
  strcpy(kb[K_J].name, "K_J"); kb[K_J].len = 0;
  strcpy(kb[K_K].name, "K_K"); kb[K_K].len = 0;
  strcpy(kb[K_Z].name, "K_Z"); kb[K_Z].len = 0;
  strcpy(kb[K_X].name, "K_X"); kb[K_X].len = 0;
  strcpy(kb[K_C].name, "K_C"); kb[K_C].len = 0;
  strcpy(kb[K_V].name, "K_V"); kb[K_V].len = 0;
  strcpy(kb[K_B].name, "K_B"); kb[K_B].len = 0;
  strcpy(kb[K_N].name, "K_N"); kb[K_N].len = 0;
  strcpy(kb[K_M].name, "K_M"); kb[K_M].len = 0;
  strcpy(kb[K_CTRL_W].name, "K_CTRL_W"); kb[K_CTRL_W].len = 0;
  strcpy(kb[K_CTRL_E].name, "K_CTRL_E"); kb[K_CTRL_E].len = 0;
  strcpy(kb[K_CTRL_R].name, "K_CTRL_R"); kb[K_CTRL_R].len = 0;
  strcpy(kb[K_CTRL_T].name, "K_CTRL_T"); kb[K_CTRL_T].len = 0;
  strcpy(kb[K_CTRL_Y].name, "K_CTRL_Y"); kb[K_CTRL_Y].len = 0;
  strcpy(kb[K_CTRL_U].name, "K_CTRL_U"); kb[K_CTRL_U].len = 0;
  strcpy(kb[K_CTRL_I].name, "K_CTRL_I"); kb[K_CTRL_I].len = 0;
  strcpy(kb[K_CTRL_O].name, "K_CTRL_O"); kb[K_CTRL_O].len = 0;
  strcpy(kb[K_CTRL_P].name, "K_CTRL_P"); kb[K_CTRL_P].len = 0;
  strcpy(kb[K_CTRL_A].name, "K_CTRL_A"); kb[K_CTRL_A].len = 0;
  strcpy(kb[K_CTRL_D].name, "K_CTRL_D"); kb[K_CTRL_D].len = 0;
  strcpy(kb[K_CTRL_F].name, "K_CTRL_F"); kb[K_CTRL_F].len = 0;
  strcpy(kb[K_CTRL_G].name, "K_CTRL_G"); kb[K_CTRL_G].len = 0;
  strcpy(kb[K_CTRL_H].name, "K_CTRL_H"); kb[K_CTRL_H].len = 0;
  strcpy(kb[K_CTRL_J].name, "K_CTRL_J"); kb[K_CTRL_J].len = 0;
  strcpy(kb[K_CTRL_K].name, "K_CTRL_K"); kb[K_CTRL_K].len = 0;
  strcpy(kb[K_CTRL_X].name, "K_CTRL_X"); kb[K_CTRL_X].len = 0;
  strcpy(kb[K_CTRL_V].name, "K_CTRL_V"); kb[K_CTRL_V].len = 0;
  strcpy(kb[K_CTRL_B].name, "K_CTRL_B"); kb[K_CTRL_B].len = 0;
  strcpy(kb[K_CTRL_N].name, "K_CTRL_N"); kb[K_CTRL_N].len = 0;
  strcpy(kb[K_CTRL_M].name, "K_CTRL_M"); kb[K_CTRL_M].len = 0;
  strcpy(kb[K_ALT_Q].name, "K_ALT_Q"); kb[K_ALT_Q].len = 0;
  strcpy(kb[K_ALT_W].name, "K_ALT_W"); kb[K_ALT_W].len = 0;
  strcpy(kb[K_ALT_E].name, "K_ALT_E"); kb[K_ALT_E].len = 0;
  strcpy(kb[K_ALT_R].name, "K_ALT_R"); kb[K_ALT_R].len = 0;
  strcpy(kb[K_ALT_T].name, "K_ALT_T"); kb[K_ALT_T].len = 0;
  strcpy(kb[K_ALT_Y].name, "K_ALT_Y"); kb[K_ALT_Y].len = 0;
  strcpy(kb[K_ALT_U].name, "K_ALT_U"); kb[K_ALT_U].len = 0;
  strcpy(kb[K_ALT_I].name, "K_ALT_I"); kb[K_ALT_I].len = 0;
  strcpy(kb[K_ALT_O].name, "K_ALT_O"); kb[K_ALT_O].len = 0;
  strcpy(kb[K_ALT_P].name, "K_ALT_P"); kb[K_ALT_P].len = 0;
  strcpy(kb[K_ALT_A].name, "K_ALT_A"); kb[K_ALT_A].len = 0;
  strcpy(kb[K_ALT_S].name, "K_ALT_S"); kb[K_ALT_S].len = 0;
  strcpy(kb[K_ALT_D].name, "K_ALT_D"); kb[K_ALT_D].len = 0;
  strcpy(kb[K_ALT_F].name, "K_ALT_F"); kb[K_ALT_F].len = 0;
  strcpy(kb[K_ALT_G].name, "K_ALT_G"); kb[K_ALT_G].len = 0;
  strcpy(kb[K_ALT_H].name, "K_ALT_H"); kb[K_ALT_H].len = 0;
  strcpy(kb[K_ALT_J].name, "K_ALT_J"); kb[K_ALT_J].len = 0;
  strcpy(kb[K_ALT_K].name, "K_ALT_K"); kb[K_ALT_K].len = 0;
  strcpy(kb[K_ALT_L].name, "K_ALT_L"); kb[K_ALT_L].len = 0;
  strcpy(kb[K_ALT_Z].name, "K_ALT_Z"); kb[K_ALT_Z].len = 0;
  strcpy(kb[K_ALT_X].name, "K_ALT_X"); kb[K_ALT_X].len = 0;
  strcpy(kb[K_ALT_C].name, "K_ALT_C"); kb[K_ALT_C].len = 0;
  strcpy(kb[K_ALT_V].name, "K_ALT_V"); kb[K_ALT_V].len = 0;
  strcpy(kb[K_ALT_B].name, "K_ALT_B"); kb[K_ALT_B].len = 0;
  strcpy(kb[K_ALT_N].name, "K_ALT_N"); kb[K_ALT_N].len = 0;
  strcpy(kb[K_ALT_M].name, "K_ALT_M"); kb[K_ALT_M].len = 0;
  strcpy(kb[K_PLUS].name, "K_PLUS"); kb[K_PLUS].len = 0;
  strcpy(kb[K_MINUS].name, "K_MINUS"); kb[K_MINUS].len = 0;
  strcpy(kb[K_INS].name, "K_INS"); kb[K_INS].len = 0;
  strcpy(kb[K_DEL].name, "K_DEL"); kb[K_DEL].len = 0;
  strcpy(kb[K_HOME].name, "K_HOME"); kb[K_HOME].len = 0;
  strcpy(kb[K_END].name, "K_END"); kb[K_END].len = 0;
  strcpy(kb[K_UP].name, "K_UP"); kb[K_UP].len = 0;
  strcpy(kb[K_DOWN].name, "K_DOWN"); kb[K_DOWN].len = 0;
  strcpy(kb[K_LEFT].name, "K_LEFT"); kb[K_LEFT].len = 0;
  strcpy(kb[K_PUP].name, "K_PUP"); kb[K_PUP].len = 0;
  strcpy(kb[K_PDN].name, "K_PDN"); kb[K_PDN].len = 0;
  strcpy(kb[K_RIGHT].name, "K_RIGHT"); kb[K_RIGHT].len = 0;
}

int trm::readkey()
{
  nrd = read(0, kbuf, sizeof(kbuf));
  kbuf[nrd] = 0;
  //char s[50];
  //sprintf(s, "%d %d %d %d", kbuf[0], kbuf[1], kbuf[2], kbuf[3]);
  //print(1,1,s);
  for (int i=0; i < NUMKEYS; i++) {
    if (nrd == kb[i].len)
      if (strcmp(kb[i].esc, kbuf) == 0) return i;
  }
  return -1;
}
 
char trm::lastchar()
{
  int i = nrd - 1;
  if (i < 0) i = 0;
  return kbuf[i];
}

// set escape seq for key k

void trm::setkey(char *k, char *s)
{
  for (int i=0; i < NUMKEYS; i++) {
    if (strcmp(kb[i].name,k) == 0) {
      strcpy(kb[i].esc, s);
      kb[i].len = strlen(kb[i].esc);
  //    printf("%s:%d %d\n",kb[i].name,kb[i].esc[0],kb[i].esc[1]);
    }
  }
}

void trm::define_keys()
{
  char s[7];
  char *pt = &s[0];

  //
  // Three stages of loading keys:
  // 1) Default keys for linux console
  // 2) Read key defininitions from termcap/terminfo
  // 3) Load keys for current terminal from /etc/alsterm.$TERM
  // 4) Load keys for current terminal from ~/.alsterm.$TERM 
  //

  // default key values
  s[0] = '\033'; s[1] = 0; setkey("K_ESC", pt);
  s[0] = 0x9; setkey("K_TAB", pt);
  s[0] = 0x7f; setkey("K_BSPACE", pt);
  s[0] = 0x20; setkey("K_SPACE", pt);
  s[0] = 0xd; setkey("K_ENTER", pt);
  s[0] = 0x30; setkey("K_0", pt);
  s[0] = 0x31; setkey("K_1", pt);
  s[0] = 0x32; setkey("K_2", pt);
  s[0] = 0x33; setkey("K_3", pt);
  s[0] = 0x34; setkey("K_4", pt);
  s[0] = 0x35; setkey("K_5", pt);
  s[0] = 0x36; setkey("K_6", pt);
  s[0] = 0x37; setkey("K_7", pt);
  s[0] = 0x38; setkey("K_8", pt);
  s[0] = 0x39; setkey("K_9", pt);
  s[0] = 0x52; setkey("K_R", pt);
  s[0] = 0xc; setkey("K_CTRL_L", pt);
  s[0] = 0x4c; setkey("K_L", pt);
  s[0] = 0x46; setkey("K_F", pt);
  s[0] = 0x51; setkey("K_Q", pt);
  s[0] = 0x57; setkey("K_W", pt);
  s[0] = 0x45; setkey("K_E", pt);
  s[0] = 0x54; setkey("K_T", pt);
  s[0] = 0x59; setkey("K_Y", pt);
  s[0] = 0x55; setkey("K_U", pt);
  s[0] = 0x49; setkey("K_I", pt);
  s[0] = 0x4f; setkey("K_O", pt);
  s[0] = 0x50; setkey("K_P", pt);
  s[0] = 0x41; setkey("K_A", pt);
  s[0] = 0x53; setkey("K_S", pt);
  s[0] = 0x44; setkey("K_D", pt);
  s[0] = 0x47; setkey("K_G", pt);
  s[0] = 0x48; setkey("K_H", pt);
  s[0] = 0x4a; setkey("K_J", pt);
  s[0] = 0x4b; setkey("K_K", pt);
  s[0] = 0x5a; setkey("K_Z", pt);
  s[0] = 0x58; setkey("K_X", pt);
  s[0] = 0x43; setkey("K_C", pt);
  s[0] = 0x56; setkey("K_V", pt);
  s[0] = 0x42; setkey("K_B", pt);
  s[0] = 0x4e; setkey("K_N", pt);
  s[0] = 0x4d; setkey("K_M", pt);
  s[0] = 0x17; setkey("K_CTRL_W", pt);
  s[0] = 0x05; setkey("K_CTRL_E", pt);
  s[0] = 0x12; setkey("K_CTRL_R", pt);
  s[0] = 0x14; setkey("K_CTRL_T", pt);
  s[0] = 0x19; setkey("K_CTRL_Y", pt);
  s[0] = 0x15; setkey("K_CTRL_U", pt);
  s[0] = 0x09; setkey("K_CTRL_I", pt);
  s[0] = 0x0f; setkey("K_CTRL_O", pt);
  s[0] = 0x10; setkey("K_CTRL_P", pt);
  s[0] = 0x01; setkey("K_CTRL_A", pt);
  s[0] = 0x04; setkey("K_CTRL_D", pt);
  s[0] = 0x06; setkey("K_CTRL_F", pt);
  s[0] = 0x07; setkey("K_CTRL_G", pt);
  s[0] = 0x08; setkey("K_CTRL_H", pt);
  s[0] = 0x0a; setkey("K_CTRL_J", pt);
  s[0] = 0x0b; setkey("K_CTRL_K", pt);
  s[0] = 0x18; setkey("K_CTRL_X", pt);
  s[0] = 0x16; setkey("K_CTRL_V", pt);
  s[0] = 0x02; setkey("K_CTRL_B", pt);
  s[0] = 0x0e; setkey("K_CTRL_N", pt);
  s[0] = 0x0a; setkey("K_CTRL_M", pt);

  s[0] = 0x1b; s[1] = 0x71; s[2] = 0; setkey("K_ALT_Q", pt);
  s[0] = 0x1b; s[1] = 0x77; setkey("K_ALT_W", pt);
  s[0] = 0x1b; s[1] = 0x65; setkey("K_ALT_E", pt);
  s[0] = 0x1b; s[1] = 0x72; setkey("K_ALT_R", pt);
  s[0] = 0x1b; s[1] = 0x74; setkey("K_ALT_T", pt);
  s[0] = 0x1b; s[1] = 0x79; setkey("K_ALT_Y", pt);
  s[0] = 0x1b; s[1] = 0x75; setkey("K_ALT_U", pt);
  s[0] = 0x1b; s[1] = 0x69; setkey("K_ALT_I", pt);
  s[0] = 0x1b; s[1] = 0x6f; setkey("K_ALT_O", pt);
  s[0] = 0x1b; s[1] = 0x70; setkey("K_ALT_P", pt);
  s[0] = 0x1b; s[1] = 0x61; setkey("K_ALT_A", pt);
  s[0] = 0x1b; s[1] = 0x73; setkey("K_ALT_S", pt);
  s[0] = 0x1b; s[1] = 0x64; setkey("K_ALT_D", pt);
  s[0] = 0x1b; s[1] = 0x66; setkey("K_ALT_F", pt);
  s[0] = 0x1b; s[1] = 0x67; setkey("K_ALT_G", pt);
  s[0] = 0x1b; s[1] = 0x68; setkey("K_ALT_H", pt);
  s[0] = 0x1b; s[1] = 0x6a; setkey("K_ALT_J", pt);
  s[0] = 0x1b; s[1] = 0x6b; setkey("K_ALT_K", pt);
  s[0] = 0x1b; s[1] = 0x6c; setkey("K_ALT_L", pt);
  s[0] = 0x1b; s[1] = 0x7a; setkey("K_ALT_Z", pt);
  s[0] = 0x1b; s[1] = 0x78; setkey("K_ALT_X", pt);
  s[0] = 0x1b; s[1] = 0x63; setkey("K_ALT_C", pt);
  s[0] = 0x1b; s[1] = 0x76; setkey("K_ALT_V", pt);
  s[0] = 0x1b; s[1] = 0x62; setkey("K_ALT_B", pt);
  s[0] = 0x1b; s[1] = 0x6e; setkey("K_ALT_N", pt);
  s[0] = 0x1b; s[1] = 0x6d; setkey("K_ALT_M", pt);

  s[0] = 0x2b; s[1] = 0; setkey("K_PLUS", pt);
  s[0] = 0x2d; s[1] = 0; setkey("K_MINUS", pt); 

  // values from termcap or terminfo
  if (tc._key_f1) setkey("K_F1", tc._key_f1);
  if (tc._key_f2) setkey("K_F2", tc._key_f2);
  if (tc._key_f3) setkey("K_F3", tc._key_f3);
  if (tc._key_f4) setkey("K_F4", tc._key_f4);
  if (tc._key_f5) setkey("K_F5", tc._key_f5);
  if (tc._key_f6) setkey("K_F6", tc._key_f6);
  if (tc._key_f7) setkey("K_F7", tc._key_f7);
  if (tc._key_f8) setkey("K_F8", tc._key_f8);
  if (tc._key_f9) setkey("K_F9", tc._key_f9);
  if (tc._key_f10) setkey("K_F10", tc._key_f10);
  if (tc._key_f11) setkey("K_F11", tc._key_f11);
  if (tc._key_f12) setkey("K_F12", tc._key_f12);
  if (tc._key_backspace) setkey("K_BSPACE", tc._key_backspace);
  if (tc._key_ic) setkey("K_INS", tc._key_ic);
  if (tc._key_dc) setkey("K_DEL", tc._key_dc);
  if (tc._key_home) setkey("K_HOME", tc._key_home);
  if (tc._key_end) setkey("K_END", tc._key_end);
  if (tc._key_up) setkey("K_UP", tc._key_up);
  if (tc._key_down) setkey("K_DOWN", tc._key_down);
  if (tc._key_left) setkey("K_LEFT", tc._key_left);
  if (tc._key_ppage) setkey("K_PUP", tc._key_ppage);
  if (tc._key_npage) setkey("K_PDN", tc._key_npage);
  if (tc._key_right) setkey("K_RIGHT", tc._key_right);
  if (tc._key_enter) setkey("K_ENTER", tc._key_enter);
  
  // if exist file /etc/alsterm.$TERM or ~/.alsterm.$TERM
  // load keys from that files
 
  int uid = getuid();
  FILE *f;
  struct passwd *pw;
  char fn[PATH_MAX];
  char ss[255], *ssp;
  char *p;
  char *pc[7];
  char *keyname;
  pw = getpwuid(uid);
  strcpy(fn, "/etc/alsterm."); strcat(fn, getname());
  f = fopen(fn, "r");
  if (f != NULL) {
    // /etc/alsterm.$TERM parsing
    while (fgets(ss, 255, f) != NULL) {
      if (strncmp(ss, "#", 1) == 0) continue;
      if (strncmp(ss, "\n", 1) == 0) continue;
      ssp = &ss[0];
      if ((p = strsep(&ssp, ":")) == NULL) { continue; }
      else {
	keyname = strdup(p); int i=0; 
	while ((p=strsep(&ssp, ":"))!=NULL) {
	  s[i] = atoi(p);
          pc[i++] = p;
	}
	s[i] = '\0';
        setkey(keyname, pt);
	free(keyname);
      }
    } 
    fclose(f);
  }
  strcpy(fn, pw->pw_dir); strcat(fn, "/.alsterm."); strcat(fn, getname());
  f = fopen(fn, "r");
  if (f != NULL) {
    // $HOME/.alsterm.$TERM parsing
    while (fgets(ss, 255, f) != NULL) {
      if (strncmp(ss, "#", 1) == 0) continue;
      if (strncmp(ss, "\n", 1) == 0) continue;
      ssp = &ss[0];
      if ((p = strsep(&ssp, ":")) == NULL) { continue; }
      else {
	keyname = strdup(p); int i=0; 
	while ((p=strsep(&ssp, ":"))!=NULL) {
	  s[i] = atoi(p);
          pc[i++] = p;
	}
	s[i] = '\0';
        setkey(keyname, pt);
	free(keyname);
      }
    } 
    fclose(f);
  }
}

//
// позиционирование курсора
//
void trm::move(int y, int x)
{
  char *cm = tc._cursor_address;

  if (cm == NULL) return;
  if (x < 0 || y < 0 || x >= co() || y >= li()) return;
  tputs(tgoto(cm, x, y), 1, put);
}

//
// вывод в нужную позицию терминала
//
void trm::print(int y, int x, char *s)
{
  move(y, x);
  printf(s); 
}

//
// установить цвет
//
void trm::setfg(int c)
{
  char s[40];
  char *b;
  sprintf(s,"%s",tc._set_a_foreground);
  b = tparm(s,c);
  tputs(b, 1, put);
}

//
// установить цвет фона
//
void trm::setbg(int c)
{
  char s[40], *b;
  sprintf(s,"%s",tc._set_a_background);
  b = tparm(s,c);
  tputs(b, 1, put);
}

//
// установить цветовую пару
//
void trm::setcolor(int fg, int bg)
{
  setfg(fg); setbg(bg);
}

//
// очистка до конца экрана
//
void trm::cleos(int y, int x)
{
  move(y, x);
  tputs(tc._clr_eos, 1, put);
}

//
// очистка до конца строки
//
void trm::cleol(int line)
{
  move(line,0);
  tputs(tc._clr_eol, 1, put);
}

//
// bold mode
//
void trm::bold()
{
  tputs(tc._enter_bold_mode, 1, put);
}

//
// normal mode
//
void trm::normal()
{
  tputs(tc._exit_attribute_mode, 1, put);
}

//
// reverse mode
//
void trm::reverse()
{
  tputs(tc._enter_reverse_mode, 1, put);
}

char *trm::keyname(int num)
{
  return kb[num].name;
}
