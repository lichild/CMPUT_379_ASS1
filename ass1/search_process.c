#include <stdio.h>
#include <stdlib.h>

#include "process.h"

// function that returns the process searching for, if process not in list, returns null

process * search_process(process * PT, int pid){

  process * current = PT;
  while (current != NULL) {
    if (current->pid == pid){
      return current;
    }
    current = current->next;
  }
  return NULL;
}
