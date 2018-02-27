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
int snl_execute(char** args, char** args2, int piped);
int snl_fork(char **args);
int main(int argc, char **argv);
int snl_detect_pipe(char* line, char** linepiped, char** args, char** args2);
int snl_forkpipe(char** args, char** args2);

// An array of strings with built-in command names
char* snl_builtins_names[] = {"cd"};

/*
	Calculates the number of built-in commcdands
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
  char *args2[MAXARGS];
  char *argspiped[MAXARGS];
  int status;
  int piped;

  do {
    printf("> ");
    line = snl_read_line();
    piped = snl_detect_pipe(line, argspiped, args, args2);
    status = snl_execute(args, args2, piped);

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

  based off of: https://www.geeksforgeeks.org/making-linux-shell-c/
*/
void snl_split_line(char* line, char** args){
	for (int i = 0; i < MAXARGS; i++) {
		// Get each arg seperated by space
		args[i] = strsep(&line, " ");
    // printf("%s\n", args[i]);

		if (args[i] == NULL){
			break; // end of line
		} else if (strlen(args[i]) == 0){
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
int snl_detect_pipe(char* line, char** linepiped, char** args, char** args2){
  for (int i = 0; i < 2; i++) {
    char* c = strsep(&line, "|");
    printf("c %s\n", c);
    // linepiped[i] = strsep(&line, "|");
    linepiped[i] = c;
    if (linepiped[i] == NULL){
      break; // no input
    }
    printf("%s\n", linepiped[1]);
    // if there is a second arg here there is a pipe
    if (linepiped[0] == NULL) {
      snl_split_line(line, args);
      return 0; // no pipe
    } else {
      snl_split_line(linepiped[0], args);
      snl_split_line(linepiped[1], args2);
      return 1; // pipe
    }
  }

}

/*
	Analyzes either to launch a child process or run a built-in command
	Inputs: char** of the user-inputted line split with spaces
	Returns: TODO

	Adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
int snl_execute(char** args, char** args2, int piped) {
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
  printf("%d\n", piped);
  if (!piped){
	  return snl_fork(args);
  }
  return snl_forkpipe(args, args2);

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

int snl_forkpipe(char** args, char** args2){
  printf("%s\n", "jesus christ...");
  pid_t pid1, pid2;
  int piping[2];

  if (pipe(piping) < 0){ // https://www.geeksforgeeks.org/pipe-system-call/
    printf("%s\n", "pipe isn't working");
    exit(EXIT_FAILURE);
  }
  pid1 = fork();
  if (pid1 < 0){
    printf("%s\n", "pid1 won't fork");
    exit(EXIT_FAILURE);
  }
  if (pid1 == 0){
    close(piping[0]);
    dup2(piping[0], STDOUT_FILENO); //http://codewiki.wikidot.com/c:system-calls:dup2
    close(piping[1]);

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
    close(piping[1]);
    dup2(piping[0], STDIN_FILENO); //http://codewiki.wikidot.com/c:system-calls:dup2
    close(piping[0]);

      if(execvp(args[0], args) == -1){
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
