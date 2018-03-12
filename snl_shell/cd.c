/*
  snl_cd: Runs "cd" command and changes current working director in the shell.
  Inputs: Aruments to command, environment variables
  Returns: 1 on success, -1 on error (because 0 exits)
*/
int snl_cd(char **args, char** environ) {
  if (args[1] == NULL){
    perror("snl: expected argument");
    return -1;
  } else {
    if (chdir(args[1]) != 0){
      perror("snl");
      return -1;
    }
  }
  return 1;
}
