//#include "ebox_common.h"
#include "mcu.h"
#include "ebox_core.h"

#ifdef __cplusplus
extern "C" {
#endif
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002

cpu_t mcu;
__IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
 static void get_system_clock(cpu_clock_t *clock);
    __IO uint16_t micro_para;

void mcu_init(void)
{
        get_system_clock(&mcu.clock);
        SysTick_Config(mcu.clock.core/1000);//  ÿ�� (nhz/168,000,000)s����һ���ж�
        #ifdef __CC_ARM
            ebox_heap_init((void*)STM32_SRAM_BEGIN, (void*)STM32_SRAM_END);
        #elif __ICCARM__
            rt_system_heap_init(__segment_end("HEAP"), (void*)STM32_SRAM_END);
        #else
            rt_system_heap_init((void*)&__bss_end, (void*)STM32_SRAM_END);
        #endif
        micro_para = mcu.clock.core/1000000;//����micros����������
        mcu.ability = 0;
        millis_seconds = 0;
        //ͳ��cpu��������//////////////////
        do
        {
            mcu.ability++;//ͳ��cpu�������� 
        }
        while(millis_seconds < 100);
        mcu.ability = mcu.ability * 10;
        ////////////////////////////////
        NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);


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
    end = millis_seconds + ms ;
    while(millis_seconds < end);
}
void mcu_delay_us(uint64_t us)
{
    uint64_t end = mcu_micros() + us;
    while(mcu_micros() < end);
}


callback_fun_type systick_cb_table[1] = {0};
__IO uint16_t systick_user_event_per_sec;//��ʵ��ֵ
__IO uint16_t _systick_user_event_per_sec;//���ڱ�millis_secondȡ����

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
#ifdef __cplusplus
}
#endif
