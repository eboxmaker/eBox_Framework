/**
  ******************************************************************************
  * @file    *.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright 2015 shentq. All Rights Reserved.
  *
  * Copyright Notice
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq.
  *
  * @Disclaimer
  * This specification is preliminary and is subject to change at any time without
  * notice. shentq assumes no responsibility for any errors contained herein.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示了定时器正交解码功能
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"timer decoding example"
#define EXAMPLE_DATE	"2018-08-11"


Encoder encoder(TIM1, &PA8, &PA9);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    encoder.begin(3);
}

int main(void)
{
    setup();
    uint16_t temp;
    float speed;
    while(1)
    {
        speed = encoder.read_speed() / 50;
        UART.printf("count :%0.2f(%d)\r\n", speed, encoder.read_direction());
        delay_ms(1000);
    }
}








