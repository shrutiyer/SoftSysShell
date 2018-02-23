
/*
SNL (Shruti, Nora, Lucy) Shell
Created for Software Systems project 1

*/

#include <stdio.h>
#include <string.h>

int MAXARGS = 100;


// splits line into set of args
void lsh_split_line(char* line, char** args){
	for (int i = 0; i < MAXARGS) {
		// get each arg seperated by space
		args[i] = strsep(&line, " "); 

		if (parsed[i] == NULL){
			break; // end of line
		} else if (strlen(parsed[i]) == 0){
			i--; // to re-write over empty arg
		}
	}
}


// Using slight adaption of Brennan's implementation of the
//  main loop https://brennan.io/2015/01/16/write-a-shell-in-c/
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

// Using Brennan's implementation of main
// https://brennan.io/2015/01/16/write-a-shell-in-c/
int main(int argc, char **argv){
  // Load config files, if any.

  // Run command loop.
  snl_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}