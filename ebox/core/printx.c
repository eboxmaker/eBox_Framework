
#include "../ebox/core/mem/ebox_mem.h"
#include "printx.h"

//extern struct my_fifo *x_ptr;
struct my_fifo *uart_fifo_ptr;


void ebox_printf(const char *fmt, ...)
{
    char *buf;
    int     size1 = 0;
    size_t  size2 = 256;
    va_list va_params;

    va_start(va_params, fmt);
    
    do{
        buf = (char *)ebox_malloc(size2);
        if(buf == NULL)
            return ;
        size1 = vsnprintf(buf, size2,fmt, va_params);
        if(size1 == -1  || size1 > size2)
        {
            size2+=128;
            size1 = -1;
            ebox_free(buf);
        }
    }while(size1 == -1);

    size1 = vsprintf(buf, fmt, va_params); 
    va_end(va_params);

    my_fifo_put(uart_fifo_ptr,(unsigned char *)buf, size1);
    ebox_free(buf);

}



































//#pragma import(__use_no_semihosting)
//_sys_exit(int x)
//{
//x = x;
//}
//struct __FILE
//{
//int handle;
// 
// 
// 
//};
// 
//FILE __stdout;
//int fputc(int ch, FILE *f)
//{
//    uint8_t *p;
//    *p = ch;
//    my_fifo_put(x_ptr,p,1);
//    return ch;
//}

//_ttywrch(int ch)
//{
//ch = ch;
//}
