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

#include "ebox_core.h"
#include "mcu.h"
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002
extern "C" {

    
    extern uint16_t  AD_value[];

    __IO uint64_t millis_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销
    __IO uint16_t micro_para;

    
	static void get_system_clock(CpuClock_t *clock);
    static void get_chip_info(void);
    __weak void SystemClock_Config()
	{
		/* Configuration will allow to reach a SYSCLK frequency set to 24MHz:
		 Syst freq = ((HSI_VALUE * PLLMUL)/ PLLDIV)
		             ((8MHz * 12)/ 4)                  = 24MHz             */
		LL_UTILS_PLLInitTypeDef sUTILS_PLLInitStruct = {LL_RCC_PLL_MUL_12 , LL_RCC_PREDIV_DIV_2}; ;

		/* Variable to store AHB and APB buses clock configuration */
		/* Settings to have HCLK set to 12MHz and APB to 6 MHz */
		LL_UTILS_ClkInitTypeDef sUTILS_ClkInitStruct = {LL_RCC_SYSCLK_DIV_1, LL_RCC_APB1_DIV_1};

		/* Switch to PLL with HSI as clock source             */
		LL_PLL_ConfigSystemClock_HSI(&sUTILS_PLLInitStruct, &sUTILS_ClkInitStruct);
	}

    void mcu_init(void)
    {
        SystemClock_Config();
        get_system_clock(&cpu.clock);


        SysTick_Config(cpu.clock.core/1000);//  每隔 1ms产生一次中断
        LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);//systemticks clock；
        micro_para = cpu.clock.core/1000000;//减少micros函数计算量
        
//        NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);
        
        get_chip_info();

        set_systick_user_event_per_sec(1000);


//        //将pb4默认设置为IO口，禁用jtag
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//        set_systick_user_event_per_sec(1000);

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
/**
 *@brief    获取系统时钟
 *@param    *clock：  时钟指针，返回系统时钟
 *@retval   none
*/
	static void get_system_clock(CpuClock_t *clock)
    {
		LL_RCC_ClocksTypeDef RCC_Clock;
		
		SystemCoreClockUpdate();
		LL_RCC_GetSystemClocksFreq(&RCC_Clock);
		
        clock->core = RCC_Clock.SYSCLK_Frequency;
        clock->hclk = RCC_Clock.HCLK_Frequency;
        //clock->pclk2 = clock->core;
        clock->pclk1 = RCC_Clock.PCLK1_Frequency;          
    }

/**
 *@brief    获取系统信息，CPU_ID,flashsize
 *@param    *clock：  时钟指针，返回系统时钟
 *@retval   none
*/    
    static void get_chip_info()
    {
        cpu.type = MCU_TYPE;
        cpu.pins = MCU_PINS;
        memcpy(cpu.company,MCU_COMPANY,sizeof(MCU_COMPANY));
        
        
        cpu.chip_id[2] = LL_GetUID_Word0(); //低字节
        cpu.chip_id[1] = LL_GetUID_Word1(); //
        cpu.chip_id[0] = LL_GetUID_Word2(); //高字节

        cpu.flash_size = LL_GetFlashSize();   //芯片flash容量



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
    
    uint32_t get_cpu_calculate_per_sec()
    {
        return cpu.ability;
    }

   

}
