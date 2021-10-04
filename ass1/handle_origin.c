/*
Function name: handle_origin
Purpose: to handle the built in <cmd>
Caution: command ls, cd won't work
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <time.h>

#include "process.h"
#include "create_process.h"
#include "append_table.h"
#include "printPT.h"
#include "jobs.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif



process * handle_origin(char cmd[], process * PT){
  char* p;
  char* exec_command[40];
  int command_index = 0;
  int status;
  char* command;
  int has_and = 0;
  int has_in = 0;
  int has_out = 0;
  int in_n_out = 0;
  char file_name[21];
  char in_file_name[21];
  char out_file_name[21];
  int time_used = 0;



  size_t length = strlen(cmd);
  if (cmd[length-2] == '&'){
    has_and = 1;
  }
  cmd[length - 1] = '\0';


  // check if has_in
  for (int i = 0; i < length; i++){
    if (cmd[i] == '<'){
      has_in = 1;
    }
  }
  if (has_in == 1 && has_out == 0){
    char * in_fName = strchr(cmd, '<');
    size_t file_len = strlen(in_fName);
    for (int i = 1; i < file_len; i++){
      file_name[i-1] = in_fName[i];
      if (in_fName[i] == ' ' || in_fName[i] == '\n' || in_fName[i]== '\0'){
        break;
      }
    }

  }

  // check if has_out
  for (int i = 0; i < length; i++){
    if (cmd[i] == '>'){
      has_out = 1;
    }
  }
  if (has_out == 1 && has_in == 0){
    char * out_fName = strchr(cmd, '>');
    size_t file_len = strlen(out_fName);
    for (int i = 1; i < file_len; i++){
      file_name[i-1] = out_fName[i];
      if (out_fName[i] == ' ' || out_fName[i] == '\n' || out_fName[i]== '\0'){
        break;
      }
    }
  }


  // check if has_in and has_out
  if (has_in == has_out && has_out == 1){
    in_n_out = 1;
  }

  if (in_n_out == 1){
    // isolate the input file name

    char * in_fName = strchr(cmd, '<');
    size_t in_file_len = strlen(in_fName);
    for (int i = 1; i < in_file_len; i++){
      in_file_name[i-1] = in_fName[i];
      if (in_fName[i] == ' ' || in_fName[i] == '\n' || in_fName[i]== '\0'){
        break;
      }
    }
    // isolate the output file name

    char * out_fName = strchr(cmd, '>');
    size_t out_file_len = strlen(out_fName);
    for (int i = 1; i < out_file_len; i++){
      out_file_name[i-1] = out_fName[i];
      if (out_fName[i] == ' ' || out_fName[i] == '\n' || out_fName[i]== '\0'){
        break;
      }
    }
  }






  // prepare the command line for creating the process node
  command = strdup(cmd);
  p = strtok(cmd, " ");
  while (p != NULL){
    exec_command[command_index] = p;
    command_index++;
    p = strtok(NULL, " ");
  }

  exec_command[command_index] = NULL;




  int fork_result = fork();
  if (fork_result < 0){
    perror("fork falied");
  }
  if (fork_result == 0){
    // child
    // if has output redirection
    if (has_out == 1 && has_in == 0){

      int fd = open(file_name, O_WRONLY | O_CREAT, 0644);
      if (fd == -1){
        printf("Error when opening a file\n");
        return PT;
      }
      int fd2 = dup2(fd, STDOUT_FILENO);
      if ((execvp(exec_command[0], exec_command)) < 0){ // bug: passing the redirection destination as argument to execute, solution: rebuild exec_command array
        printf("execution fail\n");
        exit(0);
      }
      close(fd);
    }

    else if (has_in == 1 && has_out == 0){
      int fd = open (file_name, O_RDONLY);
      if (fd == -1){
        printf("Error when opeing a file\n");
        return PT;
      }
      int fd2 = dup2(fd, 0);
      if ((execvp(exec_command[0], exec_command)) < 0){
        printf("execution fail\n");
        exit(0);
      }
      close(fd);

    }

    else if (in_n_out == 1){
      int fd1 = open(in_file_name, O_RDONLY);
      int fd2 = open(out_file_name, O_WRONLY | O_CREAT, 0644);
      if (fd2 == -1 || fd1 == -1){
        printf("Error when opeing a file\n");
        return PT;
      }
      int fd11 = dup2(fd1, 0);
      int fd22 = dup2(fd2, STDOUT_FILENO);
      if ((execvp(exec_command[0], exec_command)) < 0){
        printf("execution fail\n");
        exit(0);
      }
      close(fd1);
      close(fd2);
    }

    else{
      if ((execvp(exec_command[0], exec_command)) < 0){
        printf("execution fail\n");
        exit(0);
      }
    }
  }

  else{
    // parent
    // save pid, insert new process in the pcb, wait for the child

    if (has_and == 0){
      waitpid(fork_result, &status, WUNTRACED);
    }

    time_t time_start = time(NULL);
    process * Process = create_process((pid_t)fork_result, 'R', time_used, command);
    PT = append_table(PT, Process);

  }




  return PT;
}
