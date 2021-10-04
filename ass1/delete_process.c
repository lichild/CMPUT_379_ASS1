#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "process.h"

process * delete_process(process ** head, pid_t pid){
  process * temp = *head, *prev;

  // delete head
  if (temp != NULL && (temp->pid) == pid){
    *head = temp->next;
    /*
    free(temp->command);
    free(temp);
    */
    return *head;
  }

  while (temp != NULL && (temp->pid) != pid){
    prev = temp;
    temp = temp->next;
  }
  // not found
  if (temp == NULL){
    return *head;
  }
  prev->next = temp->next;
  /*
  free(temp->command);
  free(temp);
  */
  return *head;


}
