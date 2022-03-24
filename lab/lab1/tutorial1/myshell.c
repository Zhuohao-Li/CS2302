/*

  OS Tutorial 1

  myshell - basic shell replacement

  usage:

    myshell

    reads in a line of keyboard input at a time, parsing it into
    tokens that are separated by white spaces (set by #define
    SEPARATORS).

    Internal Commands/Aliases:

    clr - clear the screen using the system function clear
    dir <directory> - list the current directory contents (ls -al <directory>)
    environ - list all the environment strings
    quit - quit from the program with a zero return value

    External Commands:

    All other command line inputs are relayed to a system shell for execution

 *********************************************************************
  version: 1.0
 *******************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 1024    // max line buffer
#define MAX_ARGS 64        // max # args
#define SEPARATORS " \t\n" // token sparators

extern char **environ;
/*******************************************************************/

int main(int argc, char **argv)
{
    char linebuf[MAX_BUFFER]; // line buffer
    char cmndbuf[MAX_BUFFER]; // command buffer
    char *args[MAX_ARGS];     // pointers to arg strings
    char **arg;               // working pointer thru args
    char *prompt = "==>";     // shell prompt

    // keep reading input until "quit" command or eof of redirected input
    while (!feof(stdin))
    {
        // get command line from input
        fputs(prompt, stdout); // write prompt
        fflush(stdout);
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
                cmndbuf[0] = 0; // set zero-length command string

                // check for internal/external command
                if (!strcmp(args[0], "clr"))
                {
                    // "clr" command
                    strcpy(cmndbuf, "clear");
                }
                else if (!strcmp(args[0], "dir"))
                {
                    // "dir" command
                    strcpy(cmndbuf, "ls -al ");
                    if (!args[1])
                        args[1] = "."; // if no arg set current directory
                    strcat(cmndbuf, args[1]);
                }
                else if (!strcmp(args[0], "environ"))
                {
                    // "environ" command
                    char **envstr = environ;
                    while (*envstr)
                    {
                        // print out environment
                        printf("%s\n", *envstr);
                        envstr++;
                    }
                    // (no entry in cmndbuf)
                }
                else if (!strcmp(args[0], "quit"))
                {
                    // "quit" command
                    break;
                }
                else
                {
                    // pass command on to OS shell
                    int i = 1;
                    strcpy(cmndbuf, args[0]);
                    while (args[i])
                    {
                        strcat(cmndbuf, " ");
                        strcat(cmndbuf, args[i++]);
                    }
                }

                // pass any command onto OS
                if (cmndbuf[0])
                    system(cmndbuf);
            }
        }
    }
    return 0;
}
