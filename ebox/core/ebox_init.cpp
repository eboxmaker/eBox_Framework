#include "ebox.h"
#include "mcu.h"
#include "ebox_core.h"
#include "ebox_mem.h"

void ebox_init(void)
{

    interrupts  = __enable_irq;
    no_interrupts = __disable_irq;
    delay_ms    = mcu_delay_ms;
    delay_us    = mcu_delay_us;
    micros      = mcu_micros;
    millis      = mcu_millis;

    mcu_init();


}
extern "C" {
    /**
     *@brief    定义一个ebox_printf的输出函数，其中的输出设备必须是可用的
     *@param    NONE
     *@retval   NONE
    */
    size_t ebox_printf(const char *fmt, ...)
    {
#if USE_PRINTF
        int     size1 = 0;
        size_t  size2 = 64;
        char    *p;

        va_list va_params;
        va_start(va_params, fmt);


        do
        {
            p = (char *)ebox_malloc(size2);
            if(p == NULL)
                return 0;

            size1 =  ebox_vsnprintf(p, size2, fmt, va_params);

            if(size1 == -1  || size1 >= size2)
            {
                size2 += 64;
                size1 = -1;
                ebox_free(p);
            }
        }
        while(size1 == -1);
        va_end(va_params);
        DBG_UART.write(p, size1);
        ebox_free(p);
        return size1;
#endif
    }


    int ebox_uart_putc(int ch)
    {
#if USE_PRINTF
        DBG_UART.write(ch);
        return ch;
#endif
    }

    size_t ebox_uart_write(const char *buffer, size_t size)
    {
#if USE_PRINTF
        DBG_UART.write(buffer, size);
        return size;
#endif
    }



}


