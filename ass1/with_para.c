#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include "delete_process.h"
#include "process.h"
#include "search_process.h"

int resume(int pid, process * PT);
int suspend(int pid, process * PT);

void with_para(char command[], char* para[], process* PT, int argument){

  // command kill<int>
  if (strcmp(command, "kill") == 0){
    int success = kill(argument, SIGKILL);
    if (success == 0){
      PT = delete_process(&PT, argument);
    }
    else{
      printf("An error has occured\n");
    }
  }




  // command resume<int>
  else if (strcmp(command, "resume") == 0){
    int temp = resume(argument, PT);
    if (temp == argument){
      printf("Process %d has already finished\n", argument);
    }
    else if(temp != 0){
      printf("An error has occured\n");
    }
  }



  // command sleep<int>
  else if (strcmp(command, "sleep") == 0){
    sleep(argument);
  }



  // command suspend
  else if (strcmp(command, "suspend") == 0){
    int temp = suspend(argument, PT);
    if (temp == argument){
      printf("Process %d has already finished\n", argument);
    }
    else if (temp != 0) {
      printf("An error has occrued\n");
    }
  }


  // command wait
  else if (strcmp(command, "wait") == 0){
    process * temp = search_process(PT,argument);
    if (temp == NULL){
      printf("There is no such process\n");
      return;
    }
    if (temp->status == 'S'){
      printf("This process is being suspended\n");
      return;
    }
    if(waitpid(argument, NULL, 0) == -1){
      printf("An error has occured when waiting process %d\n", argument);
      return;
    }
    else{
      PT = delete_process(&PT, argument);
    }
  }

  return;
}

// a function to resume the indicated function
int resume(int pid, process * PT){
  int result = 0;

  // check if the process has already finished
  result = waitpid(pid, NULL, WNOHANG);
  if (result == pid){
    PT = delete_process(&PT, pid);
    return result;
  }

  // Run the process
  result = kill(pid, SIGCONT);
  if (result == 0){
    process * temp = search_process(PT, pid);
    temp->status = 'R';
    time_t now = time(NULL);
    temp->time = now + temp->time;
  }

  return result;

}


// a function that suspend certain function
int suspend(int pid, process * PT){
  int result = 0;

  // check if the process has already finished
  result = waitpid(pid, NULL, WNOHANG);
  if (result == pid){
    PT = delete_process(&PT, pid);
    return result;
  }

  // suspend the Process
  result = kill(pid, SIGSTOP);
  if (result == 0){
    process * temp = search_process(PT, pid);
    temp->status = 'S';
    time_t now = time(NULL);
    temp->time = now - temp->time;
  }

  return result;
}
