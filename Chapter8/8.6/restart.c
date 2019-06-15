#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include "SIO.h"

sigjmp_buf buf;

void handler(int sig)
{
    siglongjmp(buf,1);
}

int main()
{
    if(!sigsetjmp(buf,1))
    {
        signal(SIGINT,handler);
        sio_puts("starting...\n");
    }
    else
        sio_puts("restarting...\n");
    
    while(1)
    {
        sleep(1);
        sio_puts("processing...\n");
    }
    _exit(0);                           //Control never reach here
}