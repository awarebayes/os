#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int N = 2;
const int TIME_SLEEP = 2;

int main()
{
    int child[N];
    for (int i = 0; i < N; i++)
    {
        if ((child[i] = fork()) == -1)
        {
            perror("fork failed\n");
            exit(1);
        }
        else if (!child[i])
        {
            printf("Child %d! PID: %d, PPID: %d, GROUP: %d \n", i + 1, getpid(), getppid(), getpgrp());
            sleep(TIME_SLEEP);
            printf("Child %d! PID: %d, PPID: %d, GROUP: %d\n", i + 1, getpid(), getppid(), getpgrp());
            return 0;
        }
        else 
        {
            printf("[parent] children: %d, %d! \nParent: PID: %d, GROUP: %d\n", child[0], child[1], getpid(), getpgrp());
        }
    }
    return 0;
}