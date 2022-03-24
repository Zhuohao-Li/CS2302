#include <stdio.h>
#include <stdlib.h>

int main(argc, argv)
int argc;
char *argv[];
{
    int i;

    for (i = 0; i < argc; i++)
    {
        if (argv[i])
            printf("argv[%d]=%s\n", i, argv[i]);
        else
        {
            printf("argv[%d] unexpected NULL argument\n", i);
            break;
        }
    }
    exit(0);
}