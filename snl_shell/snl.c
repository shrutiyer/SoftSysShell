/*
  SNL (Shruti, Nora, Lucy) Shell
  Created for Software Systems Project 1
  Authors: LucyWilcox, nmohamed, shrutiyer

  This program is an implementation of a shell with a few basic UNIX commands
*/

#include "snl.h"
#include "builtins.c"
#include "fork.c"
#include "utils.c"

extern char** environ;

/*
  Main loop of the shell program
  Inputs: Nothing
  Returns: Nothing

  Using slight adaption of Brennan's implementation of the
  main loop https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
void snl_loop(void) {
  char* line;
  char* args[MAXARGS];
  int status;
  int piped;
  int split;

  do {
    printf("%s> ", get_cwd());
    line = snl_read_line();
    split = snl_split_line(line, args);
    status = snl_execute(args);

    free(line);
    // free(args);
  } while (status);
}

/*
  Reads the line from standard input
  Inputs: Nothing
  Returns: char* of the user-inputted line by getline

  Based off of Brennan's implementation
*/
char* snl_read_line(void){
  char* line;
  ssize_t buffersize = 0;
  getline(&line, &buffersize, stdin);
  // Remove trailing \n character with end of line
  if (line[strlen(line)-1]=='\n') {
    line[strlen(line)-1]='\0';
  }
  return line;
}


/*
  Splits line into set of args, replaces args with environment val when specified
  Inputs: The user-inputted line of command,
  an array of arrays to store the splitted line
  Returns: Nothing

  based off of: https://www.geeksforgeeks.org/making-linux-shell-c/
*/
int snl_split_line(char* line, char** args){
  for (int i = 0; i < MAXARGS; i++) {
    // Get each arg seperated by space
    args[i] = strsep(&line, " ");

    if (args[i] == NULL) {
      break; // end of line
    } else if (strlen(args[i]) == 0) {
      i--; // re-write over empty entry
    } else {
      if (strcmp(&args[i][0], "$")){ // 36 is $
        char** env_elm;
        for (env_elm = environ; *env_elm != 0; env_elm ++ ){
          char* keyval[2];
          char* elm_dup = strdup(*env_elm); // dup elm so we don't edit actual env
          for (int i = 0; i < 2; i++) {
            keyval[i] = strsep(&elm_dup, "=");
            if (keyval[i] == NULL){
              break;
            }
          }
          if (strcmp(args[i] + 1, keyval[0]) == 0) {
            args[i] = keyval[1]; // replace arg with env val
          }
        }
      }
    }
  }
  return 1;
}

/*
  Analyzes either to run a built-in command or launch child processes
  Inputs: char** of the user-inputted line split with spaces
  Returns: Value from forking the process

  Adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
int snl_execute(char** args) {
  // If the command was null
  if (args[0] == NULL) {
    return 1;
  }
  for (int i=0; i<snl_builtins_number(); i++) {
    if (!strcmp(args[0], snl_builtins_names[i])) {
      return (*snl_builtin_func[i])(args, environ);
    }
  }
  // If there's a pipe present
  if (snl_detect_pipe(args) == 1){
    return snl_forkpipe(args);
  }
  if (snl_detect_redirect(args) != 0) {
    return snl_fork_redirect(args, snl_detect_redirect(args));
  }
  return snl_fork(args);
}


/*
  Main function of the shell

  Using Brennan's implementation of main
  https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
int main(int argc, char** argv, char** envp){
  // Load config files, if any.

  // Run command loop.
  environ = envp;
  snl_loop();

  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}
