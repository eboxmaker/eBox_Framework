/**
  ******************************************************************************
  * @file    rtc.cpp
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
#include "ebox_rtc.h"
#include "ebox_config.h"

#if USE_PRINTF
// 是否打印调试信息, 1打印,0不打印
#define debug 1
#endif

#if debug
#define  DEBUG(...) DBG("[RTC]  "),DBG(__VA_ARGS__)
#else
#define  DEBUG(...)
#define	 DBG(...)
#endif



#define RTC_CFG_FLAG 0XA5A5
//Rtc rtc;
//callback_fun_type rtc_callback_table[3];//
static uint32_t rtc_irq_id;
static rtc_irq_handler irq_handler;

Date_T gDate;

static uint8_t _RTC_WeekDayNum(uint32_t nYear, uint8_t nMonth, uint8_t nDay)
{
  uint32_t year = 0, weekday = 0;

  year = 2000 + nYear;
  
  if(nMonth < 3)
  {
    /*D = { [(23 x month)/9] + day + 4 + year + [(year-1)/4] - [(year-1)/100] + [(year-1)/400] } mod 7*/
    weekday = (((23 * nMonth)/9) + nDay + 4 + year + ((year-1)/4) - ((year-1)/100) + ((year-1)/400)) % 7;
  }
  else
  {
    /*D = { [(23 x month)/9] + day + 4 + year + [year/4] - [year/100] + [year/400] - 2 } mod 7*/
    weekday = (((23 * nMonth)/9) + nDay + 4 + year + (year/4) - (year/100) + (year/400) - 2 ) % 7; 
  }

  return (uint8_t)weekday;
}

int Rtc::begin(ClockS clock)
{

    int ret = ENG;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    
    if(clock == clock_lse){
        if((_getTimeFlag() == 0)||(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET))
        {
            if(_config(clock) != EOK)
            {
                DEBUG("LSE时钟启动失败,使用LSI时钟\r\n");
                _config(clock_lsi);
            }    
        }
        else
        {
            /* Check if the Power On Reset flag is set */
            if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
            {
                 DEBUG("Power On Reset occurred.... \r\n\n");
            }
            /* Check if the Pin Reset flag is set */
            else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
            {
                 DEBUG("External Reset occurred....\r\n\n ");
            }
            
            DEBUG("\r\n No need to configure RTC....");
            DEBUG("\r\n step 0....");
            /* Wait for RTC registers synchronization */
            RTC_WaitForSynchro();
            /* Enable the RTC Second */
            RTC_ITConfig(RTC_IT_SEC, ENABLE);
            /* Wait until last write operation on RTC registers has finished */
            RTC_WaitForLastTask();
            ret = EOK;
        }	
        
    }else{
        DEBUG("使用LSI时钟\r\n");
        ret = _config(clock_lsi);
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
    int ret;
    uint32_t i=0;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();
    
    if(clock == clock_lse)
    {
        /* Enable LSE */
        RCC_LSEConfig(RCC_LSE_ON);
        /* Wait till LSE is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            i++;
            if(i >= 0x1fffff)
            {
                ret = ETIMEOUT;
                return ret;
            }
        }            
        /* Select LSE as RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    }
    else
    {
        RCC_LSICmd(ENABLE);
        /* Wait till LSI is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        /* Enable RTC Clock */
        RCC_RTCCLKCmd(ENABLE);
        _setTimeFlag(0xffff);   //清除备份域标志
    }

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(clock == clock_lse)
    {
        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
    }
    else
    {
        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(39999); /* RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */
    }

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();       
    
    nvic(ENABLE);
    return EOK;
}
/**
  *@brief    检查时钟是否配置过
  *@param    none
  *@retval   0 不支持或未配置
  */
uint8_t Rtc::_getTimeFlag(void)
{
    return (BKP_ReadBackupRegister(BKP_DR1) == RTC_CFG_FLAG);
}
/**
  *@brief    备份域写信息,用来识别时钟是否已经配置过
  *@param    none
  *@retval   none
  */
void Rtc::_setTimeFlag(uint16_t configFlag)
{
    BKP_WriteBackupRegister(BKP_DR1, configFlag);
}

/**
  *@brief    设置日期
  *@param    Date_T date 日期
  *@retval   none
  */
void Rtc::setDate(Date_T date)
{
   uint32_t counter_time = 0, counter_alarm = 0, hours = 0;

    /* Change the current date */
    gDate.Year  = date.Year;
    gDate.Month = date.Month;
    gDate.Day  = date.Day;

  /* WeekDay set by user can be ignored because automatically calculated */
  gDate.WeekDay = _RTC_WeekDayNum(gDate.Year,gDate.Month,gDate.Day);
  date.WeekDay = gDate.WeekDay;

  /* Reset time to be aligned on the same day */
  /* Read the time counter*/
  counter_time = get_counter();

  /* Fill the structure fields with the read parameters */
  hours = counter_time / 3600;
  if (hours > 24)
  {
    /* Set updated time in decreasing counter by number of days elapsed */
    counter_time -= ((hours / 24) * 24 * 3600);
    /* Write time counter in RTC registers */
    set_counter(counter_time);

    /* Read current Alarm counter in RTC registers */
    counter_alarm = _getAlarmCount();

    /* Set again alarm to match with new time if enabled */
    if (counter_alarm != (uint32_t)0xFFFFFFFF)
    {
      if(counter_alarm < counter_time)
      {
        /* Add 1 day to alarm counter*/
        counter_alarm += (uint32_t)(24 * 3600);
        RTC_WaitForLastTask();    
        RTC_SetAlarm(counter_alarm);
      }
    }  

  }
     
}

void Rtc::getDate(Date_T *date){
    date->Year = gDate.Year;
    date->Month = gDate.Month;
    date->Day = gDate.Day;
    date->WeekDay = gDate.WeekDay;
}    

/**
 *@name     setTime
 *@brief    设置时间,时间格式为bin
 *@param    Time_T time 时间
 *@retval   none
*/
void Rtc::setTime(Time_T time)
{
    uint32_t counter_time = 0;

    counter_time = (uint32_t)(((uint32_t)time.Hours * 3600) + \
                    ((uint32_t)time.Minutes * 60) + \
                    ((uint32_t)time.Seconds));
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();    
	set_counter(counter_time);
	_setTimeFlag(RTC_CFG_FLAG);	
}

void Rtc::getTime(Time_T *time)
{
    uint32_t counter_time = 0, counter_alarm = 0, days_elapsed = 0, hours = 0;

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask(); 
    
    counter_time = get_counter();
    
      /* Fill the structure fields with the read parameters */
    hours = counter_time / 3600;
    time->Minutes  = (uint8_t)((counter_time % 3600) / 60);
    time->Seconds  = (uint8_t)((counter_time % 3600) % 60);
    
    if (hours >= 24)
    {
        /* Get number of days elapsed from last calculation */
        days_elapsed = (hours / 24);
        /* Set Hours in RTC_TimeTypeDef structure*/
        time->Hours = (hours % 24);
        /* Read Alarm counter in RTC registers */
        counter_alarm = _getAlarmCount() ;

        /* 计算alarm剩余时间 (only if set and not yet expired)*/
        if ((counter_alarm != (uint32_t)0xFFFFFFFF) && (counter_alarm > counter_time))
        {
            counter_alarm -= counter_time;
        } else 
        {
        /* 如果 counter_alarm < counter_time ，alarm 已经发生且没有解除*/
            counter_alarm = (uint32_t)0xFFFFFFFF;
        }

        /* Set updated time in decreasing counter by number of days elapsed */
        counter_time -= (days_elapsed * 24 * 3600);
        /* Write time counter in RTC registers */
        RTC_SetCounter(counter_time);


        /* Set updated alarm to be set */
        if (counter_alarm != (uint32_t)0xFFFFFFFF)
        {
            counter_alarm += counter_time;
        }
        /* Write time counter in RTC registers */
        RTC_WaitForLastTask();    
        RTC_SetAlarm(counter_alarm);

        /* Update date */
        _updateDate(days_elapsed);
    }
    else 
    {
    time->Hours = hours;    
    }
    
	time->Format12_24 = 0;
}

void Rtc::_updateDate(uint32_t dayElapsed){
    uint32_t year = 0, month = 0, day = 0;
    uint32_t loop = 0;

    /* Get the current year*/
    year = gDate.Year;

    /* Get the current month and day */
    month = gDate.Month;
    day = gDate.Day;

    for (loop = 0; loop < dayElapsed; loop++)
    {
        if((month == 1) || (month == 3) || (month == 5) || (month == 7) || \
        (month == 8) || (month == 10) || (month == 12))
        {
            if(day < 31)
            {
                day++;
            } else     /* Date structure member: day = 31 */
            {
                if(month != 12)
                {
                      month++;
                      day = 1;
                }else     /* Date structure member: day = 31 & month =12 */
                {
                  month = 1;
                  day = 1;
                  year++;
                }
            }
        }else if((month == 4) || (month == 6) || (month == 9) || (month == 11))
        {
            if(day < 30)
            {
                day++;
            } else /* Date structure member: day = 30 */
            {
                month++;
                day = 1;
            }
        }else if(month == 2)
        {
            if(day < 28)
            {
            day++;
            }
        else if(day == 28)
        {
            /* Leap year */
            if(_isLeapYear(year))
            {
              day++;
            }
            else
            {
              month++;
              day = 1;
            }
        } else if(day == 29)
        {
            month++;
            day = 1;
        }
    }
    }

    /* Update year */
    gDate.Year = year;
    /* Update day and month */
    gDate.Month = month;
    gDate.Day = day;
    /* Update day of the week */
    gDate.WeekDay = _RTC_WeekDayNum(year, month, day);
}

/**
  *@brief    设置时间,时间格式为bin
  *@param    nYear 年份
  *@retval   1 闰年，0 平年
  */
uint8_t  Rtc::_isLeapYear(uint16_t nYear){
      if((nYear % 4) != 0) 
  {
    return 0;
  }
  
  if((nYear % 100) != 0) 
  {
    return 1;
  }
  
  if((nYear % 400) == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void Rtc::set_counter(uint32_t count)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Change the current time */
    RTC_SetCounter(count);
}

uint32_t Rtc::_getAlarmCount(void){
    uint16_t high1 = 0, low = 0;

    high1 = READ_REG(RTC->ALRH & RTC_CNTH_RTC_CNT);
    low   = READ_REG(RTC->ALRL & RTC_CNTL_RTC_CNT);

    return (((uint32_t) high1 << 16 ) | low);
}

void Rtc::setAlarm(Time_T alarm,uint32_t mask)
{
    uint32_t counter_alarm = 0, counter_time;
    Time_T _time;
    
    counter_alarm = (uint32_t)(((uint32_t)alarm.Hours * 3600) + \
                ((uint32_t)alarm.Minutes * 60) + \
                ((uint32_t)alarm.Seconds)); 
    
    /*GetTime function to update date if counter higher than 24 hours */
    getTime(&_time);    
    /* Convert time in seconds */
    counter_time = (uint32_t)(((uint32_t)_time.Hours * 3600) + \
                      ((uint32_t)_time.Minutes * 60) + \
                      ((uint32_t)_time.Seconds)); 
 
    /* Check that requested alarm should expire in the same day (otherwise add 1 day) */
    if (counter_alarm < counter_time)
    {
    /* Add 1 day to alarm counter*/
    counter_alarm += (uint32_t)(24 * 3600);
    }
//    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();    
    RTC_SetAlarm(counter_alarm);
}

void Rtc::alarmOnOff(FunctionalState state){
    alarm_interrupt(state);
    nvic(ENABLE);
}


void Rtc::nvic(FunctionalState state)
{

    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    //	NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = state;
    NVIC_Init(&NVIC_InitStructure);
}

void Rtc::attach_sec_interrupt(void (*cb_fun)(void))
{
   // rtc_callback_table[0] = cb_fun;
}
void Rtc::attach_alarm_interrupt(void (*cb_fun)(void))
{
//    rtc_callback_table[1] = cb_fun;
}
void Rtc::attach_overflow_interrupt(void (*cb_fun)(void))
{
   // rtc_callback_table[2] = cb_fun;
}

void Rtc::sec_interrupt(FunctionalState state)
{
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<0);
    else
         RTC->CRH &= ~(1<<0);
}

void Rtc::alarm_interrupt(FunctionalState state)
{
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<1);
    else
         RTC->CRH &= ~(1<<1);
}

void Rtc::overflow_interrupt(FunctionalState state)
{
     
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<2);
    else
         RTC->CRH &= ~(1<<2);
}


void Rtc::attach(void (*fptr)(void), Rtc_IrqType type) {
    if (fptr) {
        _irq[type].attach(fptr);
    }
}


void Rtc::_irq_handler(uint32_t id, Rtc_IrqType irq_type) {
    Rtc *handler = (Rtc*)id;
    handler->_irq[irq_type].call();
}

uint32_t Rtc::get_counter()
{ 
    uint16_t high1 = 0, high2 = 0, low = 0;
    uint32_t timecounter = 0;
    
    high1 = READ_REG(RTC->CNTH & RTC_CNTH_RTC_CNT);
    low   = READ_REG(RTC->CNTL & RTC_CNTL_RTC_CNT);
    high2 = READ_REG(RTC->CNTH & RTC_CNTH_RTC_CNT);
    
    if (high1 != high2)
    { /* In this case the counter roll over during reading of CNTL and CNTH registers, 
    read again CNTL register then return the counter value */
    timecounter = (((uint32_t) high2 << 16 ) | READ_REG(RTC->CNTL & RTC_CNTL_RTC_CNT));
    }
    else
    { /* No counter roll over during reading of CNTL and CNTH registers, counter 
    value is equal to first value of CNTL and CNTH */
    timecounter = (((uint32_t) high1 << 16 ) | low);
    }
    
    return timecounter;
};
extern "C" {
    void RTC_IRQHandler(void)
    {
        if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
        {
						irq_handler(rtc_irq_id,Sec_Irq);
            RTC_ClearITPendingBit(RTC_IT_SEC);
        }
        if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
        {
						irq_handler(rtc_irq_id,Alr_Irq);
            RTC_ClearITPendingBit(RTC_IT_ALR);
        }
        if (RTC_GetITStatus(RTC_IT_OW) != RESET)
        {
						irq_handler(rtc_irq_id,Ow_Irq);
            RTC_ClearITPendingBit(RTC_IT_OW);
        }
    }
		
		void rtc_irq_init(rtc_irq_handler handler, uint32_t id)
		{
				irq_handler = handler;
				rtc_irq_id = id;
		}
}
