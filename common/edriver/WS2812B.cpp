
/*===========================================================================
 *  ��������:
 *  Դ�ļ���: WS2812B.cpp
 *  ��������: 2017.09.14
 *  ��     ��:  xzwj
 *  ��	  վ��http://www.cnblogs.com/xzwj/
 *  �汾˵��:
 *---------------------------------------------------------------------------
 *  Ӳ������:
 *
 *---------------------------------------------------------------------------
 *  �������:
 *
 *---------------------------------------------------------------------------
 *  ��������:  ebox��WS2812B RGBLED������֧��TIM1-TIM4��Ӧ��PWM�����
			 TIM1 PA8 PA9 PA10 PA11(PA8 ����������BUG)  DMA5
			 TIM2 PA0 PA1 PA2  PA3						DMA2
			 TIM3 PA6 PA7 PB0  PB1						DMA3
			 TIM4 PB6 PB7 PB8  PB9						DMA7

			 ��ע����ʹ��ʱע��DMA�Ƿ�ϵͳ����������ռ�ã���ʹ�ô���1�ʹ���2
			 	   ʱ����ò�Ҫʹ��TIM2�� PA0 PA1 PA2 PA3����TIM4(PB6 PB7 PB8  PB9)
 *
 *
 *---------------------------------------------------------------------------
 * ����Ȩ��  Copyright(C)             All Rights Reserved
 * ��������  �˳������ڲ��ԣ�������ע����Ȩ��������Ϣ��
 *===========================================================================*/

#include "ebox.h"
#include "WS2812B.h"

 /*---------------------------------------------------------
 -	 �������ƣ�
 -	 ����������
 -	 �����б�
 -	 ���ؽ����
 -	 ��	   ע��
 ----------------------------------------------------------*/
 WS2812B::WS2812B(Gpio *pin)//(Pwm *pwm)//Pwm *pwmw,
 {
	 pwm_pin = pin;
 }

 void WS2812B::begin()
 {
    _init_info(pwm_pin);
    pwm_pin->mode(AF_PP);
	this->oc_polarity = TIM_OCPolarity_High;
    _base_init();
    _set_duty();
	_set_dma();
 }
 /*---------------------------------------------------------
 -	 �������ƣ�
 -	 ����������PWMģʽ����
 -	 �����б�
 -	 ���ؽ����
 -	 ��	   ע��
 ----------------------------------------------------------*/
void WS2812B::_set_duty(void)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode		= TIM_OCMode_PWM1;//TIM_OCMode_Timing;//
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse		= 0;
	TIM_OCInitStructure.TIM_OCPolarity	= TIM_OCPolarity_High;
    switch(ch)
    {
	    case TIMxCH1:
	        TIM_OC1Init(TIMx, &TIM_OCInitStructure);
	        break;
	    case TIMxCH2:
	        TIM_OC2Init(TIMx, &TIM_OCInitStructure);
	        break;
	    case TIMxCH3:
	        TIM_OC3Init(TIMx, &TIM_OCInitStructure);
	        break;
	    case TIMxCH4:
	        TIM_OC4Init(TIMx, &TIM_OCInitStructure);
	        break;
    }
}
 /*---------------------------------------------------------
 -	 �������ƣ�
 -	 ������������ʼ��
 -	 �����б�
 -	 ���ؽ����
 -	 ��	   ע��
 ----------------------------------------------------------*/
 void WS2812B::initialize(uint8_t length)
 {
 	begin();
	bbuffersize = (length * 24) + 43;

 }

 void WS2812B::_base_init(void)
 {
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 if(rcc == RCC_APB2Periph_TIM1 ||  rcc == RCC_APB2Periph_TIM8 )
	 {
		 RCC_APB2PeriphClockCmd(rcc, ENABLE);
		 TIM_CtrlPWMOutputs(TIMx,ENABLE);
	 }
	 else
	 RCC_APB1PeriphClockCmd(rcc, ENABLE);
	 TIM_TimeBaseStructure.TIM_Period		 = 90-1; // 800kHz
	 TIM_TimeBaseStructure.TIM_Prescaler	 = 0;
	 TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	 TIM_TimeBaseStructure.TIM_CounterMode	 = TIM_CounterMode_Up;
//	 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	 TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
//	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	 TIM_ARRPreloadConfig(TIMx, ENABLE);
//	 TIM_Cmd(TIMx, ENABLE); //

 }
 /*---------------------------------------------------------
 -	 �������ƣ�
 -	 ����������ͨ���趨��GPIO,��ȷ�� TIM RCC CH;
 -	 �����б�
 -	 ���ؽ����
 -	 ��	   ע��
 ----------------------------------------------------------*/
 void WS2812B::_init_info(Gpio *pwm_pin)
 {

    switch((uint8_t)pwm_pin->id)
    {
    //TIM2
    case PA0_ID:
        TIMx = TIM2;
        rcc = RCC_APB1Periph_TIM2;
        ch = TIMxCH1;//irq = TIM2_IRQn;
        break;
    case PA1_ID:
        TIMx = TIM2;
        rcc = RCC_APB1Periph_TIM2;
        ch = TIMxCH2;//irq = TIM2_IRQn;
        break;
    case PA2_ID:
        TIMx = TIM2;
        rcc = RCC_APB1Periph_TIM2;
        ch = TIMxCH3;//irq = TIM2_IRQn;
        break;
    case PA3_ID:
        TIMx = TIM2;
        rcc = RCC_APB1Periph_TIM2;
        ch = TIMxCH4;//irq = TIM2_IRQn;
        break;

    //TIM3
    case PA6_ID:
        TIMx = TIM3;
        rcc = RCC_APB1Periph_TIM3;
        ch = TIMxCH1;//irq = TIM3_IRQn;
        break;
    case PA7_ID:
        TIMx = TIM3;
        rcc = RCC_APB1Periph_TIM3;
        ch = TIMxCH2;//irq = TIM3_IRQn;

    //TIM1
    case PA8_ID:
        TIMx = TIM1;
        rcc = RCC_APB2Periph_TIM1;
        ch = TIMxCH1;//irq = TIM4_IRQn;
        break;
    case PA9_ID:
        TIMx = TIM1;
        rcc = RCC_APB2Periph_TIM1;
        ch = TIMxCH2;//irq = TIM4_IRQn;
        break;
    case PA10_ID:
        TIMx = TIM1;
        rcc = RCC_APB2Periph_TIM1;
        ch = TIMxCH3;//irq = TIM4_IRQn;
        break;
    case PA11_ID:
        TIMx = TIM1;
        rcc = RCC_APB2Periph_TIM1;
        ch = TIMxCH4;//irq = TIM4_IRQn;
        break;



    //TIM4
    case PB6_ID:
        TIMx = TIM4;
        rcc = RCC_APB1Periph_TIM4;
        ch = TIMxCH1;//irq = TIM4_IRQn;
        break;
    case PB7_ID:
        TIMx = TIM4;
        rcc = RCC_APB1Periph_TIM4;
        ch = TIMxCH2;//irq = TIM4_IRQn;
        break;
    case PB8_ID:
        TIMx = TIM4;
        rcc = RCC_APB1Periph_TIM4;
        ch = TIMxCH3;//irq = TIM4_IRQn;
        break;
    case PB9_ID:
        TIMx = TIM4;
        rcc = RCC_APB1Periph_TIM4;
        ch = TIMxCH4;//irq = TIM4_IRQn;
        break;

    //TIM3
    case PB0_ID:
        TIMx = TIM3;
        rcc = RCC_APB1Periph_TIM3;
        ch = TIMxCH3;//irq = TIM3_IRQn;
        break;
    case PB1_ID:
        TIMx = TIM3;
        rcc = RCC_APB1Periph_TIM3;
        ch = TIMxCH4;//irq = TIM3_IRQn;
        break;

    }
	 

 }

 /*---------------------------------------------------------
 -	 �������ƣ�
 -	 ����������DMA ����
 -	 �����б�
 -	 ���ؽ����
 -	 ��	   ע��
 ----------------------------------------------------------*/
 char WS2812B::_set_dma()
 {
 	 DMA_InitTypeDef DMA_InitStructure;
	 /* configure DMA */
	 /* DMA clock enable */
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	 switch((uint32_t)TIMx)
	 {
		case (uint32_t)TIM1:
		{
			dma_ch = DMA1_Channel5;
			switch(ch)
		    {
			    case TIMxCH1:
			        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM1->CCR1;//(uint32_t)TIM1_CCR1_Address; // physical address of Timer 3 CCR1
			        break;
			    case TIMxCH2:
			        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM1->CCR2;//(uint32_t)TIM1_CCR2_Address; // physical address of Timer 3 CCR1
			        break;
			    case TIMxCH3:
			        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM1->CCR3;//(uint32_t)TIM1_CCR3_Address; // physical address of Timer 3 CCR1
			        break;
			    case TIMxCH4:
			        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM1->CCR4;//(uint32_t)TIM1_CCR4_Address; // physical address of Timer 3 CCR1
			        break;
		    }
		}
		break;
		case (uint32_t)TIM2:
		{
			dma_ch = DMA1_Channel2;
			switch(ch)
			{
				case TIMxCH1:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM2->CCR1;//TIM2_CCR1_Address; // physical address of Timer 3 CCR1
					break;
				case TIMxCH2:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM2->CCR2;//TIM2_CCR2_Address; // physical address of Timer 3 CCR1
					break;
				case TIMxCH3:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM2->CCR3;//TIM2_CCR3_Address; // physical address of Timer 3 CCR1
					break;
				case TIMxCH4:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM2->CCR4;//TIM2_CCR4_Address; // physical address of Timer 3 CCR1
					break;
			}

		}
		break;
		case (uint32_t)TIM3:
		{
			dma_ch = DMA1_Channel3;
			switch(ch)
			{
				case TIMxCH1:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM3->CCR1;//(uint32_t)TIM3_CCR1_Address; // physical address of Timer 3 CCR1
					break;
				case TIMxCH2:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM3->CCR2;//(uint32_t)TIM3_CCR2_Address; // physical address of Timer 3 CCR1
					break;
				case TIMxCH3:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM3->CCR3; //(uint32_t)TIM3_CCR3_Address;// physical address of Timer 3 CCR1
					break;
				case TIMxCH4:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM3->CCR4;//(uint32_t)TIM3_CCR4_Address; // physical address of Timer 3 CCR1
					break;
			}

		}
		break;
		case (uint32_t)TIM4:
		{
			dma_ch = DMA1_Channel7;
			switch(ch)
			{
				case TIMxCH1:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM4->CCR1;//(uint32_t)TIM4_CCR1_Address; // physical address of Timer 3 CCR1
					break;
				case TIMxCH2:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM4->CCR2;//(uint32_t)TIM4_CCR2_Address; // physical address of Timer 3 CCR1
					break;
				case TIMxCH3:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM4->CCR3;//(uint32_t)TIM4_CCR3_Address; // physical address of Timer 3 CCR1
					break;
				case TIMxCH4:
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM4->CCR4;//(uint32_t)TIM4_CCR4_Address; // physical address of Timer 3 CCR1
					break;
			}
		}
		break;
	}
	 DMA_InitStructure.DMA_MemoryBaseAddr	  = (uint32_t)LED_BYTE_Buffer;		 // this is the buffer memory
	 DMA_InitStructure.DMA_DIR				  = DMA_DIR_PeripheralDST;						 // data shifted from memory to peripheral
	 DMA_InitStructure.DMA_BufferSize		  = 42;
	 DMA_InitStructure.DMA_PeripheralInc	  = DMA_PeripheralInc_Disable;
	 DMA_InitStructure.DMA_MemoryInc		  = DMA_MemoryInc_Enable;					 // automatically increase buffer index
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	 DMA_InitStructure.DMA_MemoryDataSize	  = DMA_MemoryDataSize_HalfWord;
	 DMA_InitStructure.DMA_Mode 			  = DMA_Mode_Normal;						 // stop DMA feed after buffer size is reached
	 DMA_InitStructure.DMA_Priority 		  = DMA_Priority_High;
	 DMA_InitStructure.DMA_M2M				  = DMA_M2M_Disable;

	 DMA_DeInit(dma_ch);//DMA1_Channel3
	 DMA_Init(dma_ch, &DMA_InitStructure);//DMA1_Channel3
	/* TIMx CCx DMA Request enable */
	 TIM_DMACmd(TIMx, TIM_DMA_Update, ENABLE);

 }

 /*---------------------------------------------------------
  -   �������ƣ�
  -   ����������һ��ȫ����ʾlen��led����ɫȫ��һ��
  -   �����б�
  -   ���ؽ����
  -   ��		ע��
  ----------------------------------------------------------*/
void WS2812B::send(uint8_t green,uint8_t red,uint8_t bule, uint16_t len)
{
	uint8_t i;
	uint16_t memaddr;
	uint16_t buffersize;
	buffersize = (len * 24) + 43;	// number of bytes needed is #LEDs * 24 bytes + 42 trailing bytes
	memaddr = 0;				// reset buffer memory index
//	busize = buffersize;
	while (len)
	{
		for(i=0; i<8; i++) // GREEN data
		{
			LED_BYTE_Buffer[memaddr] = ((green<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		for(i=0; i<8; i++) // RED
		{
			LED_BYTE_Buffer[memaddr] = ((red<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		for(i=0; i<8; i++) // BLUE
		{
			LED_BYTE_Buffer[memaddr] = ((bule<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}

		len--;
	}
//===================================================================//
//bug�����һ�����ڲ��β�֪��Ϊʲôȫ�Ǹߵ�ƽ��������һ������
  	LED_BYTE_Buffer[memaddr] = ((bule<<8) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
//===================================================================//
    memaddr++;
	while(memaddr < buffersize)
	{
		LED_BYTE_Buffer[memaddr] = 0;
		memaddr++;
	}
	DMA_SetCurrDataCounter(dma_ch, buffersize); 	//DMA1_Channel3 load number of bytes to be transferred
	DMA_Cmd(dma_ch, ENABLE); 			//DMA1_Channel3 enable DMA channel 6
	TIM_Cmd(TIMx, ENABLE); 						// enable Timer 3
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)) ; 	// wait until transfer complete
	TIM_Cmd(TIMx, DISABLE); 	// disable Timer 3
	DMA_Cmd(dma_ch, DISABLE); 			//DMA1_Channel3 disable DMA channel 6
	DMA_ClearFlag(DMA1_FLAG_TC3); 				// clear DMA1 Channel 6 transfer complete flag
}
/*---------------------------------------------------------
 -	 �������ƣ�
 -	 ���������� ��������ÿһ��LED����ɫ
 -	 �����б� green
 				red
 				bule
 				num  ��Ӧ��led���
 -	 ���ؽ����
 -	 ��	   ע��
 ----------------------------------------------------------*/

void WS2812B::set_color(uint8_t green,uint8_t red,uint8_t bule, uint16_t num)
{
	bmemaddr = num * 24;				// reset buffer memory index
	for(uint8_t i=0; i<8; i++) // GREEN data
	{
			LED_BYTE_Buffer[bmemaddr] = ((green << i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			bmemaddr++;
	}
	for(uint8_t i=0; i<8; i++) // RED
	{
			LED_BYTE_Buffer[bmemaddr] = ((red << i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			bmemaddr++;
	}
	for(uint8_t i=0; i<8; i++) // BLUE
	{
			LED_BYTE_Buffer[bmemaddr] = ((bule << i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			bmemaddr++;
	}
}
/*---------------------------------------------------------
 -	 �������ƣ�
 -	 ������������������led��ɫ���У���������led
 -	 �����б�
 -	 ���ؽ����
 -	 ��	   ע��
 ----------------------------------------------------------*/
void WS2812B::enable()
{
    bmemaddr++;
	while(bmemaddr < bbuffersize)
	{
		LED_BYTE_Buffer[bmemaddr] = 0;
		bmemaddr++;
	}
	DMA_SetCurrDataCounter(dma_ch, bbuffersize); 	//DMA1_Channel3 load number of bytes to be transferred
	DMA_Cmd(dma_ch, ENABLE); 			//DMA1_Channel3 enable DMA channel 6
	TIM_Cmd(TIMx, ENABLE); 						// enable Timer 3
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)) ; 	// wait until transfer complete
	TIM_Cmd(TIMx, DISABLE); 	// disable Timer 3
	DMA_Cmd(dma_ch, DISABLE); 			//DMA1_Channel3 disable DMA channel 6
	DMA_ClearFlag(DMA1_FLAG_TC3); 				// clear DMA1 Channel 6 transfer complete flag
}

