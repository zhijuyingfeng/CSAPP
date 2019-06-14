#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "SIO.h"

void sigint_handler(int sig)//SIGINT handler
{
    // fprintf(stdout,"CAUGHT SIGINT!\n");
    sio_puts("CAUGHT SIGINT!\n");       //Secure version
    _exit(0);
}

int main()
{
    //Install the SIGINT handler
    if(signal(SIGINT,sigint_handler)==SIG_ERR)
    {
        fprintf(stderr,"signal error!\n");
    }

    pause();//Wait for the receipt of a signal

    return 0;
}