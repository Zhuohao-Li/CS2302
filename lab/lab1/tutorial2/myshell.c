/*

  OS Tutorial 2

  myshell - basic shell replacement

  usage:

    myshell

    reads in a line of keyboard input at a time, parsing it into
    tokens that are separated by white spaces (set by #define
    SEPARATORS).

    Prompt contains current working directory.

    Internal Commands/Aliases:

    cd - change dirtectory (PWD environment variable updated)
    clr - clear the screen using the system function clear
    dir <directory> - list the current directory contents (ls -al <directory>)
    environ - list all the environment strings
    quit - quit from the program with a zero return value

    External Commands:

    All other command line inputs are executed via a fork and exec

 *********************************************************************
  version: 1.1
  date:    December 2003
  author:  Dr Ian G Graham, ian.graham@griffith.edu.au
  history:
    v1.0:  clr, dir, environ, quit
    v1.1
         - Add cd command, PWD alteration, and working directory in prompt syserrmsg
         - Add fork and exec capability via execute function
 *******************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAX_BUFFER 1024    // max line buffer
#define MAX_ARGS 64        // max # args
#define SEPARATORS " \t\n" // token sparators

extern char **environ;

void execute(char **, int);
void syserrmsg(char *, char *); // error message printout

/*******************************************************************/

int main(int argc, char **argv)
{
    char linebuf[MAX_BUFFER]; // line buffer
    char cwdbuf[MAX_BUFFER];  // cwd buffer
    char *args[MAX_ARGS];     // pointers to arg strings
    char **arg;               // working pointer thru args
    char *prompt = "==>";     // shell prompt
    int foreground = TRUE;    // run commands in foreground mode
    int execargs;             // execute command in args

    // keep reading input until "quit" command or eof of redirected input
    while (!feof(stdin))
    {
        // get command line from input
        if (fgets(linebuf, MAX_BUFFER, stdin))
        {
            // read a line
            // tokenize the input into args array
            arg = args;
            *arg++ = strtok(linebuf, SEPARATORS); // tokenize input
            while ((*arg++ = strtok(NULL, SEPARATORS)))
                ;
            // last entry will be NULL
            if (args[0])
            {
                // if there's anything there
                execargs = TRUE; // set default execute of args

                // check for internal/external command
                // "cd" command
                if (!strcmp(args[0], "cd"))
                {
                    // Tutorial 2
                    // TODO
                    execargs = FALSE; // no need for further exec
                }
                else
                    // "clr" command
                    if (!strcmp(args[0], "clr"))
                    {
                        // Tutorial 2
                        // TODO
                    }
                    else
                        // "dir" command
                        if (!strcmp(args[0], "dir"))
                        {
                            // Tutorial 2
                            // TODO
                        }
                        else
                            // "environ" command
                            if (!strcmp(args[0], "environ"))
                            {
                                // Tutorial 2
                                // TODO
                                execargs = FALSE; // no need for further exec
                            }
                            else
                                // "quit" command
                                if (!strcmp(args[0], "quit"))
                                {
                                    // Tutorial 2
                                    // TODO
                                }
                // else pass command on to OS shell

                if (execargs)
                    execute(args, foreground);
            }
        }
    }
    return 0;
}

/***********************************************************************

  void execute(char ** args, int foreground);

  fork and exec the program and command line arguments in args
  if foreground flag is TRUE, wait until pgm completes before
    returning

***********************************************************************/

void execute(char **args, int foreground)
{
    // Tutorial 2
    // TODO
}

/********************************************************************

  void syserrmsg(char * msg1, char * msg2);

  print an error message (or two) on stderr followed by system error
  message.

  if msg2 is NULL only msg1 and system message is printed
  if msg1 is NULL only the system message is printed
 *******************************************************************/

void syserrmsg(char *msg1, char *msg2)
{
    if (msg1)
        fprintf(stderr, "ERROR - %s ", msg1);
    if (msg2)
        fprintf(stderr, "%s ", msg2);
    fprintf(stderr, "; ");
    perror(NULL);
    return;
}
