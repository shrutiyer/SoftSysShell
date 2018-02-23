
/*
SNL (Shruti, Nora, Lucy) Shell
Created for Software Systems project 1

*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


// todo: this is it folks, only 100 commands at a time
#define MAXARGS 100


/* splits line into set of args
*/
void snl_split_line(char* line, char** args){
	for (int i = 0; i < MAXARGS) {
		// get each arg seperated by space
		args[i] = strsep(&line, " "); 

		if (parsed[i] == NULL){
			break; // end of line
		} else if (strlen(parsed[i]) == 0){
			i--; // re-write over empty entry
		}
	}
}

// Using Brennan's implementation of the main loop
// https://brennan.io/2015/01/16/write-a-shell-in-c/
int snl_fork(char **args){
  pid_t pid = fork();
  int status;

  if(pid == 0){ // CHILD PROCESS
    // If non-exisiting commands are launched, end process
    if(execvp(args[0], args) == -1){
      perror("snl command not found");
      kill(getpid(), SIGTERM);
    }
    //
    exit(EXIT_FAILURE);
  } else if(pid < 0){
    perror("snl forking error");
  } else{ // PARENT PROCESS
    do{
      waitpid(pid, &status, WUNTRACED); // TODO: may need to be wpid = waitpid
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

/* based off of Brennan
returns char* because allocation is done by getline
*/
char* snl_read_line(void){
	char* line; // todo: maybe needs to equal null??
	ssize_t buffersize = 0;
	getline(&line, &buffersize, stdin);
	return line;
}

/* Using slight adaption of Brennan's implementation of the
  main loop https://brennan.io/2015/01/16/write-a-shell-in-c/

*/
void snl_loop(void){
  char *line;
  char **args[MAXARGS];
  int status;

  do {
    printf("> ");
    line = snl_read_line();
    snl_split_line(line, args);
    status = snl_execute(args);

    free(line);
    free(args);
  } while (status);
}

/* Using Brennan's implementation of main
 https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
int main(int argc, char **argv){
  // Load config files, if any.

  // Run command loop.
  snl_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
