



#include "ebox_core.h"
#include "mcu.h"

#ifdef __cplusplus
extern "C" {
#endif
#define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
#define systick_interrupt()     SysTick->CTRL |=0x0002

__IO uint32_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
__IO uint16_t micro_para;

static void update_chip_info();
static void update_system_clock(CpuClock_t *clock);
void SystemClock_Config(void);

void mcu_init(void)
{
    SystemClock_Config();
    update_system_clock(&cpu.clock);
    SysTick_Config(cpu.clock.core / 1000); //  ÿ�� (nhz/168,000,000)s����һ���ж�
    micro_para = cpu.clock.core / 1000000; //����micros����������
    //NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

    update_chip_info();
}

void mcu_reset(void)
{
    NVIC_SystemReset();
}

uint32_t mcu_micros(void)
{
    uint32_t micro;
    micro = (millis_seconds * 1000 + (1000 - (SysTick->VAL) / (micro_para)));
    return  micro;
}


uint32_t mcu_millis( void )
{
    return millis_seconds;
}

void mcu_delay_ms(uint32_t ms)
{
    uint32_t end ;
    end = mcu_micros() + ms * 1000 - 3;
    while(mcu_micros() < end);
}


//    void mcu_delay_us(uint32_t us)
//    {
//        uint32_t end = mcu_micros() + us;
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
    uint32_t told, tnow, tcnt = 0;

    ticks = (us - 1) * micro_para;          /* �������� */
    tcnt = 0;
    told = SysTick->VAL;               /* ���浱ǰ����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;
        tcnt += (tnow < told) ? (told - tnow) : (SysTick->LOAD - tnow + told);
        if (tcnt >= ticks)break;
        told = tnow;
    }
}

fun_noPara_t callBackFun;
static uint16_t _multiple = 1;


/**
*@brief    ע���жϻص�����������ָ���������� = �ж����� * multiple
*@param    SystickCallBack_T fun �ص��������޲Σ�, uint8_t multiple �����������趨��������
*@retval   E_OK  ע��ɹ��� E_NG ע��ʧ��
*/
uint16_t attachSystickCallBack(fun_noPara_t fun, uint16_t multiple)
{

    if (callBackFun == NULL || callBackFun == nullFun)
    {
        callBackFun = fun;
        _multiple = multiple == 0 ? 1 : multiple;
        return EOK;
    }
    else
    {
        return ENG;
    }
}
#include "stm32f3xx_ll_rcc.h"


static void update_system_clock(CpuClock_t *clock)
{
    LL_RCC_ClocksTypeDef RCC_Clocks;
    SystemCoreClockUpdate();
    
//      uint32_t SYSCLK_Frequency;        /*!< SYSCLK clock frequency */
//  uint32_t HCLK_Frequency;          /*!< HCLK clock frequency */
//  uint32_t PCLK1_Frequency;         /*!< PCLK1 clock frequency */
//  uint32_t PCLK2_Frequency;         /*!< PCLK2 clock frequency */
//} LL_RCC_ClocksTypeDef;

    LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
    clock->core = RCC_Clocks.SYSCLK_Frequency;
    clock->hclk = RCC_Clocks.HCLK_Frequency;
    clock->pclk1 = RCC_Clocks.PCLK1_Frequency;
    clock->pclk2 = RCC_Clocks.PCLK2_Frequency;
}
//Ĭ��72Mhz����
void SystemClock_Config(void)
{
   LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(72000000);

  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);

}


void SysTick_Handler(void)//systick�ж�
{
    if (millis_seconds++ % _multiple == 0)
    {
        if(callBackFun != 0)
        {
            callBackFun();
        }
    }
}

static void update_chip_info()
{
    cpu.type = MCU_TYPE;
    cpu.pins = MCU_PINS;
    memcpy(cpu.company, "st\0", sizeof("st\0"));
    
//    cpu.chip_id[2] = *(__IO uint32_t *)(0x1FFF7A10 + 0x00); //���ֽ�
//    cpu.chip_id[1] = *(__IO uint32_t *)(0x1FFF7A10 + 0x04); //
//    cpu.chip_id[0] = *(__IO uint32_t *)(0x1FFF7A10 + 0x08); //���ֽ�
//    cpu.flash_size = *(uint16_t *)(0x1FFF7A10 + 0x12);   //оƬflash����

    millis_seconds = 0;
    SysTick->VAL = SysTick->LOAD;
    /////////////////ͳ��cpu��������(�������Լ9��)//////////////////
    do
    {
        cpu.ability++;
    }
    while(millis_seconds < 1);//ͳ��cpu��������
    ////////////////////////////////////////////////////////////////
    cpu.ability = cpu.ability  * 1000 * 9;
    ////////////////////////////////
}

#ifdef __cplusplus
}
#endif
