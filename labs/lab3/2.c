#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "util.h"

const int N = 2;
const int TIME_SLEEP = 2;

int main()
{
    int child[N];
    int i = 0;
    int status = 0;

    for (i = 0; i < N; i++)
    {

        if ((child[i] = fork()) == -1)
        {
            perror("fork failed\n");
            exit(1);
        }
        else if (!child[i])
        {
            printf("Child %d!\t\t PID: %d, PPID: %d,\t GROUP: %d\n", i + 1, getpid(), getppid(), getpgrp());
            return 0;
        }
        else
        {
            printf("Parent process. Children: %d, %d! \nParent: PID: %d, GROUP: %d\n ", child[0], child[1], getpid(), getpgrp());
        }
    }
    for (i = 0; i < N; i++)
    {
        status = 0;
        if (waitpid(child[i], &status, NULL) == -1)
            exit(4);
        print_status(status, child[i], i);
    }
    return 0;
}