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
#include "encoder.h"
#include "ltc1446.h"

#define PinA	PA0
#define PinB	PA1
#define ADCA	PC0
#define ADCB	PC1

Encoder encoder1(&PinA, &PinB);
//LTCX ltc1(&PB1,&spi1);
LTCX ltc1(&PA6, &spi1);


void setup()
{
    ebox_init();
    ltc1.begin(1);
    uart1.begin(115200);
}

int main(void)
{
    uint32_t i = 2048, t = 0;
    setup();
    uart1.printf("init ok \r\n");
    uart1.printf("测试成功 \r\n");
    uart1.printf("%d \r\n", i);
    while(1)
    {
        switch(encoder1.read_encoder())
        {
        case 1:
            i++;
            uart1.printf("%d \r\n", i);
            break;
        case 2:
            i--;
            uart1.printf("%d \r\n", i);
            break;
        }
        if(i != t)
        {
            t = i;
            ltc1.write(4096 - i, i);
            uart1.printf("通道A电压 = %d; 通道B电压 = %d \r\n", analog_read_voltage(&ADCA), analog_read_voltage(&ADCB));
        }
    }
}
