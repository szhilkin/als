#include <stdio.h>
#include "tty.h"
#include "trm.h"

int main(void)
{
  char k[20];
  tty t;
  trm term(&t);
  while(1) {
    term.openv(); term.readkey();
    int chars = term.charcount();
    for (int i=0; i < chars; i++) k[i] = term.getbuf(i);
    term.closev();
    for (int i=0; i < chars; i++) printf("%d ", k[i]);
    printf("\n");
  }
  return 0;
}
