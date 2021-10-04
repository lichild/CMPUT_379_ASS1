Author: Ziang(Frank) Li

1. The time section won't work
2. The input/output redirection has bug: see details in handle_origin.c
3. File contains memory leak, uncommant the 4 free() lines in delete_process.c will make sure leak free, however, will encounter double free bug.
4. All other functions are fully functional.
5. runner is a function that sleeps for 10 seconds 
