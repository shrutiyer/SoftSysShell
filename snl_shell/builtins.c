#include "exit.c"

// An array of strings with built-in command names
char* snl_builtins_names[] = {"cd", "exit"};

int (*builtin_func[]) (char **) = {
  &snl_exit
};

/*
	Calculates the number of built-in commands
	Inputs: Nothing
	Returns: Number of built-ins
*/
int snl_builtins_number() {
	return sizeof(snl_builtins_names)/sizeof(char*);
}
