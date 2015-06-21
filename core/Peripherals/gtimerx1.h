#ifndef __GTIMERX_H
#define __GTIMERX_H
#ifdef __cplusplus
extern "C"{
#endif
	
#include "arduino.h"
	
typedef struct 
{
	TIM_TypeDef *timx;
	uint32_t rcc;
	uint32_t irq;
} TIMxInfo ;

typedef struct
{
	TIMxInfo timinfo;
	void (*Frequence)(uint16_t period,uint16_t prescaler);
	void (*Interrupt)(FunctionalState x);
	void (*TIMxInit)(TIM_TypeDef* TIMx);
}GTIMx;

void TIMx_DeInit(TIM_TypeDef* TIMx);
void Frequence(uint16_t period,uint16_t prescaler);
void Interrupt(FunctionalState x);

#ifdef __cplusplus
}
#endif

#endif