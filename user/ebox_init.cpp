#include "ebox.h"
#include "mcu.h"


void ebox_init(void)
{
    _ebox_init   = mcu_init;
    delay_ms    = mcu_delay_ms;
    delay_us    = mcu_delay_us;
    micros      = mcu_micros;
    millis      = mcu_millis;
    
    _ebox_init();
    ebox_printf_init();
    
    cpu = mcu;
}

void ebox_printf_flush(void)
{
    uint16_t len;
    uint8_t ch;
    while(1)
    {
        len = ebox_fifo_get(uart_fifo_ptr,&ch,1);
        if(len == 1)
        {
            uart1.write(ch);
        }
        else
        {
            break;
        }
    }
}
