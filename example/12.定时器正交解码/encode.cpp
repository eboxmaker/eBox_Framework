/**
  ******************************************************************************
  * @file    pwm.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "math.h"
#include "ebox_encoder.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示了定时器正交解码功能
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"timer decoding example"
#define EXAMPLE_DATE	"2018-08-11"


//Encoder encoder(TIM1,&PA8,&PA9);
//float x;
//uint16_t y;
//Pwm pwm1(TIM3CH1);
void setup()
{
    ebox_init();
    //    UART.begin(115200);
    //    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    //    encoder.begin(3);
    //    pwm1.begin(1000, 100);
    //    pwm1.set_oc_polarity(1);//set output polarity after compare
    //    UART.printf("max frq = %dKhz\r\n",pwm1.get_max_frq()/1000);
    //    UART.printf("max frq = %f\r\n",pwm1.get_accuracy());
}
int main(void)
{
    setup();
    //    uint16_t temp;
    //    float speed;
    while(1)
    {
        //        x = x + PI * 0.01;
        //        if(x >= PI)x = 0;
        //        y = 2500 - (sin(x) + 1) * 1000;
        //        pwm1.set_duty(y);
        //        speed = encoder.read_speed()/50;
        //        UART.printf("count :%0.2f(%d)\r\n",speed,encoder.read_direction());
        //        delay_ms(1000);

    }
}




