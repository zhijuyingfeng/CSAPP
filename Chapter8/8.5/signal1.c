//Theis code is BUGGY!!!
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "SIO.h"

#define MAXBUF   8192  /* Max I/O buffer size */

void handler1(int sig)
{
    int olderrno=errno;
    if((waitpid(-1,NULL,0))<0)
    {
        sio_error("waitpid error!\n");
    }
    sio_puts("Handler reaped child.\n");
    sleep(1);
    errno=olderrno;
}

void handler2(int sig)
{
    int olderrno=errno;
    while((waitpid(-1,NULL,0))>0)
    {
        sio_puts("Handler reaped child.\n");
    }
    if(errno!=ECHILD)
        sio_error("waitpid error!\n");
    sleep(1);
    errno=olderrno;
}

int main()
{
    int n,i;
    char buf[MAXBUF];
    
    if(signal(SIGCHLD,handler2)==SIG_ERR)//Install SIGCHLD handler
    {
        sio_error("signal error\n");
    }

    //Parent create children
    for(i=0;i<3;i++)
    {
        if(fork()==0)
        {
            fprintf(stdout,"Hello from child %d.\n",(int)getpid());
            _exit(0);
        }
    }

    //Parent wait for terminal input and then processes it
    if((n=read(STDIN_FILENO,buf,sizeof(buf)))<0)
    {
        sio_error("read\n");
    }

    fprintf(stdout,"Parent processing input.\n");
    while(1);
    _exit(0);
}