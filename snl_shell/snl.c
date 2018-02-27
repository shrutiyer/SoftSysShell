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

// TODO: This is it folks, only 100 commands at a time
#define MAXARGS 100

// Function Declations TODO: Move to a header file
int snl_builtins_number();
void snl_loop(void);
char* snl_read_line(void);
void snl_split_line(char* line, char** args);
int snl_execute(char** args);
int snl_fork(char **args);
int main(int argc, char **argv);

// An array of strings with built-in command names
char* snl_builtins_names[] = {"cd"};

/*
	Calculates the number of built-in commands
	Inputs: Nothing
	Returns: Number of built-ins
*/
int snl_builtins_number() {
	return sizeof(snl_builtins_names)/sizeof(char*);
}

/*
	Main loop of the shell program
	Inputs: Nothing
	Returns: Nothing

	Using slight adaption of Brennan's implementation of the
  main loop https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
void snl_loop(void){
  char *line;
  char *args[MAXARGS];
  int status;

  do {
    printf("> ");
    line = snl_read_line();
    snl_split_line(line, args);
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
	return line;
}

/*
	Splits line into set of args
	Inputs: The user-inputted line of command,
					an array of arrays to store the splitted line
	Returns: Nothing
*/
void snl_split_line(char* line, char** args){
	for (int i = 0; i < MAXARGS; i++) {
		// Get each arg seperated by space
		args[i] = strsep(&line, " ");

		if (args[i] == NULL){
			break; // end of line
		} else if (strlen(args[i]) == 0){
			i--; // re-write over empty entry
		}
	}
}

/*
	Analyzes either to launch a child process or run a built-in command
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
			// TODO: call the built-in function
			printf("BUILTINS\n");
		}
	}
	return snl_fork(args);
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
    if(execvp(args[0], args) == -1){
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
