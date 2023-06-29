#include "trm.h"
#include "dirctl.h"
#include "main.h"
#include "config.h"
#include "main.h"
#include <signal.h>
#include <stdio.h>

dirctl *dp;
static struct sigaction action;

static void talarm(int sig_no)
{
  alarm(CLOCK_INTERVAL);
  dp->showclock();
}

int main()
{
  tty t;
  trm term(&t);
  alsconfig conf(&term);
  
  term.closev();
  if ((term.co() < 80) | (term.li() < 25)) {
    printf("Terminal not supported.\n");
    return -1;
  }
  if (!conf.isgood()) {
    printf("Config file not found.\n");
    return -1;
  }
  term.openv();
  signal(SIGINT, SIG_IGN);
  
  dirctl d(&term,&conf);
  dp = &d;

  if (conf.clock) {
    // устанавливаем таймер для показа часов
    sigemptyset(&action.sa_mask);
    action.sa_handler = (void (*)(int)) talarm;
    action.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &action, 0);
    alarm(CLOCK_INTERVAL);
  }  
  
  d.run();
  term.closev();
  printf("\nBye!\n\n");  

  return 0;
}
