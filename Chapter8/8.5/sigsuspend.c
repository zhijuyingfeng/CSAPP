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
    fprintf(stdout,"process %d deleted!\n",pid);
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
            fprintf(stdout,"process %d added!\n",getpid());
            sleep(1);
            _exit(0);
        }

        //Wait for SIGCHLD to be received
        pid=0;
        while(!pid)
        {
            sigsuspend(&prev);
        }
    
        sigprocmask(SIG_SETMASK,&prev,NULL);        //Optionally unblock SIGCHLD

        fprintf(stdout,".\n");                      //Do something after receiving SIGCHLD
    }

    _exit(0);
}