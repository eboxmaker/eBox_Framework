
#include "ebox_printf.h"

extern void ebox_printf_flush(void);

struct ebox_fifo *uart_fifo_ptr;
void ebox_printf_init(void)
{
    uart_fifo_ptr = ebox_fifo_alloc(64);
}

void ebox_printf(const char *fmt, ...)
{
    char *buf;
    size_t  size1 = 0;
    size_t  size2 = 32;
    va_list va_params;

    va_start(va_params, fmt);
    do{
        
        buf = (char *)ebox_malloc(size2);
        if(buf == NULL)
            return ;
        size1 = vsnprintf(buf, size2,fmt, va_params);
        if(size1 == -1  || size1 >= size2)
        {
            size2+=32;
            size1 = -1;
            ebox_free(buf);
        }
    }while(size1 == -1);
    va_end(va_params);
    while(1)
    {
        size2 = ebox_fifo_put(uart_fifo_ptr,(unsigned char *)buf,size1);
        if(size2 < size1)
        {
            size1 -= size2; 
            buf+=size2;
            ebox_printf_flush();
        }
        else
        {
            ebox_printf_flush();
            break;
        }

    }
    
    ebox_free(buf);
    
}

