#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "init_shell.h"
#include "read_command.h"
#include "parsing.h"
#include "single_command.h"
#include "handle_origin.h"
#include "with_para.h"
#include "process.h"
#include "init_table.h"
#include "append_table.h"
#include "printPT.h"



#define LINE_LENGTH 100
#define MAX_ARGS 7
#define MAX_LENGTH 20
#define MAX_PT_ENTRIES 32



int sys_time = 0;
int user_time = 0;

// main function
int main(int argc, char* argv[]){



  // define variables
  char command[LINE_LENGTH+2], cmd[LINE_LENGTH+2], *parameters[40];
  char* check_list[] = {"exit", "kill", "jobs", "resume", "sleep", "suspend", "wait"}; // check list for all <cmd> that are specified
  int check_list_length = sizeof(check_list) / sizeof(check_list[0]);
  int check_command = 0; // flag to check whether the command input is in the check list

  // init process table
  process * PT = init_table();


  while(1){
    int parsed = 0;
    // init shell
    init_shell();
    fflush(stdin);
    // getting input
    read_command(command); // read raw user input
    strcpy(cmd, command);
    parsed = parsing(command, parameters); // separate the command and the arguments
    if (parsed == -1){
      continue;
    }


    // count how many parameters user put in
    int para_in = 0;
    while (parameters[para_in] != NULL){
      para_in++;
    } // note: if the user does input parameter, the actual number of parameter is para_in - 1

    if (para_in-1 > MAX_ARGS){
      printf("too many arguments entered\n");
      continue;
    }

    // handle the original command that the shell should have
    for (int i = 0; i < check_list_length; i++){
      if (strcmp(command, check_list[i]) == 0){ // if the user input is in the command list, then stop checking and change the flag
        check_command = 1;
        break;
      }
    }
    if (check_command == 0){ // if user input is not in the check list
      PT = handle_origin(cmd, PT); // function that will execute the built in <cmd>
    }


    if (para_in == 0 && check_command != 0){
      single_command(command, PT); // a function that will handle single commands
      check_command = 0;
    }
    else if (para_in > 0 && check_command != 0){
      //printf("Position main.c: check parameters = %s\n", parameters[0]);
      int argument = atoi(parameters[0]);
      with_para(command, parameters, PT, argument); // a function that will handle commands with parameters
      check_command = 0;
    }

    if (strcmp(command, "exit") == 0){ // remember to deal with ctrl+d
      break;
    }
  }
  struct rusage ru;
  if(getrusage(RUSAGE_CHILDREN, &ru) < 0){
    printf("can't get resource\n");
    return 0;
  }

  struct timeval user_time =ru.ru_utime;
  struct timeval sys_time = ru.ru_stime;
  printf("Completed processes:\n");
  printf("User time =\t%5ld seconds\n", (user_time.tv_usec+user_time.tv_sec)/10000);
  printf("Sys  time =\t%5ld seconds\n", (sys_time.tv_usec+sys_time.tv_sec)/10000);






  return 0;
}
