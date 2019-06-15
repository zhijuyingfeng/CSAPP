#include <unistd.h>

static size_t sio_strlen(char s[])
{
    int i = 0;

    while (s[i] != '\0')
        ++i;
    return i;
}

static void sio_reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = sio_strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

static void sio_ltoa(long v, char s[], int b) 
{
    int c, i = 0;
    int neg = v < 0;

    if (neg)
	v = -v;

    do {  
        s[i++] = ((c = (v % b)) < 10)  ?  c + '0' : c - 10 + 'a';
    } while ((v /= b) > 0);

    if (neg)
	s[i++] = '-';

    s[i] = '\0';
    sio_reverse(s);
}

ssize_t sio_puts(char s[])//Put string
{
    return write(STDOUT_FILENO,s,sio_strlen(s));
}

ssize_t sio_putl(long v)//Put long
{
    char s[32];
    sio_ltoa(v,s,10);//Based on K&R itoa()
    return sio_puts(s);
}

void sio_error(char s[])//Put error message and exit
{
    sio_puts(s);
    _exit(1);
}