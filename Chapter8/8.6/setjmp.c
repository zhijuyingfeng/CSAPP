#include <setjmp.h>
#include <stdio.h>

jmp_buf buf;

int error1=0;
int error2=1;

void foo(void),bar(void);

int main()
{
    switch (setjmp(buf))
    {
    case 0:
        foo();
        break;
    case 1:
        fprintf(stdout,"Detected an error1 condition in foo.\n");
        break;
    case 2:
        fprintf(stdout,"Detected an error2 condition in foo.\n");
        break;
    default:
        fprintf(stdout,"Unknown error condition in foo.\n");
    }
    return 0;
}

void foo()
{
    if(error1)
        longjmp(buf,1);
    bar();
}

void bar()
{
    if(error2)
        longjmp(buf,2);
}