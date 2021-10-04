#ifndef PROCESS_NODE
#define PROCESS_NODE

typedef struct process{

  pid_t pid;
  char status;
  int time;
  char* command;
  struct process * next;

}process;

#endif
