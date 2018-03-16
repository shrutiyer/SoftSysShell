# SoftSys Shell Project: SNL_SHELL
### Shruti Iyer, Nora Mohamed, Lucy Wilcox
 
## Project Goals
 
The goal of our project to build a simple replica of a UNIX shell that can execute core commands frequently used in a shell. Our minimum viable product will be to build a shell that initializes, interprets commands and terminates. The shell will contain 3-5 built-in commands as well as non-built-in commands for users to use and execute.
 
The stretch goals are to add on to MVP commands to achieve functionality as close to its actual UNIX implementation, add piping and redirection functionality, make the shell secure and avoid any common security flaws, and write unit tests for our code. The success of our stretch goals depend on how far along we are with our MVP goals.
 
## Learning Goals
 
As a group we want to become more proficient at C and comfortable writing efficient C programs. This, of course, includes documenting both the code and the project! Because we are learning about the shell and shell commands we also want to gain more skills with the UNIX command line.
 
## Implementation
 
Our first steps were to look at other implementations of shells as group and talk through them. After understanding other implementations, we could easily create a high-level plan for writing a shell. In terms of writing the first few lines of code, we started with writing the skeleton of a shell which includes initializing the shell loop, reading lines from the shell, parsing the lines, and executing commands, exiting the shell. Reading multiple implementations was very helpful for us because we were able to choose parts of each implementation that we liked. It also allowed us to reason about which were more appropriate for us to use. It would have been very difficult for us to write a shell without any resources, but if we had used only one resources and simply replicated we would not have learned as much. Resources that we found useful were:
 
[Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)

[Making your own Linux Shell in C](https://www.geeksforgeeks.org/making-linux-shell-c/)

[Simple C Shell](https://github.com/jmreyes/simple-c-shell)

[Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell)
 
We wrote the following built-in functions: exit, cd, help, setenv, unsetenv, getenv. We wrote functionality to allow the shell to do infinite piping, as well as simple redirection. The shell has environment variable functionality as well. We originally were going to write non-built in but because these already exist in our shell we thought it would be more valuable and rewarding to add more built in functionality instead of duplicating these non-built in functions. The help function can be used to get more details on the built in functions that we wrote, and an example usage of some of our functionality is shown next.
 
### Redirection, piping, and environment variables example:
To run this example, navigate to the snl_shell folder of our repository. Create a file named test.txt and populate it with:
```
hi
hello
hi
apple
```
Next enter the following command into your terminal:
```
gcc snl.c -o snl && ./snl
```

```
/mnt/c/Users/nmohamed/Documents/GitHub/SoftSysShell/snl_shell> setenv FILE test.txt
/mnt/c/Users/nmohamed/Documents/GitHub/SoftSysShell/snl_shell> cat test.txt
hi
hello
hi
apple
/mnt/c/Users/nmohamed/Documents/GitHub/SoftSysShell/snl_shell> cat $FILE | sort | uniq
apple
hello
hi
/mnt/c/Users/nmohamed/Documents/GitHub/SoftSysShell/snl_shell> echo "hi" > $FILE
/mnt/c/Users/nmohamed/Documents/GitHub/SoftSysShell/snl_shell> cat $FILE
"hi" 
```

## Reflection
 
We primarily focused on pacing our work and gaining a greater understanding of the C programming language. We tried to implement good practices (like including a header file) and made sure our code was well documented. We paced our work well, making sure that the work was well planned out and delegated accordingly. We focused on our stretch goals after accomplishing the base functionality of the shell, and are satisfied with the stretch goals we did accomplish. We also understand C much better than we did originally (e.g., pointers, double pointers, various function calls). We didn’t gain any revolutionary new understanding of the UNIX command line, but it was a good reinforcement of knowledge already learned.
 
### Example of pointer magic we learned
```
int (*snl_builtin_func[]) (char**, char**) = {
  &snl_exit,
  &snl_setenv,
  &snl_unsetenv,
  &snl_getenv,
  &snl_cd
};
```
`snl_builtin_func` is an array of function pointers, taking in `args` and an array of environment variables, returning an int. It is run as `*snl_builtin_func[builtin_name](args, environ)`.
