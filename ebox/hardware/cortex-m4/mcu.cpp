



#include "ebox_core.h"
#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002

    __IO uint64_t millis_seconds;//提供一个mills()等效的全局变量。降低cpu调用开销

    __IO uint16_t micro_para;
    static void update_chip_info();
    static void update_system_clock(CpuClock_t *clock);
    void mcu_init(void)
    {
            update_system_clock(&cpu.clock);
            SysTick_Config(cpu.clock.core/1000);//  每隔 (nhz/168,000,000)s产生一次中断
            micro_para = cpu.clock.core/1000000;//减少micros函数计算量
            NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

            update_chip_info();

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

    void mcu_delay_ms(uint32_t ms)
    {
        uint64_t end ;
        end = mcu_micros() + ms * 1000 - 3;
        while(mcu_micros() < end);
    }
    
    
//    void mcu_delay_us(uint64_t us)
//    {
//        uint64_t end = mcu_micros() + us;
//        while(mcu_micros() < end);
//    }
 /**
   *@brief    us延时,使用systick计数器。48Mhz时钟时可以满足us(1.3)精度。8Mhz时最小6-7us,24Mhz时最小2.2us,16Mhz时最小3.5us
   *@param    uint16_t us  要延时的时长，最小1us
   *@retval   none
  */
  void  mcu_delay_us(uint32_t us)
  {
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;

    ticks = (us-1) *micro_para;             /* 计数周期 */
    tcnt = 0;
    told = SysTick->VAL;               /* 保存当前计数值 */

    while (1)
    {
      tnow = SysTick->VAL;
      tcnt += (tnow < told)? (told-tnow):(SysTick->LOAD - tnow + told);
      if (tcnt >= ticks)break;
      told = tnow;
    }
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

    static void update_chip_info()
    {
        cpu.type = MCU_TYPE;
        cpu.pins = MCU_PINS;
        memcpy(cpu.company,"st\0",sizeof("st\0"));
        cpu.chip_id[2] = *(__IO uint32_t *)(0x1FFF7A10 + 0x00); //低字节
        cpu.chip_id[1] = *(__IO uint32_t *)(0x1FFF7A10 + 0x04); //
        cpu.chip_id[0] = *(__IO uint32_t *)(0x1FFF7A10 + 0x08); //高字节
        cpu.flash_size = *(uint16_t *)(0x1FFF7A10 + 0x12);   //芯片flash容量
        
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
    
#ifdef __cplusplus
}
#endif
