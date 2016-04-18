/*
file   : *.cpp
author : shentq
version: V1.1
date   : 2016/03/26

Copyright 2016 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ultrasonic_wave.h"
float value;
float value_2;

ULTRA ultra(&PA0,&PA1);
ULTRA ultra_2(&PA2,&PA3);

void ultra_event()
{
	ultra.mesure_event();
}
void ultra_event_2()
{
	ultra_2.mesure_event();
}

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("ok \r\n");
    ultra.begin();
		ultra.attch_mesuer_event(ultra_event);
	
    ultra_2.begin();
		ultra_2.attch_mesuer_event(ultra_event_2);
}

int main(void)
{
    setup();
    while(1)
    {       
        ultra.start();
        ultra_2.start();
				if(ultra.avaliable())
				{
					value = ultra.read_cm();
					uart1.printf("value 1 = %0.2fcm\r\n",value);
				}
				
        ultra_2.start();
				if(ultra_2.avaliable())
				{
					value_2 = ultra_2.read_cm();
					uart1.printf("value 2 = %0.2fcm\r\n",value_2);
				}
				
				
		delay_ms(1000);
    }

}


