/*
  File containing all fork functions for regular, pipes and redirect forking
  for SNL Shell

  Authors: LucyWilcox, nmohamed, shrutiyer
*/


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
      } else {
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

/*
  Extracts the first part of the redirect command and the index of the ">" symbol

  Inputs: The user-inputted line of command stored in an array of arrays
          char *foo[] placeholder to store the first-half of the command
          int laceholder to store the index of the ">" symbol
*/

void get_command(char* args[], char* command[], int redirect_index) {
  for (int i=0; i<redirect_index; i++) {
    command[i] = args[i];
  }
  command[redirect_index] = '\0';
}

/*
  For a redirect command, launches a child process and stores the output in the
  user-given file name

  Inputs: The user-inputted line of command stored in an array of arrays
          Index of the redirect command ">" in the args array
  Returns: 1
*/
int snl_fork_redirect(char* args[], int redirect_index) {
  int file_descriptor;
  char* filename = args[redirect_index+1];
  char* command[100];

  get_command(args, command, redirect_index);

  pid_t pid = fork();
  if (pid == -1) {
    perror("snl: Fork could not create a child process\n");
  } else if (pid == 0) {

    // Open a  file to write to it
    file_descriptor = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (file_descriptor < 0) {
      perror("snl: Incorrect file given for redirect");
    }
    // Replace standard output with appropriate file
    dup2(file_descriptor, 1);
    close(file_descriptor);

    if (execvp(command[0], command) == -1){
      perror("snl: fork redirect did not execute correctly");
      kill(getpid(),SIGTERM);
    }
  }
  waitpid(pid, NULL, 0);
}
