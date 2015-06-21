
#include "pin_ebox.h"
//打印以下几个数组可知IO是否冲突
//////////////////////////////////////////////
uint16_t IOPinStatu[TOTAL_PIN];//IO口是否使用
uint16_t AnalogPinStatu[TOTAL_PIN];//是否配置为AIN模式
uint8_t PwmPinStatu[TOTAL_PIN];//是否配置为PWM模式
//////////////////////////////////////////////////

const PIN_INFO _PinInfo[]=
{
	//0-15
  { GPIOA, GPIO_Pin_0,  RCC_APB2Periph_GPIOA}, //1
  { GPIOA, GPIO_Pin_1,  RCC_APB2Periph_GPIOA}, // 2
  { GPIOA, GPIO_Pin_2,  RCC_APB2Periph_GPIOA}, // 3
  { GPIOA, GPIO_Pin_3,  RCC_APB2Periph_GPIOA}, //4
  { GPIOA, GPIO_Pin_4,  RCC_APB2Periph_GPIOA}, // 5
  { GPIOA, GPIO_Pin_5,  RCC_APB2Periph_GPIOA}, //6
  { GPIOA, GPIO_Pin_6,  RCC_APB2Periph_GPIOA}, // 7
  { GPIOA, GPIO_Pin_7,  RCC_APB2Periph_GPIOA}, //8
  { GPIOA, GPIO_Pin_8,  RCC_APB2Periph_GPIOA}, // 9
  { GPIOA, GPIO_Pin_9,  RCC_APB2Periph_GPIOA}, //10
  { GPIOA, GPIO_Pin_10,  RCC_APB2Periph_GPIOA}, // 11
  { GPIOA, GPIO_Pin_11,  RCC_APB2Periph_GPIOA}, //12
  { GPIOA, GPIO_Pin_12,  RCC_APB2Periph_GPIOA}, // 13
  { GPIOA, GPIO_Pin_13,  RCC_APB2Periph_GPIOA}, //14
  { GPIOA, GPIO_Pin_14,  RCC_APB2Periph_GPIOA}, // 15
  { GPIOA, GPIO_Pin_15,  RCC_APB2Periph_GPIOA}, //16

	
	
	//16-31
  { GPIOB, GPIO_Pin_0,  RCC_APB2Periph_GPIOB}, //16 
  { GPIOB, GPIO_Pin_1,  RCC_APB2Periph_GPIOB}, //17 
  { GPIOB, GPIO_Pin_2,  RCC_APB2Periph_GPIOB}, //18 
  { GPIOB, GPIO_Pin_3,  RCC_APB2Periph_GPIOB}, // 19
  { GPIOB, GPIO_Pin_4,  RCC_APB2Periph_GPIOB}, // 20
  { GPIOB, GPIO_Pin_5,  RCC_APB2Periph_GPIOB}, // 21
  { GPIOB, GPIO_Pin_6,  RCC_APB2Periph_GPIOB}, // 22
  { GPIOB, GPIO_Pin_7,  RCC_APB2Periph_GPIOB}, // 23
  { GPIOB, GPIO_Pin_8,  RCC_APB2Periph_GPIOB}, // 24
  { GPIOB, GPIO_Pin_9,  RCC_APB2Periph_GPIOB}, // 25
  { GPIOB, GPIO_Pin_10,  RCC_APB2Periph_GPIOB}, // 26
  { GPIOB, GPIO_Pin_11,  RCC_APB2Periph_GPIOB}, // 27
  { GPIOB, GPIO_Pin_12,  RCC_APB2Periph_GPIOB}, //28 
  { GPIOB, GPIO_Pin_13,  RCC_APB2Periph_GPIOB}, // 29
  { GPIOB, GPIO_Pin_14,  RCC_APB2Periph_GPIOB}, // 30
  { GPIOB, GPIO_Pin_15,  RCC_APB2Periph_GPIOB}, // 31
	
	//32-47
	{ GPIOC, GPIO_Pin_0,  RCC_APB2Periph_GPIOC}, // 32
  { GPIOC, GPIO_Pin_1,  RCC_APB2Periph_GPIOC}, //33 
  { GPIOC, GPIO_Pin_2,  RCC_APB2Periph_GPIOC}, //34 
  { GPIOC, GPIO_Pin_3,  RCC_APB2Periph_GPIOC}, //35 
  { GPIOC, GPIO_Pin_4,  RCC_APB2Periph_GPIOC}, //36 
  { GPIOC, GPIO_Pin_5,  RCC_APB2Periph_GPIOC}, //37 
  { GPIOC, GPIO_Pin_6,  RCC_APB2Periph_GPIOC}, //38 
  { GPIOC, GPIO_Pin_7,  RCC_APB2Periph_GPIOC}, //39 
  { GPIOC, GPIO_Pin_8,  RCC_APB2Periph_GPIOC}, //40 
  { GPIOC, GPIO_Pin_9,  RCC_APB2Periph_GPIOC}, //41 
  { GPIOC, GPIO_Pin_10,  RCC_APB2Periph_GPIOC}, //42 
  { GPIOC, GPIO_Pin_11,  RCC_APB2Periph_GPIOC}, //43 
  { GPIOC, GPIO_Pin_12,  RCC_APB2Periph_GPIOC}, // 44
  { GPIOC, GPIO_Pin_13,  RCC_APB2Periph_GPIOC}, //45 
  { GPIOC, GPIO_Pin_14,  RCC_APB2Periph_GPIOC}, //46 
  { GPIOC, GPIO_Pin_15,  RCC_APB2Periph_GPIOC}, // 47
	
	//48-63
	{ GPIOD, GPIO_Pin_0,  RCC_APB2Periph_GPIOD}, //48 
  { GPIOD, GPIO_Pin_1,  RCC_APB2Periph_GPIOD}, //49 
  { GPIOD, GPIO_Pin_2,  RCC_APB2Periph_GPIOD}, //50 
  { GPIOD, GPIO_Pin_3,  RCC_APB2Periph_GPIOD}, // 51
  { GPIOD, GPIO_Pin_4,  RCC_APB2Periph_GPIOD}, // 52
  { GPIOD, GPIO_Pin_5,  RCC_APB2Periph_GPIOD}, // 53
  { GPIOD, GPIO_Pin_6,  RCC_APB2Periph_GPIOD}, // 54
  { GPIOD, GPIO_Pin_7,  RCC_APB2Periph_GPIOD}, // 55
  { GPIOD, GPIO_Pin_8,  RCC_APB2Periph_GPIOD}, // 56
  { GPIOD, GPIO_Pin_9,  RCC_APB2Periph_GPIOD}, //57 
  { GPIOD, GPIO_Pin_10,  RCC_APB2Periph_GPIOD}, //58 
  { GPIOD, GPIO_Pin_11,  RCC_APB2Periph_GPIOD}, //59 
  { GPIOD, GPIO_Pin_12,  RCC_APB2Periph_GPIOD}, //60 
  { GPIOD, GPIO_Pin_13,  RCC_APB2Periph_GPIOD}, //61 
  { GPIOD, GPIO_Pin_14,  RCC_APB2Periph_GPIOD}, //62 
  { GPIOD, GPIO_Pin_15,  RCC_APB2Periph_GPIOD}, // 63	

	//64-79
	{ GPIOE, GPIO_Pin_0,  RCC_APB2Periph_GPIOE}, // 64
  { GPIOE, GPIO_Pin_1,  RCC_APB2Periph_GPIOE}, // 65
  { GPIOE, GPIO_Pin_2,  RCC_APB2Periph_GPIOE}, // 66
  { GPIOE, GPIO_Pin_3,  RCC_APB2Periph_GPIOE}, // 67
  { GPIOE, GPIO_Pin_4,  RCC_APB2Periph_GPIOE}, // 68
  { GPIOE, GPIO_Pin_5,  RCC_APB2Periph_GPIOE}, // 69
  { GPIOE, GPIO_Pin_6,  RCC_APB2Periph_GPIOE}, // 70
  { GPIOE, GPIO_Pin_7,  RCC_APB2Periph_GPIOE}, // 71
  { GPIOE, GPIO_Pin_8,  RCC_APB2Periph_GPIOE}, // 72
  { GPIOE, GPIO_Pin_9,  RCC_APB2Periph_GPIOE}, // 73
  { GPIOE, GPIO_Pin_10,  RCC_APB2Periph_GPIOE}, // 74
  { GPIOE, GPIO_Pin_11,  RCC_APB2Periph_GPIOE}, // 75
  { GPIOE, GPIO_Pin_12,  RCC_APB2Periph_GPIOE}, //76 
  { GPIOE, GPIO_Pin_13,  RCC_APB2Periph_GPIOE}, // 77
  { GPIOE, GPIO_Pin_14,  RCC_APB2Periph_GPIOE}, // 78
  { GPIOE, GPIO_Pin_15,  RCC_APB2Periph_GPIOE}, // 79	
	//80-95
	{ GPIOF, GPIO_Pin_0,  RCC_APB2Periph_GPIOF}, // 80
  { GPIOF, GPIO_Pin_1,  RCC_APB2Periph_GPIOF}, // 81
  { GPIOF, GPIO_Pin_2,  RCC_APB2Periph_GPIOF}, // 82
  { GPIOF, GPIO_Pin_3,  RCC_APB2Periph_GPIOF}, // 83
  { GPIOF, GPIO_Pin_4,  RCC_APB2Periph_GPIOF}, // 84
  { GPIOF, GPIO_Pin_5,  RCC_APB2Periph_GPIOF}, // 85
  { GPIOF, GPIO_Pin_6,  RCC_APB2Periph_GPIOF}, // 86
  { GPIOE, GPIO_Pin_7,  RCC_APB2Periph_GPIOF}, // 87
  { GPIOF, GPIO_Pin_8,  RCC_APB2Periph_GPIOF}, // 88
  { GPIOF, GPIO_Pin_9,  RCC_APB2Periph_GPIOF}, // 89
  { GPIOF, GPIO_Pin_10,  RCC_APB2Periph_GPIOF}, // 90
  { GPIOF, GPIO_Pin_11,  RCC_APB2Periph_GPIOF}, // 91
  { GPIOF, GPIO_Pin_12,  RCC_APB2Periph_GPIOF}, //92 
  { GPIOF, GPIO_Pin_13,  RCC_APB2Periph_GPIOF}, // 93
  { GPIOF, GPIO_Pin_14,  RCC_APB2Periph_GPIOF}, // 94
  { GPIOF, GPIO_Pin_15,  RCC_APB2Periph_GPIOF}, // 95
};

	
/////////////////////////////////////////////
const PIN_TO_ANALOAG _PinToAnalog[]=
{
	//{CH,APin}
	#ifdef STM32F103C8T6
	{0,0},
	{1,1},
	{2,2},
	{3,3},
	{4,4},
	{5,5},
	{6,6},
	{7,7},
	{8,16},
	{9,17}
	#endif
};
/////////////////////////////////////////////
const PIN_TO_EXTI PinToExti[]=
{
	//{extix,PinSourcex,irqch,APin}
	
	{EXTI_Line0,GPIO_PortSourceGPIOA,GPIO_PinSource0,EXTI0_IRQn,0},
	{EXTI_Line1,GPIO_PortSourceGPIOA,GPIO_PinSource1,EXTI1_IRQn,1},
	{EXTI_Line2,GPIO_PortSourceGPIOA,GPIO_PinSource2,EXTI2_IRQn,2},
	{EXTI_Line3,GPIO_PortSourceGPIOA,GPIO_PinSource3,EXTI3_IRQn,3},
	{EXTI_Line4,GPIO_PortSourceGPIOA,GPIO_PinSource4,EXTI4_IRQn,4},
	{EXTI_Line5,GPIO_PortSourceGPIOA,GPIO_PinSource5,EXTI9_5_IRQn,5},
	{EXTI_Line6,GPIO_PortSourceGPIOA,GPIO_PinSource6,EXTI9_5_IRQn,6},
	{EXTI_Line7,GPIO_PortSourceGPIOA,GPIO_PinSource7,EXTI9_5_IRQn,7},
	{EXTI_Line8,GPIO_PortSourceGPIOA,GPIO_PinSource8,EXTI9_5_IRQn,8},
	{EXTI_Line9,GPIO_PortSourceGPIOA,GPIO_PinSource9,EXTI9_5_IRQn,9},
	{EXTI_Line10,GPIO_PortSourceGPIOA,GPIO_PinSource10,EXTI15_10_IRQn,10},
	{EXTI_Line11,GPIO_PortSourceGPIOA,GPIO_PinSource11,EXTI15_10_IRQn,11},
	{EXTI_Line12,GPIO_PortSourceGPIOA,GPIO_PinSource12,EXTI15_10_IRQn,12},
	{EXTI_Line13,GPIO_PortSourceGPIOA,GPIO_PinSource13,EXTI15_10_IRQn,13},
	{EXTI_Line14,GPIO_PortSourceGPIOA,GPIO_PinSource14,EXTI15_10_IRQn,14},
	{EXTI_Line15,GPIO_PortSourceGPIOA,GPIO_PinSource15,EXTI15_10_IRQn,15},
	
	{EXTI_Line0,GPIO_PortSourceGPIOB,GPIO_PinSource0,EXTI0_IRQn,16},
	{EXTI_Line1,GPIO_PortSourceGPIOB,GPIO_PinSource1,EXTI1_IRQn,17},
	{EXTI_Line2,GPIO_PortSourceGPIOB,GPIO_PinSource2,EXTI2_IRQn,18},
	{EXTI_Line3,GPIO_PortSourceGPIOB,GPIO_PinSource3,EXTI3_IRQn,19},
	{EXTI_Line4,GPIO_PortSourceGPIOB,GPIO_PinSource4,EXTI4_IRQn,20},
	{EXTI_Line5,GPIO_PortSourceGPIOB,GPIO_PinSource5,EXTI9_5_IRQn,21},
	{EXTI_Line6,GPIO_PortSourceGPIOB,GPIO_PinSource6,EXTI9_5_IRQn,22},
	{EXTI_Line7,GPIO_PortSourceGPIOB,GPIO_PinSource7,EXTI9_5_IRQn,23},
	{EXTI_Line8,GPIO_PortSourceGPIOB,GPIO_PinSource8,EXTI9_5_IRQn,24},
	{EXTI_Line9,GPIO_PortSourceGPIOB,GPIO_PinSource9,EXTI9_5_IRQn,25},
	{EXTI_Line10,GPIO_PortSourceGPIOB,GPIO_PinSource10,EXTI15_10_IRQn,26},
	{EXTI_Line11,GPIO_PortSourceGPIOB,GPIO_PinSource11,EXTI15_10_IRQn,27},
	{EXTI_Line12,GPIO_PortSourceGPIOB,GPIO_PinSource12,EXTI15_10_IRQn,28},
	{EXTI_Line13,GPIO_PortSourceGPIOB,GPIO_PinSource13,EXTI15_10_IRQn,29},
	{EXTI_Line14,GPIO_PortSourceGPIOB,GPIO_PinSource14,EXTI15_10_IRQn,30},
	{EXTI_Line15,GPIO_PortSourceGPIOB,GPIO_PinSource15,EXTI15_10_IRQn,31}
	
	
};
////////////////////////////////////////////
/////////PWM pin support////////////////////
const PIN_TO_TIMx _PinTOTimx[]=
{
	//{TIMx,rcc,irqch,TIMxCHx,needremap,pin}
	#ifdef STM32F103C8T6

		{TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn,TIMxCH1,0,8},//暂时不支持
		{TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn,TIMxCH2,0,9},//暂时不支持
		{TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn,TIMxCH3,0,10},//暂时不支持
		{TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn,TIMxCH4,0,11},//暂时不支持

		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH1,0,0},
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH2,0,1},
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH3,0,2},
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH4,0,3},
		
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH1,0,6},
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH2,0,7},
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH3,0,16},
		{TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn,TIMxCH4,0,17},
		
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH1,0,22},
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH2,0,23},
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH3,0,24},
		{TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn,TIMxCH4,0,25},
		/////一下引脚需要开启ReMaping///////////////////////////
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH3,1,26},//pb10
		{TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn,TIMxCH4,1,27},//PB11
	#endif

};
////////////外设及其附属属性对应表///////////////////////////
const TIMx_INFO _TIMxInfo[]=
{
	{1,TIM1,RCC_APB2Periph_TIM1,TIM1_UP_IRQn},//暂时不支持
	{2,TIM2,RCC_APB1Periph_TIM2,TIM2_IRQn},
	{3,TIM3,RCC_APB1Periph_TIM3,TIM3_IRQn},
	{4,TIM4,RCC_APB1Periph_TIM4,TIM4_IRQn},
#ifdef STM32F10X_CL
	{5,TIM5,RCC_APB1Periph_TIM5,NULL},
	{6,TIM6,RCC_APB1Periph_TIM6,NULL},
	{7,TIM7,RCC_APB1Periph_TIM7,NULL},
#endif
};
////////////////////////////////////////////////////////////
const USARTx_INFO _USARTxInfo[]=
{
	{1,USART1,RCC_APB2Periph_USART1,USART1_IRQn},//暂时不支持
	{2,USART2,RCC_APB1Periph_USART2,USART2_IRQn},
	{3,USART3,RCC_APB1Periph_USART3,USART3_IRQn},
};
//////////////////////////////////////////////////////////////
TIM_TypeDef* PinToTIMx(uint8_t pin)
{
	int i;
	for(i = 0; i<PWM_PIN_NUM; i++)
	{
		if(_PinTOTimx[i].pin == pin)
			
			return _PinTOTimx[i].TIMx;
	}
	return 0;
}

uint8_t PinToTIM_ch(uint8_t pin)
{
		int i;

	for( i = 0; i<PWM_PIN_NUM; i++)
	{
		if(_PinTOTimx[i].pin == pin)
			
			return _PinTOTimx[i].ch;
	}
	return 0;
}
uint32_t PinToTIM_rcc(uint8_t pin)
{
		int i;

	for( i = 0; i<PWM_PIN_NUM; i++)
	{
		if(_PinTOTimx[i].pin == pin)
			
			return _PinTOTimx[i].rcc;
	}
	return 0;
}
uint32_t PinToTIM_irq(uint8_t pin)
{
		int i;

	for( i = 0; i<PWM_PIN_NUM; i++)
	{
		if(_PinTOTimx[i].pin == pin)
			
			return _PinTOTimx[i].irq;
	}
	return 0;
}

uint8_t IsPwmPin(uint8_t pin)
{
	int i;
	for( i = 0; i<PWM_PIN_NUM; i++)
	{
		if(_PinTOTimx[i].pin == pin)
			
			return 1;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
uint32_t  TIMxToRCC(TIM_TypeDef* TIMx)
{
	int i;
	uint32_t rcc;
	for(i=0;i<TIM_NUM;i++)
	{
		if(_TIMxInfo[i].timx == TIMx)
		{
					rcc = _TIMxInfo[i].rcc;
			break;

		}
	
	}
	return rcc;
}
uint32_t TIMxToIRQ(TIM_TypeDef* TIMx)
{
	uint32_t irq;
	int i;
	for(i=0;i<TIM_NUM;i++)
	{
		if(_TIMxInfo[i].timx == TIMx)
		{
			irq = _TIMxInfo[i].irq;
			break;
		}
	
	}	

	return irq;
}
uint8_t TIMxToID(TIM_TypeDef* TIMx)
{
	uint32_t id;
	int i;
	for(i=0;i<TIM_NUM;i++)
	{
		if(_TIMxInfo[i].timx == TIMx)
		{
			id = _TIMxInfo[i].id;
			break;
		}
	
	}	

	return id;
}

////////////////////////////////////////////////////
uint32_t  USARTxToRCC(USART_TypeDef* USARTx)
{
	int i;
	uint32_t rcc;
	for(i=0;i<USART_NUM;i++)
	{
		if(_USARTxInfo[i].usart == USARTx)
		{
					rcc = _USARTxInfo[i].rcc;
			break;

		}
	
	}
	return rcc;
}
uint32_t USARTxToIRQ(USART_TypeDef* USARTx)
{
	uint32_t irq;
	int i;
	for(i=0;i<USART_NUM;i++)
	{
		if(_USARTxInfo[i].usart == USARTx)
		{
			irq = _USARTxInfo[i].irq;
			break;
		}
	
	}	

	return irq;
}
uint8_t USARTxToID(USART_TypeDef* USARTx)
{
	uint32_t id;
	int i;
	for(i=0;i<USART_NUM;i++)
	{
		if(_USARTxInfo[i].usart == USARTx)
		{
			id = _USARTxInfo[i].id;
			break;
		}
	
	}	

	return id;
}

/////////////////////////////////////////////////////
uint8_t IsAnalogPin(uint8_t pin)
{
	int i;
	for( i = 0; i<ANALOG_PIN_NUM; i++)
	{
		if(_PinToAnalog[i].pin == pin)
			
			return 1;
	}
	return 0;
}
uint8_t IsPinNeedRemap(uint8_t pin)
{
	int i;

	for( i = 0; i<255; i++)
	{
		if(_PinTOTimx[i].pin == pin)
			
			return _PinTOTimx[i].needremap;
	}
		return 0XFF;
}
//#define GPIO_Remap_SPI1             ((uint32_t)0x00000001)  /*!< SPI1 Alternate Function mapping */
//#define GPIO_Remap_I2C1             ((uint32_t)0x00000002)  /*!< I2C1 Alternate Function mapping */
//#define GPIO_Remap_USART1           ((uint32_t)0x00000004)  /*!< USART1 Alternate Function mapping */
//#define GPIO_Remap_USART2           ((uint32_t)0x00000008)  /*!< USART2 Alternate Function mapping */
//#define GPIO_PartialRemap_USART3    ((uint32_t)0x00140010)  /*!< USART3 Partial Alternate Function mapping */
//#define GPIO_FullRemap_USART3       ((uint32_t)0x00140030)  /*!< USART3 Full Alternate Function mapping */
//#define GPIO_PartialRemap_TIM1      ((uint32_t)0x00160040)  /*!< TIM1 Partial Alternate Function mapping */
//#define GPIO_FullRemap_TIM1         ((uint32_t)0x001600C0)  /*!< TIM1 Full Alternate Function mapping */
//#define GPIO_PartialRemap1_TIM2     ((uint32_t)0x00180100)  /*!< TIM2 Partial1 Alternate Function mapping */
//#define GPIO_PartialRemap2_TIM2     ((uint32_t)0x00180200)  /*!< TIM2 Partial2 Alternate Function mapping */
//#define GPIO_FullRemap_TIM2         ((uint32_t)0x00180300)  /*!< TIM2 Full Alternate Function mapping */
//#define GPIO_PartialRemap_TIM3      ((uint32_t)0x001A0800)  /*!< TIM3 Partial Alternate Function mapping */
//#define GPIO_FullRemap_TIM3         ((uint32_t)0x001A0C00)  /*!< TIM3 Full Alternate Function mapping */
//#define GPIO_Remap_TIM4             ((uint32_t)0x00001000)  /*!< TIM4 Alternate Function mapping */
//#define GPIO_Remap1_CAN1            ((uint32_t)0x001D4000)  /*!< CAN1 Alternate Function mapping */
//#define GPIO_Remap2_CAN1            ((uint32_t)0x001D6000)  /*!< CAN1 Alternate Function mapping */
//#define GPIO_Remap_PD01             ((uint32_t)0x00008000)  /*!< PD01 Alternate Function mapping */
//#define GPIO_Remap_TIM5CH4_LSI      ((uint32_t)0x00200001)  /*!< LSI connected to TIM5 Channel4 input capture for calibration */
//#define GPIO_Remap_ADC1_ETRGINJ     ((uint32_t)0x00200002)  /*!< ADC1 External Trigger Injected Conversion remapping */
//#define GPIO_Remap_ADC1_ETRGREG     ((uint32_t)0x00200004)  /*!< ADC1 External Trigger Regular Conversion remapping */
//#define GPIO_Remap_ADC2_ETRGINJ     ((uint32_t)0x00200008)  /*!< ADC2 External Trigger Injected Conversion remapping */
//#define GPIO_Remap_ADC2_ETRGREG     ((uint32_t)0x00200010)  /*!< ADC2 External Trigger Regular Conversion remapping */
//#define GPIO_Remap_ETH              ((uint32_t)0x00200020)  /*!< Ethernet remapping (only for Connectivity line devices) */
//#define GPIO_Remap_CAN2             ((uint32_t)0x00200040)  /*!< CAN2 remapping (only for Connectivity line devices) */
//#define GPIO_Remap_SWJ_NoJTRST      ((uint32_t)0x00300100)  /*!< Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST */
//#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */
//#define GPIO_Remap_SWJ_Disable      ((uint32_t)0x00300400)  /*!< Full SWJ Disabled (JTAG-DP + SW-DP) */
//#define GPIO_Remap_SPI3             ((uint32_t)0x00201100)  /*!< SPI3/I2S3 Alternate Function mapping (only for Connectivity line devices) */
//#define GPIO_Remap_TIM2ITR1_PTP_SOF ((uint32_t)0x00202000)  /*!< Ethernet PTP output or USB OTG SOF (Start of Frame) connected
//                                                                 to TIM2 Internal Trigger 1 for calibration
//                                                                 (only for Connectivity line devices) */
//#define GPIO_Remap_PTP_PPS          ((uint32_t)0x00204000)  /*!< Ethernet MAC PPS_PTS output on PB05 (only for Connectivity line devices) */

//#define GPIO_Remap_TIM15            ((uint32_t)0x80000001)  /*!< TIM15 Alternate Function mapping (only for Value line devices) */
//#define GPIO_Remap_TIM16            ((uint32_t)0x80000002)  /*!< TIM16 Alternate Function mapping (only for Value line devices) */
//#define GPIO_Remap_TIM17            ((uint32_t)0x80000004)  /*!< TIM17 Alternate Function mapping (only for Value line devices) */
//#define GPIO_Remap_CEC              ((uint32_t)0x80000008)  /*!< CEC Alternate Function mapping (only for Value line devices) */
//#define GPIO_Remap_TIM1_DMA         ((uint32_t)0x80000010)  /*!< TIM1 DMA requests mapping (only for Value line devices) */

//#define GPIO_Remap_TIM9             ((uint32_t)0x80000020)  /*!< TIM9 Alternate Function mapping (only for XL-density devices) */
//#define GPIO_Remap_TIM10            ((uint32_t)0x80000040)  /*!< TIM10 Alternate Function mapping (only for XL-density devices) */
//#define GPIO_Remap_TIM11            ((uint32_t)0x80000080)  /*!< TIM11 Alternate Function mapping (only for XL-density devices) */
//#define GPIO_Remap_TIM13            ((uint32_t)0x80000100)  /*!< TIM13 Alternate Function mapping (only for High density Value line and XL-density devices) */
//#define GPIO_Remap_TIM14            ((uint32_t)0x80000200)  /*!< TIM14 Alternate Function mapping (only for High density Value line and XL-density devices) */
//#define GPIO_Remap_FSMC_NADV        ((uint32_t)0x80000400)  /*!< FSMC_NADV Alternate Function mapping (only for High density Value line and XL-density devices) */

//#define GPIO_Remap_TIM67_DAC_DMA    ((uint32_t)0x80000800)  /*!< TIM6/TIM7 and DAC DMA requests remapping (only for High density Value line devices) */
//#define GPIO_Remap_TIM12            ((uint32_t)0x80001000)  /*!< TIM12 Alternate Function mapping (only for High density Value line devices) */
//#define GPIO_Remap_MISC             ((uint32_t)0x80002000)  /*!< Miscellaneous Remap (DMA2 Channel5 Position and DAC Trigger remapping, 
void ReMapCtr(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	//定时器2
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
//	
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
}
#ifdef STM32F103C8T6
void ReMap15_19_26_27ToTIM2(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
}
void ReMap15ToTIM2(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
}
void ReMap19ToTIM2(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
}
void ReMap16_17_20_21ToTIM3(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
}

#endif
