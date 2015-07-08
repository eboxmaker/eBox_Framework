#include "io.h"


GPIO::GPIO(GPIO_TypeDef* _port,uint16_t _pin)
{
	port = _port;
	pin = _pin;
	switch((uint32_t)port)
	{
		case (uint32_t)GPIOA:
			rcc = RCC_APB2Periph_GPIOA;
			break;

		case (uint32_t)GPIOB:
			rcc = RCC_APB2Periph_GPIOB;
			break;

		case (uint32_t)GPIOC:
			rcc = RCC_APB2Periph_GPIOC;
			break;

		case (uint32_t)GPIOD:
			rcc = RCC_APB2Periph_GPIOD;
			break;

		case (uint32_t)GPIOE:
			rcc = RCC_APB2Periph_GPIOE;
			break;
		
		case (uint32_t)GPIOF:
			rcc = RCC_APB2Periph_GPIOF;
			break;
	}
}

void GPIO::mode(PINMODE modeVal)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(rcc,ENABLE);
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)modeVal;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(port, &GPIO_InitStructure);   //³õÊ¼»¯GPIOC¶Ë¿Ú
}

void GPIO::write(uint8_t val)
{
	if(val == 0)
		port->BRR = pin;
	else
		port->BSRR = pin;
}

void GPIO::read(uint8_t* val)
{
	*val = port->IDR & pin;
}		

uint8_t GPIO::read(void)
{
	return  (port->IDR & pin);
}
