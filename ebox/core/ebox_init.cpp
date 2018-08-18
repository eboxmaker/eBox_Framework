#include "ebox.h"
#include "mcu.h"
#include "ebox_printf.h"
#include "ebox_core.h"
#include "ebox_mem.h"

void ebox_init(void)
{
    
    ebox_heap_init((void*)MCU_HEAP_BEGIN, (void*)MCU_HEAP_END);
    interrupts  = __enable_irq;
    no_interrupts = __disable_irq;
    delay_ms    = mcu_delay_ms;
    delay_us    = mcu_delay_us;
    micros      = mcu_micros;
    millis      = mcu_millis;

    mcu_init();
    
    #if USE_PRINTF 
    ebox_printf_init();
    #endif
    
    
}
#if USE_PRINTF
extern "C"{
    /**
     *@brief    定义一个ebox_printf的输出函数，其中的输出设备必须是可用的
     *@param    NONE
     *@retval   NONE
    */
    void ebox_printf_flush(void)
    {
        uint16_t len;
        uint8_t buffer[64];
        while(1)
        {
            len = ebox_fifo_get(uart_fifo_ptr,buffer,64);
            if(len >= 1)
            {
                uart1.write(buffer,len);
            }
            else
            {
                break;
            }
        }
    }
}
#endif
