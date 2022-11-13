#include <stdio.h>
#include <stdlib.h>

int main()
{
    int childpid;
    if ((childpid=fork()) == -1)
    {
        perror("Cant fork");
        exit(1);
    }
    else if (childpid==0)
    {
        while(1)
        {
            printf("Parent says: my child pid: %d\n", getpid());
        }
    }
    else 
    {
        while(1)
        {
            printf("Child says: my parent pid: %d\n", getpid());
        }
    }
    return 0;
}