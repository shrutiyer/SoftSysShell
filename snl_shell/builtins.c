/*
  All builtin functions written for SNL Shell

  Authors: LucyWilcox, nmohamed, shrutiyer
*/

#include "exit.c"
#include "manage_env.c"

// An array of strings with built-in command names
char* snl_builtins_names[] = {"exit", "setenv", "unsetenv", "getenv"};

int (*snl_builtin_func[]) (char**, char**) = {
  &snl_exit,
  &snl_setenv,
  &snl_unsetenv,
  &snl_getenv
};

/*
  Calculates the number of built-in commands
  Inputs: Nothing
  Returns: Number of built-ins
*/
int snl_builtins_number() {
  return sizeof(snl_builtins_names)/sizeof(char*);
}
