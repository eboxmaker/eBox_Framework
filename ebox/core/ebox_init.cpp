#include "ebox.h"
#include "mcu.h"
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
    
    
}
#if USE_PRINTF
extern "C"{
    /**
     *@brief    ����һ��ebox_printf��������������е�����豸�����ǿ��õ�
     *@param    NONE
     *@retval   NONE
    */
    size_t ebox_printf(const char *fmt, ...)
    {
        int     size1 = 0;
        size_t  size2 = 64;
        char    *p;

        va_list va_params;
        va_start(va_params, fmt);
        
        
        do{
            p = (char *)ebox_malloc(size2);
            if(p == NULL)
                return 0;

            size1 =  ebox_vsnprintf(p, size2,fmt, va_params);

            if(size1 == -1  || size1 >= size2)
            {
                size2+=64;
                size1 = -1;
                ebox_free(p);
            }
        }while(size1 == -1);
        va_end(va_params);
        uart1.write(p, size1);
        ebox_free(p);
        return size1;
    }
    

}
#endif
