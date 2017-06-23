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
#include "tingATCmd.h"

Ting ting;
uint8_t sendBuf[10]={'1','2','3','4','5','6','7','8','9','0'};
uint8_t len;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    ting.begin(&PA1,&uart3,115200);
    uart1.printf("start\r\n");
    ting.set_addr(0xffff);
    ting.set_dest(0xffff);
}
uint16_t addr1;
uint16_t addr;

int main(void)
{
    setup();
    while(1)
    {
        //ting.test();
       /// delay_ms(100);
        //ting.test();
        //ting.sleep();
        for(int i = 0; i < 2; i++)
        {
            ting.pwm1(1,2,i);
            ting.pwm2(1,2,2 - i);
            
        delay_ms(100);

        }
                // ting.set_pb0();

        //ting.clear_pd0();
              //   ting.clear_pb0();


        //delay_ms(1000);

    }

}


