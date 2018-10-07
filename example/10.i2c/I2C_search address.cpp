/**
  ******************************************************************************
  * @file    example.cpp
  * @author  cat_li
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
#include "bsp_ebox.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"I2C serach address example"
#define EXAMPLE_DATE	"2018-08-08"

//mcuI2c i2c1(I2C1, &PB8, &PB9);

void setup()
{
	ebox_init();
	UART.begin(115200);
	print_log(EXAMPLE_NAME,EXAMPLE_DATE);
	I2C.begin(40000);
}

int main(void)
{
	uint8_t add = 0x45;
    uint16_t i = 0;
	setup();
	
	
	I2C.write(0x4e,0x00);
	delay_ms(1000);
	UART.printf("I2C address scanner, Scaning.......\r\n");
	while (1)
	{	
        I2C.write(0x4e,0x00);
        delay_ms(2000);
        UART.printf("I2C read %d \r\n",I2C.read(0x4e));
        I2C.write(0x4e,0x08);
        delay_ms(2000);
        UART.printf("I2C read %d \r\n",I2C.read(0x4e));

        
		// 7位地址,0-127
		for (;add<=0x50;add++)
		{
			UART.printf("test address is 0x%2x  ",add);
			if (I2C.waitAck(add)==0)
			{
				UART.printf("success! \r\n");
				I2C.write(add,0x08);
				delay_ms(600);
				//break;				
			} else{
				UART.printf("fail! \r\n");
			}
		}
		if (add>127)
		{
			UART.printf("请检查接线,并确认设备ok! \r\n");
		}
		delay_ms(6000);
		I2C.write(0x4e,0x00);
		add = 0x45;
	}

}




