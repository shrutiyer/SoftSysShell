/*
  Some misc functions useful to the functioning of SNL shell

  Authors: LucyWilcox, nmohamed, shrutiyer
*/



/*
  Returns your current working directory.
  Inputs: Nothing
  Returns: char* of path to your current working directory
*/
char* get_cwd() {
  long size = pathconf(".", _PC_PATH_MAX);
  char* buf, *ptr;
  buf = (char* )malloc((size_t)size);

  if (buf != NULL){
    ptr = getcwd(buf, (size_t)size);
  }

  return ptr;
}

/*
  Detects if array of arguments has a pipe as an arg
  Inputs: an array which will be used as to split cmds into args
  Returns: 0 if not piped, 1 if piped
*/

int snl_detect_pipe(char** args){
  for (int i = 0; i < MAXARGS; i++) {
    if(args[i] == NULL){
      return 0; // reached end, no pipe
    }
    if (strcmp(args[i],"|") == 0){
      return 1; // yes pipe
    }
  }
  return 0; // no pipe
}

int snl_detect_redirect(char** args) {
  for (int i = 0; i < MAXARGS; i++) {
    if(args[i] == NULL){
      return 0; // reached end, no redirect
    }
    if (strcmp(args[i],">") == 0){
      return i; // yes redirect
    }
  }
  return 0; // no redirect
}
