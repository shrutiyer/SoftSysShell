
/*
SNL (Shruti, Nora, Lucy) Shell
Created for Software Systems project 1

*/


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

void snl_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = snl_read_line();
    args = snl_split_line(line);
    status = snl_execute(args);

    free(line);
    free(args);
  } while (status);
}

// Using Brennan's implementation of main
// https://brennan.io/2015/01/16/write-a-shell-in-c/
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  snl_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
