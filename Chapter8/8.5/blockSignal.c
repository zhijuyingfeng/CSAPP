#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

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

int main()
{
    sigset_t mask,prev_mask;

    //Install the SIGINT handler
    if(signal(SIGINT,sigint_handler)==SIG_ERR)
    {
        fprintf(stdout,"sinal error!\n");
        _exit(0);
    }

    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    //BLOCK SIGINT and save previous blocked set
    sigprocmask(SIG_BLOCK,&mask,&prev_mask);
    
    //Test wheather SIGINT is blocked
    snooze(5);

    //Restore previous blocked set, unblocking SIGINT
    sigprocmask(SIG_SETMASK,&prev_mask,NULL);

    //Test wheather SIGINT is blocked
    snooze(5);
    return 0;
}