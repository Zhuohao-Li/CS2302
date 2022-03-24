#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(argc, argv)
int argc;
char *argv[];
{
    char **env = environ;
    while (*env)
        printf("%s\n", *env++);
    exit(0);
}