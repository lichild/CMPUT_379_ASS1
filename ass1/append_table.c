#include <stdio.h>
#include <stdlib.h>

#include "process.h"

// add the processes into process table at the correct position

process * append_table(process * head, process * Process){
  process * current = NULL;
  process * next = NULL;

  if (head == NULL){
    head = Process;
  }

  else{
    current = head;
    next = current->next;

    while (next != NULL) {
      if ((Process->pid) > (next->pid) && (next->next) == NULL){
        // reach the end of the list
        next->next = Process;
        return head;
      }

      else if ((Process->pid) < (next->pid) && (Process->pid) > (current->pid)){
        // case: insert between two nodes
        current->next = Process;
        Process->next = next;
        return head;
      }

      else if ((Process->pid) > (current->pid) && (current->next) != NULL){
        // case if the the element inserted is smaller than the first one and there is more than one node
        Process->next = current;
        head = Process;
        return head;
      }

      current = next;
      next = current->next;

    }

    if ((Process->pid) < (current->pid)){
      Process->next = current;
      head = Process;
    }
    current->next = Process;
  }
  return head;

}
