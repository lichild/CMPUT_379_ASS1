#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"

// create a process object and return its address

process * create_process(pid_t pid, char status, int time, char *command){

  process * Process = (process*)malloc(sizeof(process));
  Process->pid = pid;
  Process->status = status;
  Process->time = time;
  Process->command = command;
  Process->next = NULL;
  
  return Process;

}
