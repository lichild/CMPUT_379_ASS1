#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jobs.h"
#include "process.h"

void single_command(char cmd[], process* PT){

  if (strcmp(cmd, "jobs") == 0){
    jobs(PT);
  }
  return;
}
