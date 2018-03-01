// #include "snl.c"

/*
  
*/
int snl_setenv(char **args, char** environ) {
  // no value, only key
  if (args[1] == NULL) {
    printf("%s\n", "cannot setenv with no args");
    return 1;
  }

  if (args[2] == NULL){
    // https://linux.die.net/man/3/setenv
    setenv(args[1], "", 1);
  } else {
    setenv(args[1], args[2], 1);
  }

  return 1;
}

int snl_unsetenv(char **args, char** environ) {
  return 1;
}

/*
  Prints env
  Inputs: char** of other args (which is not used) and char** environ
  Returns: 1

  Used this resource: https://github.com/jmreyes/simple-c-shell/blob/master/simple-c-shell.c
*/
int snl_getenv(char **args, char** environ) {
  char** env_elm;
  for (env_elm = environ; *env_elm != 0; env_elm ++ ){
    printf("%s\n", *env_elm);
  }
  return 1;
}