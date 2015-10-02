/*
file   : common.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __COMMON_H
#define __COMMON_H
#ifdef __cplusplus
extern "C"{
#endif
	
#include "stm32f10x.h"                  // Device header



////////�궨��/////////////////////////////////


#define true 0x1
#define false 0x0
 
#define HIGH 0x1
#define LOW  0x0


#define LSB_FIRST 0
#define MSB_FIRST 1

#define PI 3.1415926535898


typedef enum
{ 
	AIN = 0x0,
  INPUT = 0x04,
  INPUT_PD = 0x28,
  INPUT_PU = 0x48,
  OUTPUT_OD = 0x14,
  OUTPUT_PP = 0x10,
  AF_OD = 0x1C,
  AF_PP = 0x18
}PIN_MODE;

//�Ժ�NVIC_PriorityGroupConfig()��������Ҫ�ٱ����á����������Է�NVIC_GROUP_CONFIGֵ������
#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2   /*!< 2 bits for pre-emption priority
																									 2 bits for subpriority */

#define interrupts() 		__enable_irq()
#define no_interrupts() 	__disable_irq()
	
///////ȫ�ֱ���������///////////////////////////////////////////////
extern __IO uint32_t millis_seconds;

typedef void (*callback_fun_type)(void);	 
	 
void ebox_init(void);	
void 	ADC1_init(void);
uint32_t millis( void ) ;
void delay_ms(uint32_t ms);
void delay_us(uint16_t us);
void delayus(uint32_t us);

class GPIO
{
	public:
		GPIO(GPIO_TypeDef* _port,uint16_t _pin);
		void mode(PIN_MODE mode_val);
		void set();
		void reset();
		void write(uint8_t val);
		void read(uint8_t* val);	
		uint8_t read(void);
	
		GPIO_TypeDef* port;
		uint16_t pin;
	
};
//��io�ٶ�Ҫ��ϸߵĵط�������ʹ��
#define pin_mode(pin,val)			pin.mode(val)
#define digital_write(pin,val)	pin.write(val)
#define digital_read(pin) 			pin.read()

uint16_t	analog_read(GPIO* pin);	
uint16_t	analog_read_voltage(GPIO* pin); 

void			shift_out(GPIO* data_pin, GPIO* clock_pin, uint8_t bit_order, uint8_t val);
uint8_t		shift_in(GPIO* data_pin, GPIO* clock_pin, uint8_t bit_order);


#ifdef __cplusplus
}
#endif

#endif
