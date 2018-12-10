/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */



#include "ebox.h"
#include "ddc.h"


void ddc_evnet_ch20(uint8_t *ptr, uint16_t len)
{
    PB8.toggle();
}
void ddc_input()
{
    ddc_get_char(uart1.read());
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.attach(ddc_input, RxIrq);
    uart1.interrupt(RxIrq, ENABLE);

    ddc_init();
    ddc_attach_chx(20, ddc_evnet_ch20);

    PB8.mode(OUTPUT);

}
uint8_t buf[8] = {'1', '1', '1', '1', '1', '1', '1', '1',};
DataFloat_t d1, d2;
int main(void)
{
    setup();

    while(1)
    {
        ddc_nonblocking(buf, 8, DDC_NoAck, 4);
        delay_ms(100);
        ddc_loop();

    }
}




