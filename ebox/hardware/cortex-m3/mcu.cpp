/**
  ******************************************************************************
  * @file    core.cpp
  * @author  shentq
  * @version V2.1
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
#include "ebox_analog.h"

#include "ebox_core.h"
#include "mcu.h"
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002
extern "C" {


    extern uint16_t  AD_value[];

    __IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
    __IO uint16_t micro_para;

    static void update_system_clock(CpuClock_t *clock);
    static void update_chip_info(void);

    void mcu_init(void)
    {
        update_system_clock(&cpu.clock);
        SysTick_Config(cpu.clock.core/1000);//  ÿ�� 1ms����һ���ж�
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//systemticks clock��
        micro_para = cpu.clock.core/1000000;//����micros����������
        
        NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);
        
        update_chip_info();

        ADC1_init();


        //��pb4Ĭ������ΪIO�ڣ�����jtag
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
        set_systick_user_event_per_sec(1000);
        random_seed(AD_value[0]);//��ʼ�����������

    }
    void mcu_reset(void)
    {
        NVIC_SystemReset();
    }
    
    uint64_t mcu_micros(void)
    {
        uint64_t micro;
        uint32_t temp = __get_PRIMASK();//����֮ǰ�ж�����
        no_interrupts();
        if(SysTick->CTRL & (1 << 16))//���������
        {    
            if( __get_IPSR() ||  (temp) ) //�����ʱ�����������жϻ��߱�����жϴ���޷�ִ�У�systick�жϺ���������Ҫ��millis_secend���в���
            millis_seconds++;
        }
        micro = (millis_seconds * 1000 + (1000 - (SysTick->VAL)/(micro_para)));
        __set_PRIMASK(temp);//�ָ�֮ǰ�ж�����
        
        return  micro;
    }
    uint64_t mcu_millis( void )
    {
        return millis_seconds;
    }

    void mcu_delay_ms(uint64_t ms)
    {
        uint64_t end ;
        end = mcu_micros() + ms * 1000 - 3;
        while(mcu_micros() < end);
    }
    void mcu_delay_us(uint64_t us)
    {
        uint64_t end = mcu_micros() + us - 3;
        while(mcu_micros() < end);
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
        while(millis_seconds < 1);
        cpu.ability = cpu.ability  * 1000 * 2;
        ////////////////////////////////
    }
    
    uint32_t get_cpu_calculate_per_sec(void)
    {
        return cpu.ability;
    }


}
