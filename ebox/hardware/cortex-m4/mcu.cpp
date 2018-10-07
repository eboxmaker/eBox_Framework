



#include "ebox_core.h"
#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002

    __IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���

    __IO uint16_t micro_para;
    static void update_chip_info();
    static void update_system_clock(CpuClock_t *clock);
    void mcu_init(void)
    {
            update_system_clock(&cpu.clock);
            SysTick_Config(cpu.clock.core/1000);//  ÿ�� (nhz/168,000,000)s����һ���ж�
            micro_para = cpu.clock.core/1000000;//����micros����������
            NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

            update_chip_info();

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
   *@brief    us��ʱ,ʹ��systick��������48Mhzʱ��ʱ��������us(1.3)���ȡ�8Mhzʱ��С6-7us,24Mhzʱ��С2.2us,16Mhzʱ��С3.5us
   *@param    uint16_t us  Ҫ��ʱ��ʱ������С1us
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

    static void update_chip_info()
    {
        cpu.type = MCU_TYPE;
        cpu.pins = MCU_PINS;
        memcpy(cpu.company,"st\0",sizeof("st\0"));
        cpu.chip_id[2] = *(__IO uint32_t *)(0x1FFF7A10 + 0x00); //���ֽ�
        cpu.chip_id[1] = *(__IO uint32_t *)(0x1FFF7A10 + 0x04); //
        cpu.chip_id[0] = *(__IO uint32_t *)(0x1FFF7A10 + 0x08); //���ֽ�
        cpu.flash_size = *(uint16_t *)(0x1FFF7A10 + 0x12);   //оƬflash����
        
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
    
#ifdef __cplusplus
}
#endif
