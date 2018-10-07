
#include "ebox_core.h"
#include "ebox_mem.h"

#include "ebox_printf.h"

#if USE_PRINTF


//void ebox_printf(const char *fmt, ...)
//{
//    int     len = 0;
//    size_t  size2 = 64;
//    char *buf;

//    va_list va_params;
//    va_start(va_params, fmt);
//    
//    do{
//        buf = (char *)ebox_malloc(size2);
//        if(buf == NULL)
//            return ;

//        len =  ebox_vsnprintf(buf, size2,fmt, va_params);

//        if(len == -1  || len >= size2)
//        {
//            size2+=64;
//            len = -1;
//            ebox_free(buf);
//        }
//    }while(len == -1);
//    va_end(va_params);

//    
////    while(1)
////    {
//        size2 = ebox_fifo_put(uart_fifo_ptr,(unsigned char *)buf,len);
////        if(size2 < len)
////        {
////            len -= size2; 
////            buf+=size2;
////            ebox_printf_flush();
////        }
////        else
////        {
////            ebox_printf_flush();
////            break;
////        }

////    }
//    ebox_free(buf);
//    
//}
#endif

