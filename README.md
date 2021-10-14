## COMPILING THE CODE
Run make in the directory containg all my shell files.
## EXECUTION
Run ./shshell in the same directory
## INFO ABOUT THE CODE
1)`shshell.c`:It contains the main function of my program and function to take input and divide it in tokens.
2)`headers.h`:It contains all the headers,global variables used in program and various functions are defined which are used all over the project.
3)`printprompt.c`:It contains the function which prints prompt for the shell.
4)`inbuilt.c`:It contains functions for all the commands that were asked to execute in asssignment.
5)`foreground.c`:It contains the function to run any other process in the foreground.
6)`background.c`:It contains the function to run process in background.
7)`jobs.c`:It contains jobs,sig,fg,bg command functions and signal handling.
8)`piping.c`:It take cares of piping and redirection.