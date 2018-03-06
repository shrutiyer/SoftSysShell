/*
SNL (Shruti, Nora, Lucy) Shell
Created for Software Systems Project 1

This program is an implementation of a shell with a few basic UNIX commands
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "builtins.c"

// TODO: This is it folks, only 100 commands at a time
#define MAXARGS 100

// Function Declations TODO: Move to a header file
int snl_builtins_number();
void snl_loop(void);
char* snl_read_line(void);
void snl_split_line(char* line, char** args);
int snl_execute(char** args, char** args2, int piped, int redirect, char *output_file);
int snl_fork(char **args);
char* get_cwd();
int main(int argc, char **argv);
int snl_detect_pipe(char* line, char** args, char** args2);
int snl_detect_redirect(char* line, char** args, char *file_name);
int snl_forkpipe(char** args, char** args2);
int snl_fork_redirect(char** args, char* output_file);

/*
  Main loop of the shell program
  Inputs: Nothing
  Returns: Nothing

  Using slight adaption of Brennan's implementation of the
  main loop https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
void snl_loop(void) {
  char *line;
  char *redirect_line;
  char *args[MAXARGS];
  char *args2[MAXARGS];
  char *redirect_file;
  int status;
  int piped;
  int redirect;

  do {
    printf("%s> ", get_cwd());
    line = snl_read_line();
    redirect_line = strdup(line);
    piped = snl_detect_pipe(line, args, args2);
    redirect = snl_detect_redirect(redirect_line, args, redirect_file);
    status = snl_execute(args, args2, piped, redirect, redirect_file);

    free(line);
    free(redirect_line);
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
  char *buf, *ptr;
  buf = (char *)malloc((size_t)size);

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
	Splits line into set of args
	Inputs: The user-inputted line of command,
					an array of arrays to store the splitted line
	Returns: Nothing

  based off of: https://www.geeksforgeeks.org/making-linux-shell-c/
*/
void snl_split_line(char* line, char** args){
  for (int i = 0; i < MAXARGS; i++) {
    // Get each arg seperated by space
    args[i] = strsep(&line, " ");

    if (args[i] == NULL) {
      break; // end of line
    } else if (strlen(args[i]) == 0) {
      i--; // re-write over empty entry
    }
  }
}

/*
  Detects pipes and splits args based on pipes
  Inputs: the users inputed line,
          an array which will be used as to split cmds into args
  Returns: 0 if not piped, 1 if piped

  adapted from: https://www.geeksforgeeks.org/making-linux-shell-c/
*/
int snl_detect_pipe(char* line, char** args, char** args2){
  char *linepiped[2];
  for (int i = 0; i < 2; i++) {
    linepiped[i] = strsep(&line, "|");
    if (linepiped[i] == NULL){
      break;
    }
  }
  // if there is a second arg here there is a pipe
  if (linepiped[1] == NULL) {
    snl_split_line(linepiped[0], args);
    return 0; // no pipe
  } else {
    snl_split_line(linepiped[0], args);
    snl_split_line(linepiped[1], args2);
    return 1; // pipe
  }
}

int snl_detect_redirect(char* line, char** args, char *file_name){
  char *redirect_line[2];
  for (int i = 0; i < 2; i++) {
    redirect_line[i] = strsep(&line, ">");
    if (redirect_line[i] == NULL){
      break;
    }
  }
  // if there is a second arg/file here there is a redirect
  if (redirect_line[1] == NULL) {
    snl_split_line(redirect_line[0], args);
    return 0; // no redirect
  } else {
    snl_split_line(redirect_line[0], args);
    // TODO: Fix the whitespace problem with filename
    file_name = redirect_line[1];
    return 1; // redirect
  }

}



/*
	Analyzes either to launch a child process or run a built-in command
	Inputs: char** of the user-inputted line split with spaces
	Returns: TODO

  Adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
int snl_execute(char** args, char** args2, int piped, int redirect, char *output_file) {
	// If the command was null
	if (args[0] == NULL) {
		return 1;
	}

	for (int i=0; i<snl_builtins_number(); i++) {
		if (!strcmp(args[0], snl_builtins_names[i])) {
			return (*snl_builtin_func[i])(args);
		}
	}
  if (piped) {
	  return snl_forkpipe(args, args2);
  } else if (redirect) {
    return snl_fork_redirect(args, output_file);
  } else {
    return snl_fork(args);
  }

}

/*
  Launches a child process
  Inputs: The user-inputted line of command stored in an array of arrays
  Returns: 1

  Using Brennan's implementation and https://github.com/jmreyes/simple-c-shell
*/
int snl_fork(char **args){
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

  Mix of Brennan's implementation and https://www.geeksforgeeks.org/making-linux-shell-c/
*/
int snl_forkpipe(char** args, char** args2){
  pid_t pid1, pid2;
  // used to keep track of each end of the file descriptor
  //see https://www.geeksforgeeks.org/pipe-system-call/
  int pipefd[2];
  if (pipe(pipefd) < 0){
    printf("%s\n", "pipe isn't working");
    exit(EXIT_FAILURE);
  }
  pid1 = fork();
  if (pid1 < 0){
    printf("%s\n", "pid1 won't fork");
    exit(EXIT_FAILURE);
  }
  if (pid1 == 0){
    // close closes the file descriptor
    // see http://codewiki.wikidot.com/c:system-calls:close
    close(pipefd[0]);
    // dup2 takes source and destination file descriptors
    // see http://codewiki.wikidot.com/c:system-calls:dup2
    dup2(pipefd[0], STDOUT_FILENO);
    close(pipefd[1]);

    if(execvp(args[0], args) == -1){
      perror("snl");
      kill(getpid(), SIGTERM);
    }
  } else {
    //PARENT PROCESS
    pid2 = fork();

    if (pid2 < 0){
      printf("%s\n", "pid2 won't fork");
      exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);

      if(execvp(args2[0], args2) == -1){
        perror("snl");
        kill(getpid(), SIGTERM);
      }
    } else {
      wait(NULL);
      wait(NULL);
    }
  }
  return 1;
}

/*

*/
int snl_fork_redirect(char** args, char* output_file) {
  int file_descriptor;
  pid_t pid = fork();

  if (pid == -1) {
    printf("Fork could not create a child process\n");
  } else if (pid == 0) {
    // Open a  file to write to it
    file_descriptor = open(output_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
		// Replace standard output with appropriate file
		dup2(file_descriptor, STDOUT_FILENO);
    close(file_descriptor);
    printf("IN HEREEEE\n");

    if (execvp(args[0], args) == -1){
      printf("COOOOOOL\n");
      perror("snl fork redirect");
  		kill(getpid(),SIGTERM);
    }
  }
  waitpid(pid, NULL, 0);
}

/*
 	Main function of the shell

  Using Brennan's implementation of main
 	https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
int main(int argc, char **argv){
  // Load config files, if any.

  // Run command loop.
  snl_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
