/*Warning: This code is buggy!!! */
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <errno.h>
#include <string.h>
#include "SIO.h"

#define MAX_PROCESS_NUM 4

pid_t process_list[MAX_PROCESS_NUM];
int using[MAX_PROCESS_NUM];

void initjobs()
{
    memset(process_list,0,sizeof(pid_t)*MAX_PROCESS_NUM);
    memset(using,0,sizeof(int)*MAX_PROCESS_NUM);
}

void show_process_list()
{
    int i;

    for(i=0;i<MAX_PROCESS_NUM;i++)
    {
        if(using[i])
        {
            fprintf(stdout,"%d ",process_list[i]);
        }
    }
    fprintf(stdout,"\n");
}

void deletejob(pid_t pid)
{
    int i;
    fprintf(stdout,"delete job %d.\n",pid);
    for(int i=0;i<MAX_PROCESS_NUM;i++)
    {
        if(process_list[i]==pid)
        {
            using[i]=0;
            return;
        }
    }
}

void addjob(pid_t pid)
{
    int i;
    fprintf(stdout,"add job %d.\n",pid);
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

void handler(int sig)
{
    int olderrno=errno;
    sigset_t mask_all,prev_all;
    pid_t pid;

    sigfillset(&mask_all);
    while((pid=waitpid(-1,NULL,0))>0)       //Reap a zombie child
    {
        sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
        deletejob(pid);                     //Delete the child from the job list
        sigprocmask(SIG_SETMASK,&prev_all,NULL);
    }
    if(errno!=ECHILD)
        sio_error("waitpid error!\n");
    errno=olderrno;
}

int main(int argc,char** argv)
{
    pid_t pid;
    int i;
    sigset_t mask_all,prev_one,mask_one;

    sigfillset(&mask_all);
    sigemptyset(&mask_one);
    sigaddset(&mask_one,SIGCHLD);
    signal(SIGCHLD,handler);
    initjobs();         //Initialize the job list

    for(i=0;i<MAX_PROCESS_NUM;i++)
    {
        sigprocmask(SIG_BLOCK,&mask_one,&prev_one);     //Block SIGCHLD
        if((pid=fork())==0)                             //Child process
        {
            sigprocmask(SIG_SETMASK,&prev_one,NULL);    //Unblock SIGCHLD
            // execve("/bin/date",argv,NULL);
            sleep(1);
            fprintf(stdout,"Child process ends.\n");
            _exit(0);
        }
        sigprocmask(SIG_BLOCK,&mask_all,NULL);
        addjob(pid);
        sigprocmask(SIG_SETMASK,&prev_one,NULL);        //Unblock SIGCHLD
    }
    sleep(1);
    _exit(0);
}