/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V1.0
  * @date   : 2019/06/10
  * @brief   ebox application example .
	*					 2018-7-31	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */

#include "ebox.h"
#include "bsp_ebox.h"
#include "parallel_gpio.h"

/**
	*	1	此例程演示了并行操作多个IO
  * 2 其中pin_io将不连续io绑定为一组，进行操作，效率低
  * 3 PortPin 将同一个port的连续io绑定为一组，进行操作，效率高
	*/


// 从左到右对应bit0-bitx，不连续IO或非同一port推荐使用
Gpio *pins[] = {&PA0,&PA1,&PA2,&PB1};
Gpios pin_io(pins,4);

// 操作PA2,PA3,PA4  同一port连续IO推荐使用方式
//Port PortPin(GPIOA_BASE,3,2);
 Port PortPin(GPIOA_BASE,3);		// PA0,PA1,PA2


// 使用父类指针操作不同子类
ParallelGpio * port;

void operationPortPin(void){
    uint16_t r;
    port = &PortPin;
		port->mode(OUTPUT_PP);
		port->write(0x00);	// PA2,PA3,PA4 = 0
		port->write(0xff);	// PA2,PA3,PA4 = 1
		port->write(0x03);	// PA2,PA3=1; PA4 = 0;
    r = port->read();		// r = 0x03
		r = r<<1;
		port->write(r);			// PA2 = 0, PA3,PA4 =1
}

void operationPin_IO(){
    uint16_t r = 0;
		port = &pin_io;
		port->mode(OUTPUT_PP);
		port->write(0x00);	// PA0,PA1,PA2,PB1 = 0
		port->write(0xff);	// PA0,PA1,PA2,PB1 = 1
		port->write(0x03);	// PA0,PA1=1; PA2,PB1 = 0;
    r = port->read();		// r = 0x03
		r = r<<1;
		port->write(r);			// PA0=0,PA1,PA2=1; PB1 = 0;
}

void setup()
{
    ebox_init();
}
int main(void)
{
    setup();

    while(1)
    {
				operationPin_IO();
				operationPortPin();
    }
}


