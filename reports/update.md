## SNL Shell
#### By Shruti Iyer, Nora Mohamed, and Lucy Wilcox

### Goals
#### Project Goals

The goal of our project to build a simple replica of a UNIX shell that can execute core commands frequently used in a shell. Our minimum viable product will be to build a shell that initializes, interprets commands and terminates. The shell will contain 3-5 built-in commands as well as non-built-in commands for users to use and execute.

The stretch goals are to add on to MVP commands to achieve functionality as close to its actual UNIX implementation, add piping and redirection functionality, make the shell secure and avoid any common security flaws, and write unit tests for our code. The success of our stretch goals depend on how far along we are with our MVP goals.

#### Learning Goals

As a group we want to become more proficient at C and comfortable writing efficient C programs. This, of course, includes documenting both the code and the project! Because we are learning about the shell and shell commands we also want to gain more skills with the UNIX command line.

### Implementation

Our first steps were to look at other implementations of shells as group and talk through them. After understanding other implementations, we could easily create a high-level plan for writing a shell. In terms of writing the first few lines of code, we started with writing the skeleton of a shell which includes initializing the shell loop, reading lines from the shell, parsing the lines, and executing commands, exiting the shell. Reading multiple implementations was very helpful for us because we were able to choose parts of each implementation that we liked. It also allowed us to reason about which were more appropriate for us to use. It would have been very difficult for us to write a shell without any resources, but if we had used only one resources and simply replicated we would not have learned as much. 

Resources that we found useful were:

[Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)
 
[Making your own Linux Shell in C](https://www.geeksforgeeks.org/making-linux-shell-c/)
 
[Simple C Shell](https://github.com/jmreyes/simple-c-shell)
 
[Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
 
Right now we are working on adding additional built in functions. Specifically Lucy is working on ‘help’ so our shell is easier to use, when done, this will include a help display for each built-in function we wrote and overarching help function which lists all implemented functions and how to use them. Nora is writing ‘cd’ so users can navigate, this will be complete when users can navigate to different folders. Shruti is implementing a simple version of redirections ‘>’ and organizing the code into reasonable blocks/files. We will each submit pull requests once our task is done so the rest of the group can confirm completion.
