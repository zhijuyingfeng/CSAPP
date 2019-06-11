#include <stdio.h>

int main(int argc,char**argv,char** envp)
{
    int i=0;
    fprintf(stdout,"Command-line arguments:\n");
    for(i=0;i<argc;i++)
    {
        fprintf(stdout,"argv[%d]: %s\n",i,argv[i]);
    }

    // while(argv[i]!=NULL)
    // {
    //     fprintf(stdout,"argv[%d]: %s\n",i++,argv[i]);
    // }

    fprintf(stdout,"Environment variables:\n");
    
    i=0;
    while(envp[i]!=NULL)
    {
        fprintf(stdout,"envp[%d]: %s\n",i++,envp[i]);
    }
}