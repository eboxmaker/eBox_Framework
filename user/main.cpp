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
uint8_t RecvBuf[10];
uint8_t len;
void setup()
{
    ebox_init();
    PA2.mode(INPUT_PD);
    PA3.mode(INPUT_PD);
    PB2.mode(INPUT);
    PB3.mode(INPUT);
    PB4.mode(INPUT);
    PB5.mode(INPUT);
    PB6.mode(INPUT);
    PA4.mode(INPUT);
    PA5.mode(INPUT);
    PA6.mode(INPUT);
    PA7.mode(INPUT);
    uart1.begin(115200);
    ting.begin(&PA1,&PA8,&uart3,115200);
    uart1.printf("start\r\n");
    ting.set_addr(0xffff);
    ting.set_dest(0xffff);
    delay_ms(1000);
    ting.rx();
}
uint16_t addr1;
uint16_t addr;

int main(void)
{
    setup();
    while(1)
    {
        len = ting.read(RecvBuf);
        if(len > 0)
        uart1.write(RecvBuf,len);
        delay_ms(3000);
       // ting.send(sendBuf,10);
        //delay_ms(2000);
        //len = ting.read(sendBuf);
        //uart1.write(sendBuf,len);
        //if(ting.test() == ERR_OK)
        {
         //   uart1.printf("xx\r\n");
        }
        
       /// delay_ms(100);
        //ting.test();
//        ting.sleep();
//         delay_ms(1000);
//        ting.wakeup();
//        ting.pwm1(1,200,100);
//         delay_ms(1000);
        /*
        for(int i = 0; i < 2; i++)
        {
            ting.pwm1(1,2,i);
            ting.pwm2(1,2,2 - i);
            
        delay_ms(100);

        }*/
                // ting.set_pb0();

        //ting.clear_pd0();
              //   ting.clear_pb0();


        //delay_ms(1000);

    }

}


