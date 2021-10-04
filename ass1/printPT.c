#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "process.h"

void printPT(process * PT){
  time_t now = time(NULL);
  printf("#\tPID\tS\tSEC\tCOMMAND\n");
  int counter = 0;
  process * p = PT;
  while (p != NULL) {
    p->time = now - (p->time);
    printf("%d:\t", counter);
    printf("%d\t", p->pid);
    printf("%c\t", p->status);
    printf("%d\t", p->time);
    printf("%s\t\n", p->command);
    p = p->next;
    counter++;
  }
  printf("Process = \t%d active\n", counter);
  return;
}
