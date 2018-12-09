#include <stdio.h>
#include "ebox_core.h"


#pragma import(__use_no_semihosting)                             

struct __FILE 
{ 
    int handle; 
}; 


FILE __stdout;          

_sys_exit(int x) 
{ 
    x = x; 
}


_ttywrch(int ch) 
{ 
    ch = ch; 
} 

int fputc(int ch, FILE *f){   
    
    ebox_uart_putc(ch);
    return ch;
}

