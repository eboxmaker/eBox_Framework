#include "ebox.h"




void pinMode(uint32_t pin, uint32_t mode)
{
	
//	uint8_t bit = digitalPinToBitMask(pin);
//	uint8_t port = digitalPinToPort(pin);
	
//	GPIO_TypeDef *gpio_port = g_APinDescription[Pin].pPort;
//  uint16_t gpio_pin = g_APinDescription[Pin].Pin;
	GPIO_TypeDef * gpio_port = pinInfo[pin].port;
	uint16_t gpio_pin = pinInfo[pin].pin;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(pinInfo[pin].rcc,ENABLE);
  GPIO_InitStructure.GPIO_Pin = gpio_pin;

	switch(mode)
	{
		/////////////////////////////////////////////////////////
		case INPUT:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		break;
		case INPUT_PULLUP:
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		break;
		case INPUT_PULLDOWN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	//////////////////////////////////////////////////////////
		case OUTPUT:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		break;
		case AF_PP:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		break;
	  case AF_OD:
			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
			 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  break;
		/////////////////////////////////////////////////////////////
	  case AIN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    break;

		default:
		break ;
	}
		GPIO_Init(gpio_port, &GPIO_InitStructure);   //³õÊ¼»¯GPIOC¶Ë¿Ú

}
void digitalWrite( uint32_t pin, uint32_t ulVal )
{
  if ( ulVal == 0 )
  {
    pinInfo[pin].port->BRR = pinInfo[pin].pin;
  }
  else
  {
    pinInfo[pin].port->BSRR = pinInfo[pin].pin;
  }
}
 int digitalRead( uint32_t pin )
{
	if((pinInfo[pin].port->IDR & pinInfo[pin].pin) != 0)
		return 1;
	return 0;
}
