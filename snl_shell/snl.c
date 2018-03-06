/*
SNL (Shruti, Nora, Lucy) Shell
Created for Software Systems Project 1

This program is an implementation of a shell with a few basic UNIX commands
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "builtins.c"

// TODO: This is it folks, only 100 commands at a time
#define MAXARGS 100

// Function Declations TODO: Move to a header file
int snl_builtins_number();
void snl_loop(void);
char* snl_read_line(void);
int snl_split_line(char* line, char** args);
int snl_execute(char** args);
int snl_fork(char** args);
char* get_cwd();
int main(int argc, char** argv, char** envp);
int snl_detect_pipe(char** args);
int snl_forkpipe(char** args);
int snl_detect_redirect(char** args);
int snl_fork_redirect(char** args);

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
  Returns your current working directory.
  Inputs: Nothing
  Returns: char* of path to your current working directory
*/
char* get_cwd() {
  long size = pathconf(".", _PC_PATH_MAX);
  char* buf, *ptr;
  buf = (char* )malloc((size_t)size);

  if (buf != NULL){
    ptr = getcwd(buf, (size_t)size);
  }

  return ptr;
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
  Detects if array of arguments has a pipe as an arg
  Inputs: an array which will be used as to split cmds into args
  Returns: 0 if not piped, 1 if piped
*/

int snl_detect_pipe(char** args){
  for (int i = 0; i < MAXARGS; i++) { //TODO: make this not maxargs
    if(args[i] == NULL){
      return 0; // reached end, no pipe
    }
    if (strcmp(args[i],"|") == 0){
      return 1; // yes pipe
    }
  }
  return 0; // no pipe
}

int snl_detect_redirect(char** args) {
  for (int i = 0; i < MAXARGS; i++) { //TODO: make this not maxargs
    if(args[i] == NULL){
      return 0; // reached end, no pipe
    }
    if (strcmp(args[i],">") == 0){
      return 1; // yes pipe
    }
  }
  return 0; // no pipe
}

/*
  Analyzes either to run a built-in command or launch child processes
  Inputs: char** of the user-inputted line split with spaces
  Returns: TODO

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
  if (snl_detect_redirect(args) == 1) {
    // REDIRECT
    printf("REDIRECT\n");
    return snl_fork_redirect(args);
  }
  /* TODO: If there's file I/O redirection present, do that
    return snl_fork_io(args);
  */
  return snl_fork(args);
}

/*
  Launches a child process
  Inputs: The user-inputted line of command stored in an array of arrays
  Returns: 1

  Using Brennan's implementation and https://github.com/jmreyes/simple-c-shell
*/
int snl_fork(char** args){
  pid_t pid = fork();
  int status;

  if(pid == 0){
  // CHILD PROCESS
    // If non-exisiting commands are launched, end process
    if(execvp(*args, args) < 0){
      perror("snl");
      kill(getpid(), SIGTERM);
    }
    exit(EXIT_FAILURE);
  } else if(pid < 0){
    perror("snl");
  } else{
  // PARENT PROCESS
    do{
      waitpid(pid, &status, WUNTRACED); // TODO: may need to be wpid = waitpid
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

/*
  Launches two child processes piping the first to the second
  Inputs: Two (before and after pipe) user-inputted line of command stored in an array of arrays
  Returns: 1

  Mix of Brennan's implementation, https://github.com/jmreyes/simple-c-shell,
  and https://www.geeksforgeeks.org/making-linux-shell-c/
*/
int snl_forkpipe(char* args[]){
  int filedes[2];
  int filedes2[2];
  int num_commands = 0;
  char* command[256];
  int status;
  pid_t pid;

  int err = -1;
  int end = 0;

  // Find # of commands
  for(int i = 0; args[i] != NULL; i++){
    if (strcmp(args[i],"|") == 0){
      if(args[i+1] == NULL){
        puts("snl: cannot end command with pipe");
        return 1;
      }
      num_commands++;
    }
  }
  num_commands++; //because #commands = #pipes + 1

  int k = 0;
  int j = 0;
  int l = 0;
  int i = 0;

  while(args[j] != NULL && end != 1){
      k = 0;
      while (strcmp(args[j],"|") != 0){
        command[k] = args[j];
        j++;
        if(args[j] == NULL){
          end = 1;
          k++;
          break;
        }
        k++;
      }
      // Last position of command is NULL to indicate you are at the end
      command[k] = NULL;
      j++;

      // allows a pipe to be shared between each two iterations, so inputs and
      // outputs two different commands are connected
      if(i % 2 != 0){
        pipe(filedes); // for odd i
      } else{
        pipe(filedes2); // for even i
      }

      pid=fork();

      if(pid==-1){
        if(i != num_commands - 1){ //if it's not the last command
          if(i % 2 != 0){
            close(filedes[1]); // for odd i
          } else{
            close(filedes2[1]); // for even i
          }
        }
        perror("snl");
        return 1;
      }
      if(pid==0){
        // if first command
        if(i == 0){
          dup2(filedes2[1], STDOUT_FILENO);
        }
        // if last command, replace stain depending on if we're in
        //an even or an odd position.
        else if(i == num_commands - 1){
          if(num_commands % 2 != 0){ // for odd # of commands
            dup2(filedes[0],STDIN_FILENO);
          } else{ // for even # of commands
            dup2(filedes2[0],STDIN_FILENO);
          }
        // if command is inbetween two pipes, use one for input
        // and other for output
      } else{
          if(i % 2 != 0){ // for odd command
            dup2(filedes2[0],STDIN_FILENO);
            dup2(filedes[1],STDOUT_FILENO);
          } else{ // for even command
            dup2(filedes[0],STDIN_FILENO);
            dup2(filedes2[1],STDOUT_FILENO);
          }
        }

        // If non-existing commands are launched, end process
        if(execvp(command[0],command)==err){
          perror("snl");
          kill(getpid(),SIGTERM);
        }
      }

      // close descriptors for parent
      if(i == 0){
        close(filedes2[1]);
      }
      else if(i == num_commands - 1){
        if(num_commands % 2 != 0){
          close(filedes[0]);
        } else{
          close(filedes2[0]);
        }
      } else{
        if(i % 2 != 0){
          close(filedes2[0]);
          close(filedes[1]);
        } else{
          close(filedes[0]);
          close(filedes2[1]);
        }
      }

      do{
        waitpid(pid, &status, WUNTRACED);
      } while(!WIFEXITED(status) && !WIFSIGNALED(status));

      i++;
    }

  return 1;
}

int snl_fork_redirect(char** args) {

  return 1;
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
