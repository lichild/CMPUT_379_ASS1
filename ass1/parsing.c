#include <stdio.h>
#include <string.h>

// notice that the last parameter always ends as "\n"
int parsing(char cmd[], char* par[]){

  int parse_succ = 0;

  // cmd is the string with '\n' at the end
  int input_len = strlen(cmd);
  int has_and= 0;

  // check if there is '&' in input
  for (int i = 0; i < input_len; i++){
    if (cmd[i] == '&'){
      has_and = 1;
    }
  }
  if (has_and == 1){
    if (cmd[input_len-2] != '&'){
      printf("Invalid input, & must be at the end of command\n");
      parse_succ = -1;
      return parse_succ;
    }
  }

  // check input and output command and regulate them
  int has_in = 0;
  for (int i = 0; i < input_len; i++){
    if (cmd[i] == '<'){
      has_in = 1;
    }
  }
  if (has_in == 1 && cmd[0] == '<'){
    printf("Invalid input: '<' can't be at the beginning of a command\n");
    parse_succ = -1;
    return parse_succ;
  }

  int has_out = 0;
  for (int i = 0; i < input_len; i++){
    if (cmd[i] == '>'){
      has_out = 1;
    }
  }
  if (has_out == 1 && cmd[input_len-2] == '>'){
    printf("Invalid input: '> can't be at the end of a command\n");
    parse_succ = -1;
    return parse_succ;
  }



  int index = 0;
  char* p;
  p = strtok(cmd, "\n"); // get '\n' off
  p = strtok(p, " "); // get off space ang tokenize the command input


  while (p != NULL){

    if (index == 0){
      // put the command in cmd
      cmd = p;
    }
    else{
      par[index-1] = p;
    }

    index++;
    // put the parameters in par[]
    p = strtok(NULL, " ");

  }

  if (index == 1){
    par[index-1] = NULL;
  }
  else{
    par[index] = NULL;
  }



  return parse_succ;
}
