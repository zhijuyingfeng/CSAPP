/*Warning: This code is buggy!!! */
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <errno.h>
#include <string.h>
#include "SIO.h"

#define MAX_PROCESS_NUM 100

pid_t process_list[MAX_PROCESS_NUM];
int using[MAX_PROCESS_NUM];

void initjobs()
{
    memset(process_list,0,sizeof(pid_t)*MAX_PROCESS_NUM);
    memset(using,0,sizeof(int)*MAX_PROCESS_NUM);
}

void addjob(pid_t pid)
{
    int i;
    for(i=0;i<MAX_PROCESS_NUM;i++)
    {
        if(!using[i])
        {
            process_list[i]=pid;
            using[i]=1;
            return;
        }
    }
}

void deletejob(pid_t pid)
{
    int i;
    for(int i=0;i<MAX_PROCESS_NUM;i++)
    {
        if(process_list[i]==pid)
        {
            using[i]=0;
            return;
        }
    }
}

void handler(int sig)
{
    int olderrno=errno;
    sigset_t mask_all,prev_all;
    pid_t pid;

    sigfillset(&mask_all);
    while((pid=waitpid(-1,NULL,0))>0)       //Reap a zombie child
    {
        sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
        deletejob(pid);//Delete the child from the job list
        sigprocmask(SIG_SETMASK,&prev_all,NULL);
    }
    if(errno!=ECHILD)
        sio_error("waitpid error!\n");
    errno=olderrno;z
}

int main(int argc,char** argv)
{
    int pid;
    sigset_t mask_all,prev_all;

    sigfillset(&mask_all);
    signal(SIGCHLD,handler);
    initjobs();         //Initialize the job list

    while(1)
    {
        if((pid=fork())==0)         //Child process
        {
            execve("/bin/date",argv,NULL);
        }
        sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
        addjob(pid);
        sigprocmask(SIG_SETMASK,&prev_all,NULL);
    }
    _exit(0);
}