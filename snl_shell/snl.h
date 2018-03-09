/*
  Header file for snl.c
  Declares all functions required for SNL Shell

  Authors: LucyWilcox, nmohamed, shrutiyer
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#define MAXARGS 100

// Utils
char* get_cwd();
int snl_detect_pipe(char** args);
int snl_detect_redirect(char** args);

// Shell Loop, Read, Execute functions
int snl_builtins_number();
void snl_loop(void);
char* snl_read_line(void);
int snl_split_line(char* line, char** args);
int snl_execute(char** args);

// Fork functions
int snl_fork(char** args);
int snl_forkpipe(char** args);
int snl_fork_redirect(char** args, int redirect_index);

// Main function
int main(int argc, char** argv, char** envp);
