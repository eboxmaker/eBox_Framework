/**
  ******************************************************************************
  * @file    core.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
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
#include "ebox_common.h"
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002
extern "C" {

    cpu_t cpu;

    extern uint16_t  AD_value[];

    __IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
    __IO uint16_t micro_para;


    void ebox_init(void)
    {
        get_system_clock(&cpu.clock);
        get_chip_info();
        cpu.company[0] = 'S';
        cpu.company[1] = 'T';
        cpu.company[2] = '\0';


        SysTick_Config(cpu.clock.core/1000);//  ÿ�� 1ms����һ���ж�
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//systemticks clock��
        micro_para = cpu.clock.core/1000000;//����micros����������
        
        
        //ͳ��cpu��������//////////////////
        cpu.ability = 0;
        millis_seconds = 0;
        do
        {
            cpu.ability++;//ͳ��cpu��������
        }
        while(millis_seconds < 100);
        cpu.ability = cpu.ability * 10;
        ////////////////////////////////
        ADC1_init();

        NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

        //��pb4Ĭ������ΪIO�ڣ�����jtag
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
        set_systick_user_event_per_sec(1000);
        random_seed(AD_value[0]);//��ʼ�����������

    }
    void ebox_reset()
    {
        NVIC_SystemReset();
    }
    
    uint64_t micros(void)
    {
        uint64_t micro;
        if((SysTick->CTRL & (1 << 16)) && (__get_PRIMASK())) //�����ʱ�����������ж��ҷ�����systick�������Ҫ��millis_secend���в���
        {
            millis_seconds++;
        }
        no_interrupts();
//        micro = (millis_seconds * 1000 + (1000 - (SysTick->VAL)/(cpu.clock.core/1000000)));
        micro = (millis_seconds * 1000 + (1000 - (SysTick->VAL)/(micro_para)));
        interrupts();
        return  micro;
    }
    uint64_t millis( void )
    {
        return millis_seconds;
    }

    void delay_ms(uint64_t ms)
    {
        uint64_t end ;
        end = micros() + ms * 1000 - 3;
        while(micros() < end);
    }
    void delay_us(uint64_t us)
    {
        uint64_t end = micros() + us - 3;
        while(micros() < end);
    }


    callback_fun_type systick_cb_table[1] = {0};
    __IO uint16_t systick_user_event_per_sec;//��ʵ��ֵ
    __IO uint16_t _systick_user_event_per_sec;//���ڱ�millis_secondȡ����

    void set_systick_user_event_per_sec(uint16_t frq)
    {
        _systick_user_event_per_sec = 1000 / frq;
        systick_user_event_per_sec = frq;
    }

    void attach_systick_user_event(void (*callback_fun)(void))
    {
        systick_cb_table[0] = callback_fun;
    }
    void SysTick_Handler(void)//systick�ж�
    {
        millis_seconds++;
        if((millis_seconds % _systick_user_event_per_sec) == 0)
        {
            if(systick_cb_table[0] != 0)
            {
                systick_cb_table[0]();
            }
        }

    }
	static void get_system_clock(cpu_clock_t *clock)
    {
        RCC_ClocksTypeDef RCC_ClocksStatus;
        
        SystemCoreClockUpdate();                
        RCC_GetClocksFreq(&RCC_ClocksStatus);
        
        clock->core = RCC_ClocksStatus.SYSCLK_Frequency;
        clock->hclk = RCC_ClocksStatus.HCLK_Frequency;
        clock->pclk2 = RCC_ClocksStatus.PCLK2_Frequency;
        clock->pclk1 = RCC_ClocksStatus.PCLK1_Frequency;       
    }

    
    static void get_chip_info()
    {
        cpu.chip_id[2] = *(__IO uint32_t *)(0X1FFFF7E8); //���ֽ�
        cpu.chip_id[1] = *(__IO uint32_t *)(0X1FFFF7EC); //
        cpu.chip_id[0] = *(__IO uint32_t *)(0X1FFFF7F0); //���ֽ�

        cpu.flash_size = *(uint16_t *)(0x1FFFF7E0);   //оƬflash����
    }
    
    uint32_t get_cpu_calculate_per_sec()
    {
        return cpu.ability;
    }


}
