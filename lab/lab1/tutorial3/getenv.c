#include <stdio.h>
#include <stdlib.h>

int main(argc, argv)
int argc;
char *argv[];
{
    char *cptr;

    if (argc > 1)
        if (!(cptr = getenv(argv[1])))
            printf("environment variable %s does not exist\n", argv[1]);
        else
            printf("%s=%s\n", argv[1], getenv(argv[1]));
    else
        printf("must have environment variable name as argument\n");
    exit(0);
}