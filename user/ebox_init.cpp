#include "ebox.h"
#include "mcu.h"


void ebox_init()
{
    _ebox_init   = mcu_init;
    delay_ms    = mcu_delay_ms;
    delay_us    = mcu_delay_us;
    micros      = mcu_micros;
    millis      = mcu_millis;
    
    
    _ebox_init();
    
    cpu = mcu;
}
