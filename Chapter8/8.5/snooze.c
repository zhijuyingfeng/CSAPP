#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

unsigned int snooze(unsigned int secs)
{
    unsigned int ret=sleep(secs);
    fprintf(stdout,"Slept for %d of %d secs.\n",secs-ret,secs);
    return ret;
}

void sigint_handler(int secs)
{
    //do nothing here to prevent process to be terminated
}

int main(int argc,char** argv)
{
    unsigned int secs;
    if(signal(SIGINT,sigint_handler)==SIG_ERR)
    {
        fprintf(stdout,"signal error\n");
        _exit(0);
    }
    if(argv[1]==NULL) //Ingore situation without argument
        return 1;
    secs=atoi(argv[1]);
    snooze(secs);
    return 0;
}