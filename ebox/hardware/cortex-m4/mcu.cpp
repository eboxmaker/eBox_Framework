



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

void mcu_init(void)
{
    update_system_clock(&cpu.clock);
    SysTick_Config(cpu.clock.core / 1000); //  ÿ�� (nhz/168,000,000)s����һ���ж�
    micro_para = cpu.clock.core / 1000000; //����micros����������
    NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

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

    
    uint8_t *p = (uint8_t *)(0x1FFF7A10);
    for(int i = 0 ; i < 12; i++)
    {
        cpu.chip_id[i] = *p++;
    }


    cpu.flash.size = *(uint16_t *)(0x1FFF7A22);   //оƬflash����
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
