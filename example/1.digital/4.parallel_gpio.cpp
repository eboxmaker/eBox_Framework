/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V1.0
  * @date   : 2019/06/10
  * @brief   ebox application example .
	*					 2018-7-31	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */

#include "ebox.h"
#include "bsp_ebox.h"
#include "parallel_gpio.h"

/**
	*	1	��������ʾ�˲��в������IO
  * 2 ����pin_io��������io��Ϊһ�飬���в�����Ч�ʵ�
  * 3 PortPin ��ͬһ��port������io��Ϊһ�飬���в�����Ч�ʸ�
	*/


// �����Ҷ�Ӧbit0-bitx��������IO���ͬһport�Ƽ�ʹ��
Gpio *pins[] = {&PA0,&PA1,&PA2,&PB1};
Gpios pin_io(pins,4);

// ����PA2,PA3,PA4  ͬһport����IO�Ƽ�ʹ�÷�ʽ
//Port PortPin(GPIOA_BASE,3,2);
 Port PortPin(GPIOA_BASE,3);		// PA0,PA1,PA2


// ʹ�ø���ָ�������ͬ����
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


