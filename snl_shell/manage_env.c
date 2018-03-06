// Environment management built in functions

/*
  Sets new environment variable
  Inputs: char** of args (first is the key second is the value) and char** the current env
  Returns: 1
  
*/
int snl_setenv(char** args, char** environ) {
  // no value, only key
  if (args[1] == NULL) {
    printf("%s\n", "cannot setenv with no args");
  } else if (args[2] == NULL){
    // see https://linux.die.net/man/3/setenv
    setenv(args[1], "", 1);
  } else {
    setenv(args[1], args[2], 1);
  }

  return 1;
}

/*
  Removes environment variable
  Inputs: char** of args (args[1] is the key to be removed from the env) and char** the current env
  Returns: 1
  
*/
int snl_unsetenv(char** args, char** environ) {
  if (args[1] == NULL) {
    printf("%s\n", "cannot unsetenv with no args");
  } else if (getenv(args[1]) == NULL) {
    printf("%s\n", "cannot unset env arg that does not exist");
  } else {
    // see https://linux.die.net/man/3/unsetenv
    unsetenv(args[1]);
  }
  return 1;
}

/*
  Prints env
  Inputs: char** of args (which is not used) and char** environ
  Returns: 1

  Used this resource: https://github.com/jmreyes/simple-c-shell/blob/master/simple-c-shell.c
*/
int snl_getenv(char** args, char** environ) {
  char** env_elm;
  for (env_elm = environ; *env_elm != 0; env_elm ++ ){
    printf("%s\n", *env_elm);
  }
  return 1;
}