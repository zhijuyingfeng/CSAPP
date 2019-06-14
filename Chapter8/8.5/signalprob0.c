#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include "SIO.h"

volatile long counter = 2;

void handler1(int sig)
{
    sigset_t mask,prev_mask;

    sigfillset(&mask);
    sigprocmask(SIG_BLOCK,&mask,&prev_mask);        //Block all sigals
    sio_putl(--counter);
    sigprocmask(SIG_SETMASK,&prev_mask,NULL);       //Restore signals
    
    _exit(0);
}

int main()
{
    pid_t pid;
    sigset_t mask,prev_mask;

    fprintf(stdout,"%ld\n",counter);
    fflush(stdout);

    signal(SIGUSR1,handler1);

    if((pid=fork())==0)
    {
        while(1);
    }

    kill(pid,SIGUSR1);
    waitpid(-1,NULL,0);

    sigfillset(&mask);
    sigprocmask(SIG_BLOCK,&mask,&prev_mask);        //Block all sigals
    fprintf(stdout,"%ld\n",++counter);
    sigprocmask(SIG_SETMASK,&prev_mask,NULL);       //Restore signals

    _exit(0);
}