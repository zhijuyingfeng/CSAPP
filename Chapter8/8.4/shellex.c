#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAXARGS 128
#define MAXLINE 8192

extern char **environ;

void eval(char* cmdline);
int parseline(char*buf,char** argv);
int builtin_command(char** argv);

int main()
{
    char cmdline[MAXLINE];

    while(1)
    {
        fprintf(stdout,">");
        fgets(cmdline,MAXLINE,stdin);
        if(feof(stdin))
            _exit(0);

        eval(cmdline);
    }
}

void eval(char *cmdline)
{
    char *argv[MAXARGS];    //Argument list execve()
    char buf[MAXLINE];      //Holds modified command line
    int bg;                 //Should the job run bg or fg?
    pid_t pid;              //Process id

    strcpy(buf,cmdline);
    bg=parseline(buf,argv);
    if(argv[0]==NULL)
        return;             //Ingore empty lines

    if(!builtin_command(argv))
    {
        if((pid=fork())==0) //Child runs user job
        {
            if(execve(argv[0],argv,environ)<0)
            {
                fprintf(stdout,"%s: Command not found.\n",argv[0]);
                _exit(0);
            }
        }

        //Parent waits for foreground job to terminate
        if(!bg)
        {
            int status;
            if(waitpid(pid,&status,0)<0)
            {
                fprintf(stdout,"waitfg: waitpid error\n");
                _exit(1);
            }
        }
        else
            fprintf(stdout,"%d %s",pid,cmdline);
    }
}

//If first arg is a builtin command,run it and return true
int builtin_command(char **argv)
{
    if(!strcmp(argv[0],"quit"))     //quit command
        _exit(0);
    if(!strcmp(argv[0],"&"))        //Ingore singleton &
        return 1;
    return 0;
}

//parseline - Parse the command line and build the argv array
int parseline(char* buf, char** argv)
{
    char *delim;        //Points to first space delimiter
    int argc;           //Number of args
    int bg;             //Background job?

    buf[strlen(buf)-1]=' ';     //Replace trailing '\n' with space
    while(*buf && (*buf==' '))  //Ingore leading spaces
        buf++;
    
    //Build the argv list
    argc=0;
    while((delim=strchr(buf,' ')))
    {
        argv[argc++]=buf;
        *delim='\0';
        buf=delim+1;
        while(*buf && (*buf==' '))  //Ingore spaces
            buf++;
    }
    argv[argc]=NULL;

    if(!argc)                       //Ingore blank line
        return 1;
    
    //Should job run in the background?
    if(bg=(*argv[argc-1]=='&'))
        argv[--argc]=NULL;
    return bg;
}