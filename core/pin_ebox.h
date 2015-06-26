#ifndef __PIN_EBOX_H
#define __PIN_EBOX_H

#include "ebox.h"

/* Types used for the tables below */
typedef struct 
{
  GPIO_TypeDef* port ;
  uint32_t pin ;
  uint32_t rcc ;
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
	uint32_t extiLine;
	uint8_t portSource;
	uint8_t pinSource;
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



extern const PIN_INFO pinInfo[];
extern const TIMx_INFO TIMxInfo[];
extern const USARTx_INFO USARTxInfo[];

extern const PIN_TO_ANALOAG pinToAnalog[];
extern const PIN_TO_EXTI pinToExti[];

extern uint16_t analogPinStatu[];//是否配置为AIN模式
extern uint8_t  pwmPinStatu[];//







#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04
//////////////////////////////////////
uint32_t TIMxToRCC(TIM_TypeDef* TIMx);
uint32_t TIMxToIRQ(TIM_TypeDef* TIMx);
uint8_t TIMxToID(TIM_TypeDef* TIMx);
/////////////////////////////////////////
TIM_TypeDef* 	pinToTIMx(uint8_t pin);
uint8_t 			pinToTIM_ch(uint8_t pin);
uint32_t 			pinToTIM_rcc(uint8_t pin);
uint32_t 			pinToTIM_irq(uint8_t pin);
uint8_t 			isPwmPin(uint8_t pin);
//////////////////////////
uint32_t  USARTxToRCC(USART_TypeDef* USARTx);
uint32_t 	USARTxToIRQ(USART_TypeDef* USARTx);
uint8_t 	USARTxToID(USART_TypeDef* USARTx);
//////////////////////////////////////////////////

uint8_t isAnalogPin(uint8_t pin);
uint8_t isPinNeedRemap(uint8_t pin);
void remapCtr(void);
#ifdef STM32F103C8T6
void remap15_19_26_27ToTIM2(void);
void remap15ToTIM2(void);
void remap19ToTIM2(void);
void remap16_17_20_21ToTIM3(void);
#endif
#endif

