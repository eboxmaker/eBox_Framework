#ifndef __PIN_EBOX_H
#define __PIN_EBOX_H

#include "ebox.h"

/* Types used for the tables below */
typedef struct _PinInfo
{
  GPIO_TypeDef* Port ;
  uint32_t Pin ;
  uint32_t Rcc ;
} PIN_INFO ;

typedef struct 
{
	uint8_t id ;
	USART_TypeDef *usart;
	uint32_t rcc;
	uint32_t irq;
} USARTx_INFO ;

typedef struct
{
	uint8_t pin;
	u16 analogch;
}PIN_TO_ANALOAG;

typedef struct
{
	uint32_t ExtiLine;
	uint8_t PortSource;
	uint8_t PinSource;
	uint32_t irqch;
	uint8_t  pin;

}PIN_TO_EXTI;

typedef struct 
{
	TIM_TypeDef *TIMx;
	uint32_t rcc;
	uint32_t irq;
	uint8_t ch;
	uint8_t needremap;
	uint8_t pin;

}PIN_TO_TIMx;

typedef struct 
{
	uint8_t id ;
	TIM_TypeDef *timx;
	uint32_t rcc;
	uint32_t irq;
} TIMx_INFO ;



extern const PIN_INFO _PinInfo[];
extern const TIMx_INFO _TIMxInfo[];
extern const USARTx_INFO _USARTxInfo[];

extern const PIN_TO_ANALOAG _PinToAnalog[];
extern const PIN_TO_EXTI PinToExti[];

extern uint16_t AnalogPinStatu[];//是否配置为AIN模式
extern uint8_t  PwmPinStatu[];//







#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04
//////////////////////////////////////
uint32_t TIMxToRCC(TIM_TypeDef* TIMx);
uint32_t TIMxToIRQ(TIM_TypeDef* TIMx);
uint8_t TIMxToID(TIM_TypeDef* TIMx);
/////////////////////////////////////////
TIM_TypeDef* PinToTIMx(uint8_t pin);
uint8_t PinToTIM_ch(uint8_t pin);
uint32_t PinToTIM_rcc(uint8_t pin);
uint32_t PinToTIM_irq(uint8_t pin);
uint8_t IsPwmPin(uint8_t pin);
//////////////////////////
uint32_t  USARTxToRCC(USART_TypeDef* USARTx);
uint32_t 	USARTxToIRQ(USART_TypeDef* USARTx);
uint8_t 	USARTxToID(USART_TypeDef* USARTx);
//////////////////////////////////////////////////

uint8_t IsAnalogPin(uint8_t pin);
uint8_t IsPinNeedRemap(uint8_t pin);
void ReMapCtr(void);
#ifdef STM32F103C8T6
void ReMap15_19_26_27ToTIM2(void);
void ReMap15ToTIM2(void);
void ReMap19ToTIM2(void);
void ReMap16_17_20_21ToTIM3(void);
#endif
#endif

