#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0)
    {
        char *args[3] = {"echo", "Hello, Child!", NULL};
        execvp("echo", args);
    }
    else
    {
        int stat_val;
        waitpid(pid, &stat_val, 0);
        if (WIFEXITED(stat_val))
            printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
        else if (WIFSIGNALED(stat_val))
            printf("Child terminated abnormally, signal %d\n", WTERMSIG(stat_val));
    }
    return 0;
}