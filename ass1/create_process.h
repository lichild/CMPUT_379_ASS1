#include "process.h"
#ifndef CREATE_PROCESS
#define CREATE_PROCESS
process * create_process(pid_t pid, char status, int time, char* command);
#endif
