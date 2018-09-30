/**
  ******************************************************************************
  * @file    core.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  		2019/9/28  �Ƴ����ô���,delay��������uint64_t��Ϊuint32_t		LQM
                   �޸�systemtick�жϻص�����������ע��
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "ebox_core.h"
#include "mcu.h"

#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002

extern "C" {
  __IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
  __IO uint16_t micro_para;

  static void update_system_clock(CpuClock_t *clock);
  static void update_chip_info(void);

  /**
    *@brief    ���ֿգ�Ƶ����system_stm32f10x.c�ж������
    *@param    mcu
    *@retval   none
    */
  __weak void SystemClock_Config()
  {

  }

  /**
    *@brief    ��ʼ��������ʱ�ӣ�systemtick��systemtick callback������jtag
    *@param    mcu
    *@retval   none
    */
  void mcu_init(void)
  {
    SystemClock_Config();
    // update ʱ����Ϣ
    update_system_clock(&cpu.clock);

    // ����systemtick
    SysTick_Config(cpu.clock.core/1000);//  ÿ�� 1ms����һ���ж�
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//systemticks clock��
    micro_para = cpu.clock.core/1000000;//����micros����������

    //��pb4Ĭ������ΪIO�ڣ�����jtag
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//    NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

    attachSystickCallBack(nullFun);
    update_chip_info();
  }

  void mcu_reset(void)
  {
    NVIC_SystemReset();
  }

  /**
    *@brief    ��ȡus
    *@param    mcu
    *@retval   none
    */
  uint64_t mcu_micros(void)
  {
    uint64_t micro;
    uint32_t temp = __get_PRIMASK();//����֮ǰ�ж�����
    no_interrupts();
    if (SysTick->CTRL & (1 << 16))//���������
    {
      if ( __get_IPSR() ||  (temp) ) //�����ʱ�����������жϻ��߱�����жϴ���޷�ִ�У�systick�жϺ���������Ҫ��millis_secend���в���
        millis_seconds++;
    }
    micro = (millis_seconds * 1000 + (1000 - (SysTick->VAL)/(micro_para)));
    __set_PRIMASK(temp);//�ָ�֮ǰ�ж�����

    return  micro;
  }

  /**
    *@brief    ��ȡms
    *@param    mcu
    *@retval   none
    */
  uint64_t mcu_millis( void )
  {
    return millis_seconds;
  }
  
  /**
    *@brief    ms��ʱ
    *@param    uint32_t ms  Ҫ��ʱ��ʱ������С1ms
    *@retval   none
   */
  void mcu_delay_ms(uint32_t ms)
  {
    uint64_t end ;
    end = mcu_micros() + ms * 1000 - 3;
    while (mcu_micros() < end);
  }
  /**
    *@brief    us��ʱ,ʹ��systick��������48Mhz������ʱ��ʱ��������us(1.3)���ȡ�
    *          8Mhzʱ��С6-7us,24Mhzʱ��С2.2us,16Mhzʱ��С3.5us
    *@param    uint32_t us  Ҫ��ʱ��ʱ������С1us
    *@retval   none
   */
  void  mcu_delay_us(uint32_t us)
  {
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;

    ticks = (us-1) *micro_para;             /* �������� */
    tcnt = 0;
    told = SysTick->VAL;               /* ���浱ǰ����ֵ */

    while (1)
    {
      tnow = SysTick->VAL;
      tcnt += (tnow < told)? (told-tnow):(SysTick->LOAD - tnow + told);
      if (tcnt >= ticks)break;
      told = tnow;
    }
  }

  // systick �жϻص�����ָ�룬
  fun_noPara_t  callBackFun;
  static uint16_t _multiple = 1;

  /**
  *@brief    ע���жϻص�����������ָ���������� = �ж����� * multiple
  *@param    SystickCallBack_T fun �ص��������޲Σ�, uint8_t multiple �����������趨��������
  *@retval   E_OK  ע��ɹ��� E_NG ע��ʧ��
  */
  uint16_t attachSystickCallBack(fun_noPara_t fun,uint16_t multiple){

    if (callBackFun == NULL || callBackFun == nullFun)
    {
      callBackFun = fun;
      _multiple = multiple == 0 ? 1 : multiple;
      return EOK;
    }else{
      return ENG;
    }
  }

  /**
   *@brief    systick�жϴ�����
   *@param    none
   *@retval   none
  */
  void SysTick_Handler(void)
  {
    if (millis_seconds++ % _multiple == 0)
    {
      callBackFun();
    }
  }

  /**
   *@brief    ��ȡϵͳʱ��
   *@param    *clock��  ʱ��ָ�룬����ϵͳʱ��
   *@retval   none
  */
  static void update_system_clock(CpuClock_t *clock)
  {
    RCC_ClocksTypeDef RCC_ClocksStatus;

    SystemCoreClockUpdate();
    RCC_GetClocksFreq(&RCC_ClocksStatus);

    clock->core = RCC_ClocksStatus.SYSCLK_Frequency;
    clock->hclk = RCC_ClocksStatus.HCLK_Frequency;
    clock->pclk2 = RCC_ClocksStatus.PCLK2_Frequency;
    clock->pclk1 = RCC_ClocksStatus.PCLK1_Frequency;
  }

  /**
   *@brief    ��ȡϵͳ��Ϣ��CPU_ID,flashsize
   *@param    none
   *@retval   none
  */
  static void update_chip_info()
  {
    cpu.type = MCU_TYPE;
    cpu.pins = MCU_PINS;
    memcpy(cpu.company,MCU_COMPANY,sizeof(MCU_COMPANY));

    cpu.chip_id[2] = *(__IO uint32_t *)(0X1FFFF7E8); //���ֽ�
    cpu.chip_id[1] = *(__IO uint32_t *)(0X1FFFF7EC); //
    cpu.chip_id[0] = *(__IO uint32_t *)(0X1FFFF7F0); //���ֽ�

    cpu.flash_size = *(uint16_t *)(0x1FFFF7E0);   //оƬflash����
    millis_seconds = 0;
    SysTick->VAL = 0;
    //ͳ��cpu��������//////////////////
    do
    {
      cpu.ability++;//ͳ��cpu��������
    }
    while (millis_seconds < 1);
    cpu.ability = cpu.ability  * 1000 * 2;
    ////////////////////////////////
  }

}
