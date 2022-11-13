#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "util.h"

#define N 2
#define TIME_SLEEP 2

int main()
{
    setbuf(stdout, NULL);
    int child[N];
    char *com[N] = {"./3_1.out", "./3_2.out"};
    int status = 0;
    for (int i = 0; i < N; i++)
    {
        if ((child[i] = fork()) == -1)
        {
            perror("fork failed\n");
            exit(1);
        }
        else if (!child[i])
        {
            printf("Child %d! PID: %d, PPID: %d, GROUP: %d\n", i + 1, getpid(), getppid(), getpgrp());
            if (execl(com[i], com[i], NULL) == -1)
            {
                perror("Can't exec");
                exit(2);
            }
            return 0;
        }
    }
    for (int i = 0; i < N; i++)
    {
        status = 0;
        if (waitpid(child[i], &status, NULL) == -1)
            exit(4);
        print_status(status, child[i], i);
    }
    printf("Parent process finished! Children: %d, %d! \nParent: PID: %d, GROUP: %d\n ", child[0], child[1], getpid(), getpgrp());
    return 0;
}