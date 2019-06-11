#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>

#define N 2

int main()
{
	int status,i;
	pid_t pid;

	//Parent create N children
	for(i=0;i<N;i++)
	{
		if((pid=fork())==0)//Child
			_exit(100+i);
	}
	while((pid=waitpid(-1,&status,0))>0)
	{
		if(WIFEXITED(status))
			fprintf(stdout,"child %d terminated normally with exit status=%d\n",
					pid,WEXITSTATUS(status));
		else
			fprintf(stdout,"child %d terminated abnormally\n",pid);
	}

	/*The only normal termination is if there are no more children*/
	if(errno!=ECHILD)
		fprintf(stdout,"waitpid error!\n");
	_exit(0);
}
