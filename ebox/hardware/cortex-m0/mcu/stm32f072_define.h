/******************************************************************************
程 序 名： stm32f072_define.h　
编 写 人： cat_li
编写时间： 2017年10月16日
接口说明： stm32f072xb外设定义.
	1个spi,i2c,usart,adc(11ch),15gpios,5tim(4general(3,14,16,17),1advance(1)4kRAM,16kFLASH
修改日志：　　
	NO.1-  初始版本
				2017/10/16 usart info 改用Periph_SS定义，信息中包含不同usart的时钟使能函数
******************************************************************************/

#ifndef __STM32F072_DEFINE_H_
#define __STM32F072_DEFINE_H_

#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_tim.h"
#include "ebox_port_gpio.h"
//#include "stm32_define.h"

//#define UART1_TX	PA_9
//#define	UART1_RX	PA_10

//#define UART2_TX	PA_2
//#define UART2_RX	PA_3

//#define I2C1_SCL	PB_8
//#define	I2C1_SDA	PB_9

//#define SPI_SCK		PA_5
//#define SPI_MISO	PA_6
//#define SPI_MOSI	PA_7

//#define TIM3CH1		TIM3,PA_6
//#define	TIM3CH2		TIM3,PA_7
//#define TIM3CH4		TIM3,PB_1
////#define	TIM14CH1	TIM14,PA_4
////#define	TIM16CH1	TIM16,PA_6
//#define	TIM17CH1	TIM17,PA_7
//#define TIM1CH1		TIM1,PA_8
//#define TIM1CH2		TIM1,PA_9
//#define TIM1CH3		TIM1,PA_10

//#define STM32_SRAM_SIZE					16		//内存大小，ebox_mem调用

//#include "stm32f0xx_ll_adc.h"

//static const AF_FUN_S ADC_MAP[] = {
//	PA_0,AIN,0,LL_ADC_CHANNEL_0,
//	PA_1,AIN,0,LL_ADC_CHANNEL_1,
//	PA_2,AIN,0,LL_ADC_CHANNEL_2,
//	PA_3,AIN,0,LL_ADC_CHANNEL_3,
//	PA_4,AIN,0,LL_ADC_CHANNEL_4,
//	PA_5,AIN,0,LL_ADC_CHANNEL_5,
//	PA_6,AIN,0,LL_ADC_CHANNEL_6,
//	PA_7,AIN,0,LL_ADC_CHANNEL_7,

//	PB_1,AIN,0,LL_ADC_CHANNEL_9,

//	P_NC
//};



#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

//static const AF_FUN_S TIM_MAP[] ={
//	// TIM3,CH1,CH2,CH4
//	PA_6,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH1,
//	PA_7,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH2,
//	PB_1,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH4,
//	// TIM16,CH1
//	PA_6,AF_PP,LL_GPIO_AF_5,TIM16_BASE+TIMxCH1,
//	// TIM17,CH1
//	PA_7,AF_PP,LL_GPIO_AF_5,TIM17_BASE+TIMxCH1,
//	// TIM14,CH1
//	PA_4,AF_PP,LL_GPIO_AF_4,TIM14_BASE+TIMxCH1,
//	// TIM1,CH2,CH3
//	PA_8,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH1,
//	PA_9,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH2,
//	PA_10,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH3,
//	P_NC
//};

//// 不同tim irq索引
//#define TIM1IQR		Irq1
//#define TIM3IQR		Irq2
//#define TIM14IQR	Irq3
//#define TIM16IQR	Irq4
//#define TIM17IQR	Irq5

//static const Periph_SS TIM_INFO[]={
//	TIM1_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM1,TIM1_BRK_UP_TRG_COM_IRQn,TIM1IQR,
////	TIM1_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM1,TIM1_CC_IRQn,TIM1IQR,
//	TIM3_BASE,LL_APB1_GRP1_EnableClock,LL_APB1_GRP1_PERIPH_TIM3,TIM3_IRQn,TIM3IQR,
//	TIM14_BASE,LL_APB1_GRP1_EnableClock,LL_APB1_GRP1_PERIPH_TIM14,TIM14_IRQn,TIM14IQR,
//	TIM16_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM16,TIM16_IRQn,TIM16IQR,
//	TIM17_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_TIM17,TIM17_IRQn,TIM17IQR,
//	NC
//};

//static const AF_FUN_S SPI_MAP[] = {
//	PA_5,AF_PP_PU,0,SPI1_BASE,
//	PA_6,AF_PP_PU,0,SPI1_BASE,
//	PA_7,AF_PP_PU,0,SPI1_BASE,
//	P_NC
//};
		
//static const Periph_S SPI_INFO[] = {
//	SPI1_BASE,LL_APB1_GRP2_PERIPH_SPI1,USART1_IRQn,Irq1,
//	NC
//};

// PA2,9,14 tx; PA3,10 rx
//static const AF_FUN_S UART_MAP[] ={
//	PA_2,AF_PP_PU,LL_GPIO_AF_1,USART2_BASE,
//	PA_3,AF_PP_PU,LL_GPIO_AF_1,USART2_BASE,
//	PA_9,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
//	PA_10,AF_PP_PU,LL_GPIO_AF_1,USART1_BASE,
//	P_NC
//};

//static const Periph_S UART_INFO[] = {
//	USART1_BASE,LL_APB1_GRP2_PERIPH_USART1,USART1_IRQn,Irq1,
//	USART2_BASE,LL_APB1_GRP1_PERIPH_USART2,USART2_IRQn,Irq2,
//	NC
//};

//static const Periph_SS UART_INFO[]={
//	USART1_BASE,LL_APB1_GRP2_EnableClock,LL_APB1_GRP2_PERIPH_USART1,USART1_IRQn,Irq1,
//	USART2_BASE,LL_APB1_GRP1_EnableClock,LL_APB1_GRP1_PERIPH_USART2,USART2_IRQn,Irq2,
//	NC
//};
typedef struct{
	PIN_ID_t	_pinId;		//pin_id
	PIN_MODE	_pinMode;	    //pin 参数， mode，outputtyper,updown
	uint8_t		_pinAf;		//af功能
	uint32_t	_periph;	//外设名或通道号
}AF_FUN_S;

// PB9 scl,PB9 SDA	
static const AF_FUN_S I2C_MAP[]={
	// i2c1
	PB8_ID,AF_PP_PU,LL_GPIO_AF_1,I2C1_BASE,
	PB9_ID,AF_PP_PU,LL_GPIO_AF_1,I2C1_BASE,
	(PIN_ID_t)0xff
};

static const AF_FUN_S TIM_MAP[] ={
	// TIM3,CH1,CH2,CH4
	PA6_ID,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH1,
	PA7_ID,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH2,
	PB1_ID,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH4,
	// TIM16,CH1
	PA6_ID,AF_PP,LL_GPIO_AF_5,TIM16_BASE+TIMxCH1,
	// TIM17,CH1
	PA7_ID,AF_PP,LL_GPIO_AF_5,TIM17_BASE+TIMxCH1,
	// TIM14,CH1
	PA4_ID,AF_PP,LL_GPIO_AF_4,TIM14_BASE+TIMxCH1,
	// TIM1,CH2,CH3
	PA8_ID,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH1,
	PA9_ID,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH2,
	PA10_ID,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH3,
	(PIN_ID_t)0xff
};

/**
 *@brief    根据Pin_id获取对应外设索引
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex(PIN_ID_t pin_id,uint32_t periph,const AF_FUN_S *emap)
{
	uint8_t i = 0;
	while (!((0xffffff00 & (emap+i)->_periph) == periph) || !((emap+i)->_pinId == pin_id))
	{
		if ((emap+i)->_pinId == 0xff){
			return (uint8_t)0xff;
		}
		i++;
	}
	return i;
}

/**
 *@brief    根据Pin_id获取对应外设索引
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex1(PIN_ID_t pin_id,const AF_FUN_S *emap)
{
	uint8_t i = 0;
	while (!((emap+i)->_pinId  == pin_id ))
	{
		if ((emap+i)->_pinId == 0xff){
			return (uint8_t)0xff;
		}
		i++;
	}
	return i;
}

#endif
