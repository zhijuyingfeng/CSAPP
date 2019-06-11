#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

int main()
{
    int status;
    pid_t pid;

    fprintf(stdout,"Hello\n");
    pid=fork();
    fprintf(stdout,"%d\n",!pid);

    if(pid!=0)
    {
        if(waitpid(-1,&status,0)>0)
        {
            if(WIFEXITED(status)!=0)
            {
                fprintf(stdout,"%d\n",WEXITSTATUS(status));
            }
        }
    }
    fprintf(stdout,"Bye\n");
    _exit(2);
}