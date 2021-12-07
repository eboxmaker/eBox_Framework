/**
  ******************************************************************************
  * @file    core.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  		2019/9/28  移除无用代码,delay函数参数uint64_t改为uint32_t		LQM
                   修改systemtick中断回调函数，完善注释
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
__IO uint32_t milli_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销
__IO uint16_t micro_para;

static void update_system_clock(CpuClock_t *clock);
static void update_chip_info(void);

/**
  *@brief    保持空，频率由system_stm32f10x.c中定义决定
  *@param    mcu
  *@retval   none
  */
__weak void SystemClock_Config()
{

}

/**
  *@brief    初始化，设置时钟，systemtick，systemtick callback，禁用jtag
  *@param    mcu
  *@retval   none
  */
void mcu_init(void)
{
    SystemClock_Config();
    
    // update 时钟信息
    update_system_clock(&cpu.clock);

    // 配置systemtick
    SysTick_Config(cpu.clock.core / 1000); //  每隔 1ms产生一次中断
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//systemticks clock；
    micro_para = cpu.clock.core / 1000000; //减少micros函数计算量

    //将pb4默认设置为IO口，禁用jtag
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//    NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);
//    
    attachSystickCallBack(nullFun,1);
    update_chip_info();
}

void mcu_reset(void)
{
    NVIC_SystemReset();
}

/**
  *@brief    获取us
  *@param    mcu
  *@retval   none
  */
uint32_t mcu_micros(void)
{
    uint32_t micro;
    micro = (milli_seconds * 1000 + (1000 - (SysTick->VAL) / (micro_para)));
    return  micro;
}

/**
  *@brief    获取ms
  *@param    mcu
  *@retval   none
  */
uint32_t mcu_millis( void )
{
    return milli_seconds;
}

/**
  *@brief    ms延时
  *@param    uint32_t ms  要延时的时长，最小1ms
  *@retval   none
 */
void mcu_delay_ms(uint32_t ms)
{
    uint32_t end ;
    end = mcu_micros() + ms * 1000 ;
    while (mcu_micros() < end);
}
/**
  *@brief    us延时,使用systick计数器。48Mhz及以上时钟时可以满足us(1.3)精度。
  *          8Mhz时最小6-7us,24Mhz时最小2.2us,16Mhz时最小3.5us
  *@param    uint32_t us  要延时的时长，最小1us
  *@retval   none
 */
void  mcu_delay_us(uint32_t us)
{
    if(us == 0) return;
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;

    ticks = (us - 1) * micro_para;          /* 计数周期 */
    tcnt = 0;
    told = SysTick->VAL;               /* 保存当前计数值 */

    while (1)
    {
        tnow = SysTick->VAL;
        tcnt += (tnow < told) ? (told - tnow) : (SysTick->LOAD - tnow + told);
        if (tcnt >= ticks)break;
        told = tnow;
    }
}

// systick 中断回调函数指针，
fun_noPara_t  callBackFun;
static uint16_t _multiple = 1;

/**
*@brief    注册中断回调函数，可以指定调用周期 = 中断周期 * multiple
*@param    SystickCallBack_T fun 回调函数（无参）, uint8_t multiple 倍数。用来设定调用周期
*@retval   E_OK  注册成功， E_NG 注册失败
*/
uint16_t attachSystickCallBack(fun_noPara_t fun, uint16_t multiple)
{

    if (callBackFun == NULL || callBackFun == nullFun)
    {
        callBackFun = fun;
        _multiple = ( multiple == 0 ) ? 1 : multiple;
        return EOK;
    }
    else
    {
        return ENG;
    }
}

/**
 *@brief    systick中断处理函数
 *@param    none
 *@retval   none
*/
void SysTick_Handler(void)
{
    if (milli_seconds++ % _multiple == 0)
    {
        callBackFun();
    }
}

/**
 *@brief    获取系统时钟
 *@param    *clock：  时钟指针，返回系统时钟
 *@retval   none
*/
static void update_system_clock(CpuClock_t *clock)
{
    RCC_ClocksType RCC_ClocksStatus;

    SystemCoreClockUpdate();
    RCC_GetClocksFreqValue(&RCC_ClocksStatus);

    clock->core = RCC_ClocksStatus.SysclkFreq;
    clock->hclk = RCC_ClocksStatus.HclkFreq;
    clock->pclk2 = RCC_ClocksStatus.Pclk2Freq;
    clock->pclk1 = RCC_ClocksStatus.Pclk1Freq;
}

/**
 *@brief    获取系统信息，CPU_ID,flashsize
 *@param    none
 *@retval   none
*/
static void update_chip_info()
{
    cpu.type = MCU_TYPE;
    cpu.pins = MCU_PINS;
    memcpy(cpu.company, MCU_COMPANY, sizeof(MCU_COMPANY));

    uint8_t *p = (uint8_t *)(0x1FFFF4FC);
    for(int i = 0 ; i < 12; i++)
    {
        cpu.chip_id[i] = *p++;
    }

    uint32_t val = *(uint32_t *)(0x1FFFF508);
    cpu.flash.size = (val & 0x00f00000) >> 20;   //芯片flash容量
    switch(cpu.flash.size)
    {
        case 32:
        case 64:
        case 128:
            cpu.flash.page_size = 1024;
            break;
        default:
            cpu.flash.page_size = 2048;
            break;
    }
    cpu.flash.size = cpu.flash.size * 1024;
    cpu.flash.start = MCU_FLASH_BEGIN;
    cpu.flash.end = MCU_FLASH_BEGIN + cpu.flash.size - 1;
    cpu.flash.used = MCU_FLASH_USED;
    milli_seconds = 0;
    SysTick->VAL = SysTick->LOAD;
    //统计cpu计算能力//////////////////
    do
    {
        cpu.ability++;//统计cpu计算能力
    }
    while (milli_seconds < 1);
    cpu.ability = cpu.ability  * 1000 * 9;
    ////////////////////////////////
#if	EBOX_DEBUG
#endif
    }

}
