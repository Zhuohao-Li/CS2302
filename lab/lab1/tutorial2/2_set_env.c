#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    char *path = getenv("PATH");
    printf("PATH=%s\n", path);

    int ret = setenv("PATH", "hello", 1);
    if (ret == 0)
    {
        printf("set environment variable path: ok!\n");
    }
    else
    {
        printf("set environment variable path: fail!\n");
    }
    printf("PATH=%s\n", getenv("PATH"));

    ret = putenv("hello=world");
    if (ret == 0)
    {
        printf("set environment variable hello: ok!\n");
        char *hello = getenv("hello");
        printf("hello=%s\n", hello);
    }
    return 0;
}
