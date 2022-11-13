#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void print_status(int status, int child_pid, int i)
{
        printf("child process %d exited with status: %d\n", child_pid, status);
        if (WIFEXITED(status))
        {
            printf("Child process #%d finished.\t Code: %d\n", i + 1, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("child process %d finished by signal with code: %d\n", i + 1, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("child process %d stopped with signal: %d\n", i + 1, WSTOPSIG(status));
        }
}