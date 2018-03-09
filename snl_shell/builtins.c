/*
  All builtin functions written for SNL Shell

  Authors: LucyWilcox, nmohamed, shrutiyer
*/

#include "exit.c"
#include "manage_env.c"

int snl_help(char **args, char** environ);

// An array of strings with built-in command names
char* snl_builtins_names[] = {"exit", "setenv", "unsetenv", "getenv", "help"};

int (*snl_builtin_func[]) (char**, char**) = {
  &snl_exit,
  &snl_setenv,
  &snl_unsetenv,
  &snl_getenv,
  &snl_help
};

/*
  Calculates the number of built-in commands
  Inputs: Nothing
  Returns: Number of built-ins
*/
int snl_builtins_number() {
  return sizeof(snl_builtins_names)/sizeof(char*);
}

/*
  Provides help information for builtin functions
  Inputs: char** of args (the function help is being called about - optional) and char** the current env
  Returns: 1
*/
int snl_help(char **args, char** environ) {
  if (args[1] == NULL) {
    puts("\nThis is a shell by Nora, Lucy, and Shruti for Software Systems, Spring 2018");
    puts("Built-in functions implemented are:");
    for (int i = 0; i < sizeof(snl_builtins_names)/sizeof(char*); i++) {
      printf("  %s\n", snl_builtins_names[i]);
    }
    puts("For more help with these functions, call help followed by the built-in function of your choice\n");
  } else if (!strcmp(args[1], "exit")){
    puts("\nExit ends the current program");
    puts("A call to exit is structured:");
    puts("  exit");
    puts("It takes no args\n");
  } else if (!strcmp(args[1], "setenv")){
    puts("\n Setenv adds a key/value pair to the enviroment, these values can be used later with $key.");
    puts("A call to setenv is structured:");
    puts("  setenv key val");
    puts("For example:");
    puts("  setenv a 4");
    puts("Would add the value a to the enviroment with value 4");
    puts("Calling:");
    puts("  echo $a");
    puts("Would then yield 4\n");
  } else if (!strcmp(args[1], "unsetenv")){
    puts("\nUnsetenv removes an enviroment variable. It can only remove one variable at a time");
    puts("A call to unsetenv is structured");
    puts("  unsetenv key");
    puts("For example:");
    puts("  unsetenv a");
    puts("Would remove a from the enviroment if it is an enviroment variable when called\n");
  } else if (!strcmp(args[1], "getenv")){
    puts("\nGetenv prints all enviroment variables");
    puts("A call to getenv is structured");
    puts("  getenv");
    puts("It takes no args\n");
  } else if (!strcmp(args[1], "help")){
    puts("\nHelp gives you information about built-in functions");
    puts("A call to help is structured either:");
    puts("  help \n or \n   help function_name");
    puts("For example:");
    puts("  help setenv");
    puts("Would give you information about the built-in function setenv\n");
  } else {
    printf("There is no built-in function %s\n", args[1]);
  }
  return 1;
}