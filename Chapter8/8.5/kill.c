#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int main()
{
    pid_t pid;

    //Child sleeps until SIGKILL signal received,then dies
    if(!(pid=fork()))
    {
        pause();
        fprintf(stdout,"Control should never reach here!\n");
        _exit(0);
    }

    //Parent sends a SIGKILL signal to a child
    kill(pid,SIGKILL);
    _exit(0);
}