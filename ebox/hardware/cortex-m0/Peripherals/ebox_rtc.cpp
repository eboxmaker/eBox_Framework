/**
  ******************************************************************************
  * @file    rtc.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   STM32F0 @ HAL lib
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
#include "ebox_rtc.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_rtc.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_pwr.h"

#if USE_PRINTF
// 是否打印调试信息, 1打印,0不打印
#define debug 0
#endif

#if debug
#define  DEBUG(...) DBG("[RTC]  "),DBG(__VA_ARGS__)
#else
#define  DEBUG(...)
#endif

#define RTC_TIMEOUT	10000	// 5s
/* Define used to indicate date/time updated */
#define RTC_BKP_DATE_TIME_UPDTATED ((uint32_t)0x32F2)

/* ck_apre=LSIFreq/(ASYNC prediv + 1) with LSIFreq=40 kHz RC */
#define LSI_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define LSI_SYNCH_PREDIV           ((uint32_t)0x137)

/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 256Hz with LSEFreq=32768Hz */
#define LSE_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define LSE_SYNCH_PREDIV           ((uint32_t)0x00FF)

static uint32_t rtc_irq_id;
static rtc_irq_handler irq_handler;

void rtc_irq_init(rtc_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    rtc_irq_id = id;
}

/**
  * @brief  等待Time&Date寄存器与RTC(APB)时钟同步
  * @param  None
  * @retval E_OK,E_TIMEOUT
  */
__INLINE int8_t ebox_WaitForSynchro_RTC(void)
{
    uint32_t end = GetEndTime(RTC_TIMEOUT);
    /* Clear RSF flag */
    LL_RTC_ClearFlag_RS(RTC);
    /* Wait the registers to be synchronised */
    while (LL_RTC_IsActiveFlag_RS(RTC) != 1)
    {
        if (IsTimeOut(end, RTC_TIMEOUT))
        {
            DEBUG("时钟同步超时\r\n");
            return ETIMEOUT;
        }
    }
    return EOK;
}
/**
  * @brief  进入赋值模式
  * @note   该模式计数器停止，可以更新RTC值
  * @param  None
  * @retval E_OK,E_TIMEOUT
  */
int8_t ebox_Enter_RTC_InitMode(void)
{
    uint32_t end = GetEndTime(RTC_TIMEOUT);
    /* Set Initialization mode */
    LL_RTC_EnableInitMode(RTC);
    /* Check if the Initialization mode is set */
    while (LL_RTC_IsActiveFlag_INIT(RTC) != 1)
    {
        if (IsTimeOut(end, RTC_TIMEOUT))
        {
            DEBUG("进入赋值模式超时,如果使用LSE,请断电后重新上电！\r\n");
            return ETIMEOUT;
        }
    }
    return EOK;
}

/**
  * @brief  退出赋值模式
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
int8_t ebox_Exit_RTC_InitMode(void)
{
    LL_RTC_DisableInitMode(RTC);
    /* Wait for synchro */
    /* Note: Needed only if Shadow registers is enabled LL_RTC_IsShadowRegBypassEnabled function can be used */
    return (ebox_WaitForSynchro_RTC());
}

/**
  * @brief  初始化RTC时钟
  * @param  ClockS clock 时钟源，clock_lse,clock_lsi
  * @retval ETIMEOUT 时钟配置失败,EOK 时钟配置成功,无需设置时钟，ENG 时钟配置成功，需要设置时钟
  */
int Rtc::begin(ClockS clock)
{

    int ret = ENG;
    /* Enable PWR and BKP clocks */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    LL_PWR_EnableBkUpAccess();

    if (clock == clock_lse)
    {
        if ((_getTimeFlag() == 0) )	//时钟掉电，需要重新设置
        {
            if (_config(clock_lse) != EOK)
            {
                DEBUG("LSE时钟启动失败,使用LSI时钟\r\n");
                //LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, 0x00);
                _config(clock_lsi);;
            }
        }
        else	// 时钟保持工作，不需要设置
        {
            DEBUG("使用LSE,无需配置时钟,Alarm IT is %d \r\n", LL_RTC_IsEnabledIT_ALRA(RTC));
            ret = EOK;
        }
    }
    else 	// 其他两种时钟源VDD掉电后RTC状态不定，所以需要初始化
    {
        _config(clock_lsi);
    }
    rtc_irq_init(Rtc::_irq_handler, (uint32_t)this);
    return ret;
}

/**
  *@name     _config
  *@brief    配置时钟，分频器，时间格式
  *@param    ClockS clock 1 LSE,0 LSI
  *@retval   none
  */
int Rtc::_config(ClockS clock)
{
    uint32_t end = GetEndTime(5000);
    // 默认为内部LSI
    uint32_t RTC_ASYNCH_PREDIV = LSI_ASYNCH_PREDIV;
    uint32_t RTC_SYNCH_PREDIV = LSI_SYNCH_PREDIV;

    /* To change the source clock of the RTC feature (LSE, LSI), you have to:
       - Enable the power clock
       - Enable write access to configure the RTC clock source (to be done once after reset).
       - Reset the Back up Domain
       - Configure the needed RTC clock source */
    /* 使能PWR电源&备份域访问*/
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    LL_PWR_EnableBkUpAccess();
    /*##-2- Configure LSE/LSI as RTC clock source ###############################*/
    if (clock == clock_lse)		// 1 LSE
    {
        // 强制备份域重置
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        // 外部晶振LSE配置
        RTC_ASYNCH_PREDIV = LSE_ASYNCH_PREDIV;
        RTC_SYNCH_PREDIV = LSE_SYNCH_PREDIV;
        if (LL_RCC_LSE_IsReady() == 0)
        {
            // 使能LSE
            LL_RCC_LSE_Enable();
            while (LL_RCC_LSE_IsReady() != 1)
            {
                if (IsTimeOut(end, 5000))
                {
                    DEBUG("LSE 未启动，检查外部晶振 \r\n");
                    return ETIMEOUT;
                }
            }
            // 选择LSE作为时钟源
            LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
            /* Enable RTC Clock */
            LL_RCC_EnableRTC();
            DEBUG("LSE 启动成功！Alarm IT is %d \r\n", LL_RTC_IsEnabledIT_ALRA(RTC));
        }
    }
    else 				// 0 LSI
    {
        /* Enable LSI */
        LL_RCC_LSI_Enable();
        while (LL_RCC_LSI_IsReady() != 1)
        {
            if (IsTimeOut(end, 5000))
            {
                return ETIMEOUT;
            }
        }
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
        /* Enable RTC Clock */
        LL_RCC_EnableRTC();
        DEBUG("LSI 启动成功！Alarm IT is %d \r\n", LL_RTC_IsEnabledIT_ALRA(RTC));
    }
    /* 关闭写保护并进入初始化模式 */
    LL_RTC_DisableWriteProtection(RTC);
    ebox_Enter_RTC_InitMode();

    /* 设置时间格式&分频因子 */
    LL_RTC_SetHourFormat(RTC, LL_RTC_HOURFORMAT_24HOUR);

    LL_RTC_SetAsynchPrescaler(RTC, RTC_ASYNCH_PREDIV);    // 异步
    LL_RTC_SetSynchPrescaler(RTC, RTC_SYNCH_PREDIV);      // 同步

    /* 退出初始化模式并使能写保护 */
    ebox_Exit_RTC_InitMode();
    LL_RTC_EnableWriteProtection(RTC);
    return EOK;
}

/**
  *@brief    检查时钟是否配置过
  *@param    none
  *@retval   0 不支持或未配置
  */
uint8_t Rtc::_getTimeFlag(void)
{
#if defined(RTC_BACKUP_SUPPORT)
    return (LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR1) == RTC_BKP_DATE_TIME_UPDTATED);
#else
    return 0;
#endif
}

/**
 *@brief    备份域写信息,用来识别时钟是否已经配置过
 *@param    none
 *@retval   none
*/
void Rtc::_setTimeFlag(uint16_t configFlag)
{
#if defined(RTC_BACKUP_SUPPORT)
    /*##-8- Writes a data in a RTC Backup data Register1 #######################*/
    LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, RTC_BKP_DATE_TIME_UPDTATED);
#endif
}

/**
  *@brief    设置日期
  *@param    Date_T date 日期
  *@retval   none
  */
void Rtc::setDate(Date_T date)
{
    /*##-1- 关闭写保护并进入初始化模式 */
    LL_RTC_DisableWriteProtection(RTC);
    ebox_Enter_RTC_InitMode();
    /* Set Date*/
    LL_RTC_DATE_Config(RTC, __LL_RTC_CONVERT_BIN2BCD(date.WeekDay), __LL_RTC_CONVERT_BIN2BCD(date.Day), __LL_RTC_CONVERT_BIN2BCD(date.Month), __LL_RTC_CONVERT_BIN2BCD(date.Year));

    /*##-3 退出初始化并打开写保护*/
    ebox_Exit_RTC_InitMode();
    LL_RTC_EnableWriteProtection(RTC);
}

/**
  *@name     setTime
  *@brief    设置时间,时间格式为bin
  *@param    Time_T time 时间
  *@retval   none
  */
void Rtc::setTime(Time_T time)
{
    /*##-1- 关闭写保护并进入初始化模式 */
    LL_RTC_DisableWriteProtection(RTC);
    ebox_Enter_RTC_InitMode();

    /* Set Time*/
    LL_RTC_TIME_Config(RTC, __LL_RTC_CONVERT_BIN2BCD(time.Format12_24), __LL_RTC_CONVERT_BIN2BCD(time.Hours), __LL_RTC_CONVERT_BIN2BCD(time.Minutes), __LL_RTC_CONVERT_BIN2BCD(time.Seconds));

    /*##-3 退出初始化并打开写保护*/
    ebox_Exit_RTC_InitMode();
    LL_RTC_EnableWriteProtection(RTC);

    _setTimeFlag(0);
}

/**
  *@brief    设置闹铃
  *@param    Time_T time 闹铃时间,mask  ALARMA MASK
  *@retval   none
  */
void Rtc::setAlarm(Time_T alarm, uint32_t mask)
{
    /*##-1- 关闭写保护并进入初始化模式 */
    LL_RTC_DisableWriteProtection(RTC);
    ebox_Enter_RTC_InitMode();
    /*##-2- 关闭闹铃 */
    LL_RTC_ALMA_Disable(RTC);
    _updateTime(&alarm);
    /*##-3- 设置闹铃&屏蔽位 */
    LL_RTC_ALMA_ConfigTime(RTC, __LL_RTC_CONVERT_BIN2BCD(alarm.Format12_24), __LL_RTC_CONVERT_BIN2BCD(alarm.Hours), __LL_RTC_CONVERT_BIN2BCD(alarm.Minutes), __LL_RTC_CONVERT_BIN2BCD(alarm.Seconds));
    LL_RTC_ALMA_SetMask(RTC, mask);

    /*##-4- 打开闹铃,清除标志位 */
    LL_RTC_ALMA_Enable(RTC);
    LL_RTC_ClearFlag_ALRA(RTC);
    /* Enable IT Alarm */
    /*##-5 退出初始化并打开写保护*/
    ebox_Exit_RTC_InitMode();
    LL_RTC_EnableWriteProtection(RTC);
}

/**
  *@brief    开启，关闭闹铃
  *@param    FunctionalState state ENABLE DISABLE
  *@retval   none
  */
void Rtc::setInterrupt(FunctionalState state, Rtc_IrqType type)
{
    LL_RTC_DisableWriteProtection(RTC);
    /* Clear the Alarm interrupt pending bit */
    LL_RTC_ClearFlag_ALRA(RTC);
    /* Enable IT Alarm */
    _nvic(state);
    (state == ENABLE) ? LL_RTC_EnableIT_ALRA(RTC) : LL_RTC_DisableIT_ALRA(RTC);
    LL_RTC_EnableWriteProtection(RTC);
}

/**
  *@brief    获取当前日期时间
  *@param    date_time_t *datetime
  *@retval   none
  */
void Rtc::getDateTime(date_time_t *datetime)
{
    datetime->Year 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
    datetime->Month = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
    datetime->Day 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
    datetime->WeekDay 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetWeekDay(RTC));

    datetime->Hours 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
    datetime->Minutes 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
    datetime->Seconds 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
}

/**
  *@brief    获取当前时间
  *@param    Time_T *time
  *@retval   none
  */
void Rtc::getTime(Time_T *time)
{
    time->Format12_24 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetFormat(RTC));
    time->Hours 		= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
    time->Minutes 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
    time->Seconds 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
}

/**
  *@brief    获取当前日期
  *@param    Date_T *date
  *@retval   none
  */
void Rtc::getDate(Date_T *date)
{
    date->Year	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
    date->Month	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
    date->Day 	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
    date->WeekDay = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetWeekDay(RTC));
}

void Rtc::attach(void (*fptr)(void), Rtc_IrqType type)
{
    if (fptr)
    {
        _irq.attach(fptr);
    }
}

void Rtc::_irq_handler(uint32_t id, Rtc_IrqType irq_type)
{
    Rtc *handler = (Rtc *)id;
    handler->_irq.call();
}

void Rtc::_nvic(FunctionalState state)
{
    /* RTC Alarm Interrupt Configuration: EXTI configuration */
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);
    /*## Configure the NVIC for RTC Alarm ##################################*/
    NVIC_SetPriority(RTC_IRQn, 0x0F);
    (state == ENABLE) ? NVIC_EnableIRQ(RTC_IRQn) : NVIC_DisableIRQ(RTC_IRQn);
}

void Rtc::_setFormat(uint32_t format)
{
    LL_RTC_DisableWriteProtection(RTC);
    ebox_Enter_RTC_InitMode();
    // 设置时间格式
    LL_RTC_SetHourFormat(RTC, format);
    ebox_Exit_RTC_InitMode();
    LL_RTC_EnableWriteProtection(RTC);
}

/**
  *@brief    更新时间，防止时，分，秒超范围
  *@param    Time_T *time
  *@retval   none
  */
void Rtc::_updateTime(Time_T *time)
{
    if (time->Seconds >= 60)
    {
        time->Seconds -= 60;
        time->Minutes += 1;
    }

    if (time->Minutes >= 60)
    {
        time->Minutes -= 60;
        time->Hours += 1;
    }

    if (time->Hours >= 24) time->Hours -= 24;
}

extern "C" {
    void RTC_IRQHandler(void)
    {
        /* Get the Alarm interrupt source enable status */
        if (LL_RTC_IsEnabledIT_ALRA(RTC) != 0)
        {
            /* Get the pending status of the Alarm Interrupt */
            if (LL_RTC_IsActiveFlag_ALRA(RTC) != 0)
            {
                /* Alarm callback */
                irq_handler(rtc_irq_id, Sec_Irq);
                /* Clear the Alarm interrupt pending bit */
                LL_RTC_ClearFlag_ALRA(RTC);
            }
        }
        /* Clear the EXTI's Flag for RTC Alarm */
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_17);
    }
}

