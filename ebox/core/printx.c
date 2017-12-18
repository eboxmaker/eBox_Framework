
#include "stdio.h"
#include "fifo.h"

extern struct my_fifo *x_ptr;


#pragma import(__use_no_semihosting)
_sys_exit(int x)
{
x = x;
}
struct __FILE
{
int handle;
 
 
 
};
 
FILE __stdout;
int fputc(int ch, FILE *f)
{
    uint8_t *p;
    *p = ch;
    my_fifo_put(x_ptr,p,1);
    return ch;
}

_ttywrch(int ch)
{
ch = ch;
}