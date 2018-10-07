/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : cat_li
  * @version: V2.0
  * @date   : 2017/12/17
  * @brief   ebox application example .
  *	core API�й����޸�CPUƵ��,�Լ���ʱ���֡�
	*	1  ͨ���޸Ĳ�ͬ��CPUƵ�ʣ����Կ��������ٶȣ��Լ�us��ʱ��Ӱ��
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */
#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1 ��������ʾ������޸�mcu��ʱ�ӣ��Լ��ڲ�ͬƵ����cpu�����������仯
    *   2 ͨ��SystemClock_Config���Ը�����Ҫ�޸�mcuʱ��Դ���ú���ϵͳ���Զ����ã������ֶ�����
    *   3 f0����ͨ��st�ٷ���cubeMx������ɣ���project->setting->advanced setting,ѡ��LL����
    *   4 f1ϵ����Ϊʹ�ñ�׼�⣬�޷�ʹ��cubeMx������ɣ�system_stm32f10x.c����ЩĬ�����ã�����
    *     ͨ���궨��ѡ��
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32 cpuClock example"
#define EXAMPLE_DATE	"2017-12-31"

#ifdef F0
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_rcc.h"

// ͨ����ͬ����Ͽ������ò�ͬ�Ĺ���Ƶ�ʣ�Ĭ�Ϲ�����48M,����SystemClock_Config����
#define	CPUCLOCK96M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK72M		LL_RCC_PLL_MUL_9 ,  LL_RCC_PREDIV_DIV_1
#define CPUCLOCK48M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_2
#define CPUCLOCK24M		LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_4
#define CPUCLOCK16M		LL_RCC_PLL_MUL_2 , LL_RCC_PREDIV_DIV_1
#define CPUCLOCK08M		LL_RCC_PLL_MUL_2 , LL_RCC_PREDIV_DIV_2

#define CPUCLOCK			CPUCLOCK48M

void SystemClock_Config(void)
{
  /* Configuration will allow to reach a SYSCLK frequency set to 24MHz:
   Syst freq = ((HSI_VALUE * PLLMUL)/ PLLDIV)
  						 ((8MHz * 12)/ 2)                  = 48MHz             */
  LL_UTILS_PLLInitTypeDef sUTILS_PLLInitStruct = {CPUCLOCK};
  /* Variable to store AHB and APB buses clock configuration */
  /* Settings to have HCLK set to 48MHz and APB to 48 MHz */
  LL_UTILS_ClkInitTypeDef sUTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};

  /* Switch to PLL with HSI as clock source             */
  LL_PLL_ConfigSystemClock_HSI(&sUTILS_PLLInitStruct, &sUTILS_ClkInitStruct);
}
#else
void SystemClock_Config(void)
{
  // f1ϵ�п���ֱ���޸�system_stm32f10x.c�еĺ궨���޸�Ƶ�ʣ�Ҳ���������ﶨ�����
}
#endif

void setup()
{
  ebox_init();
  UART.begin(115200);
  print_log(EXAMPLE_NAME,EXAMPLE_DATE);
  LED1.mode(OUTPUT_PP);
}

void testUs()
{
  uint8_t i = 255;
  while (i--){
    LED1.toggle();
    delay_us(1);
  }
}

void testMs()
{
  uint8_t i = 20;
  while (i--){
    LED1.toggle();
    delay_ms(1);
  }
}

int main(void){
  uint32_t t;
  setup();
  UART.printf("������ʱ��%d us,  %d ms \r\n",micros(),millis());
  while (1)
  {
    t = micros();
    testUs();
    t = micros() - t;
    UART.printf("testUs ��ʱ��%dus \r\n",t);
    // ʹ��millis_seconds,���ٵ�������
    t = millis_seconds;
    testMs();
    t = millis_seconds - t;
    UART.printf("testMs ��ʱ��%dms \r\n",t);
    delay_ms(5000);
  }
}
