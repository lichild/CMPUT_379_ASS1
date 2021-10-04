/*
jobs function
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "process.h"
#include "printPT.h"
#include "delete_process.h"

void jobs(process * PT){

  int processes_num = 0;

  printf("Running processes:\n");
  // prepare to print out the pcb table
  int status = 0;
  process * temp = PT;
  while (temp != NULL) {
    status = waitpid(temp->pid, NULL, WNOHANG);
    //printf("%d\n", status);
    if (status == -1 ){
      PT = delete_process(&PT, temp->pid);
      processes_num++;
    }
    temp = temp->next;
  }
  if (PT == NULL){
    printf("Process=\t0 active\n");
  }
  if (PT != NULL){
    printPT(PT);
  }

  struct rusage ru;
  if(getrusage(RUSAGE_CHILDREN, &ru) < 0){
    printf("can't get resource\n");
    return;
  }

  struct timeval user_time =ru.ru_utime;
  struct timeval sys_time = ru.ru_stime;
  printf("Completed processes:\n");
  printf("User time =\t%5ld seconds\n", (user_time.tv_usec+user_time.tv_sec)/10000);
  printf("Sys  time =\t%5ld seconds\n", (sys_time.tv_usec+sys_time.tv_sec)/10000);



  return;
}
