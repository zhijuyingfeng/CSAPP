#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    if(!fork())
    {
        printf("a");
        fflush(stdout);
    }
    else
    {
        printf("b");
        fflush(stdout);
        waitpid(-1,NULL,0);
    }
    printf("c");
    fflush(stdout);
    _exit(0);
}