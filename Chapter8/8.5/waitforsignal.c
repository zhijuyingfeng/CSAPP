#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>

volatile sig_atomic_t pid;

void sigchld_handler(int s)
{
    int olderrno=errno;
    pid=waitpid(-1,NULL,0);
    errno=olderrno;
}

void sigint_handler(int s)
{}

int main(int argc,char** argv)
{
    sigset_t mask,prev;

    signal(SIGINT,sigint_handler);
    signal(SIGCHLD,sigchld_handler);
    sigemptyset(&mask);
    sigaddset(&mask,SIGCHLD);

    while(1)
    {
        sigprocmask(SIG_BLOCK,&mask,&prev);         //Block SIGCHLD
        if(fork()==0)                               //Child
        {
            sleep(1);
            _exit(0);
        }

        //Parent
        pid=0;
        sigprocmask(SIG_SETMASK,&prev,NULL);        //Unblock SIGCHLD

        while(!pid);                                //Wait for SIGCHLD to be received(wasteful)
    
        fprintf(stdout,".\n");                      //Do something after receiving SIGCHLD
    }

    _exit(0);
}