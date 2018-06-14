#include "ebox_common.h"
#include "mcu_core.h"


#ifdef __cplusplus
extern "C" {
#endif
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002

cpu_t cpu;
__IO uint64_t millis_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销
 static void get_system_clock(cpu_clock_t *clock);

void ebox_init(void)
{
    get_system_clock(&cpu.clock);
    SysTick_Config(cpu.clock.core/1000);//  每隔 (nhz/168,000,000)s产生一次中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    ADC1_init();
}
uint64_t micros(void)
{
    if((SysTick->CTRL & (1 << 16)) && (__get_PRIMASK())) //如果此时屏蔽了所有中断且发生了systick溢出，需要对millis_secend进行补偿
    {
        millis_seconds++;
    }
    return  millis_seconds * 1000 + (1000 - SysTick->VAL / 168);
}
uint64_t millis( void )
{
    return micros() / 1000;
}

void delay_ms(uint64_t ms)
{
    uint64_t end ;
    end = micros() + ms * 1000;
    while(micros() < end);
}
void delay_us(uint64_t us)
{
    uint64_t end = micros() + us;
    while(micros() < end);
}


callback_fun_type systick_cb_table[1] = {0};
__IO uint16_t systick_user_event_per_sec;//真实的值
__IO uint16_t _systick_user_event_per_sec;//用于被millis_second取余数

void set_systick_user_event_per_sec(u16 frq)
{
    _systick_user_event_per_sec = 1000 / frq;
    systick_user_event_per_sec = frq;
}

void attach_systick_user_event(void (*callback_fun)(void))
{
    systick_cb_table[0] = callback_fun;
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
#ifdef __cplusplus
}
#endif
