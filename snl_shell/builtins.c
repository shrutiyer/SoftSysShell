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

int snl_help(char **args, char** environ) {
  if (args[1] == NULL) {
    puts("\n");
    puts("This is a shell by Nora, Lucy, and Shruti for Software Systems, Spring 2018");
    puts("Built-in functions implemented are:");
    for (int i = 0; i < sizeof(snl_builtins_names)/sizeof(char*); i++) {
      printf("  %s\n", snl_builtins_names[i]);
    }
    puts("For more help with these functions, call help followed by the built-in function of your choice");
    puts("\n");
  } else if (!strcmp(args[1], "exit")){
    puts("\n");
    puts("exit takes no args, call exit to end the program");
    puts("\n");
  } else if (!strcmp(args[1], "setenv")){
    puts("\n");
    puts("Setenv adds a key/value pair to the enviroment, these values can be used later with $key.");
    puts("A call to setenv is structured:");
    puts("  setenv key val");
    puts("For example:");
    puts("  setenv a 4");
    puts("Would add the value a to the enviroment with value 4");
    puts("Calling:");
    puts("  echo $a");
    puts("Would then yield 4");
    puts("\n");
  } else if (!strcmp(args[1], "unsetenv")){
    puts("\n");
  } else {
    printf("There is no built-in function %s\n", args[1]);
  }
  return 1;
}