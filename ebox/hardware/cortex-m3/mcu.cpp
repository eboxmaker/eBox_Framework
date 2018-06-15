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

    __IO uint64_t millis_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销
    __IO uint16_t micro_para;

    static void update_system_clock(CpuClock_t *clock);
    static void update_chip_info(void);

    void mcu_init(void)
    {
        update_system_clock(&cpu.clock);
        SysTick_Config(cpu.clock.core/1000);//  每隔 1ms产生一次中断
        SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//systemticks clock；
        micro_para = cpu.clock.core/1000000;//减少micros函数计算量
        
        NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);
        
        update_chip_info();

        ADC1_init();


        //将pb4默认设置为IO口，禁用jtag
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
        set_systick_user_event_per_sec(1000);
        random_seed(AD_value[0]);//初始化随机数种子

    }
    void mcu_reset(void)
    {
        NVIC_SystemReset();
    }
    
    uint64_t mcu_micros(void)
    {
        uint64_t micro;
        uint32_t temp = __get_PRIMASK();//保存之前中断设置
        no_interrupts();
        if(SysTick->CTRL & (1 << 16))//发生了溢出
        {    
            if( __get_IPSR() ||  (temp) ) //如果此时屏蔽了所有中断或者被别的中断打断无法执行，systick中断函数，则需要对millis_secend进行补偿
            millis_seconds++;
        }
        micro = (millis_seconds * 1000 + (1000 - (SysTick->VAL)/(micro_para)));
        __set_PRIMASK(temp);//恢复之前中断设置
        
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
    __IO uint16_t systick_user_event_per_sec;//真实的值
    __IO uint16_t _systick_user_event_per_sec;//用于被millis_second取余数

    void set_systick_user_event_per_sec(uint16_t frq)
    {
        _systick_user_event_per_sec = 1000 / frq;
        systick_user_event_per_sec = frq;
    }

    void attach_systick_user_event(void (*callback_fun)(void))
    {
        systick_cb_table[0] = callback_fun;
    }
    void SysTick_Handler(void)//systick中断
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

        cpu.chip_id[2] = *(__IO uint32_t *)(0X1FFFF7E8); //低字节
        cpu.chip_id[1] = *(__IO uint32_t *)(0X1FFFF7EC); //
        cpu.chip_id[0] = *(__IO uint32_t *)(0X1FFFF7F0); //高字节

        cpu.flash_size = *(uint16_t *)(0x1FFFF7E0);   //芯片flash容量
        
        millis_seconds = 0;
        SysTick->VAL = 0;
        //统计cpu计算能力//////////////////
        do
        {
            cpu.ability++;//统计cpu计算能力 
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
