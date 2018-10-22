/**
  ******************************************************************************
  * @file    ebox_rtc.h
  * @author  cat_li
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
#include "ebox_config.h"

#if USE_PRINTF
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 1
#endif

#if debug
#define  DEBUG(...) DBG("[RTC]  "),DBG(__VA_ARGS__)
#else
#define  DEBUG(...)
#define	 DBG(...)
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

fun_noPara_t rtc_callback;//

/**
  * @brief  �ȴ�Time&Date�Ĵ�����RTC(APB)ʱ��ͬ��
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
		if (IsTimeOut(end,RTC_TIMEOUT))
		{
			DEBUG("ʱ��ͬ����ʱ\r\n");
			return ETIMEOUT;
		}
	}
	return EOK;
}
/**
  * @brief  ���븳ֵģʽ
  * @note   ��ģʽ������ֹͣ�����Ը���RTCֵ
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
		if (IsTimeOut(end,RTC_TIMEOUT))
		{
			DEBUG("���븳ֵģʽ��ʱ,���ʹ��LSE,��ϵ�������ϵ磡\r\n");
			return ETIMEOUT;
		}
	}
	return EOK;
}

/**
  * @brief  �˳���ֵģʽ
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
  * @brief  ��ʼ��RTCʱ��
  * @param  ClockS clock ʱ��Դ��clock_lse,clock_lsi
  * @retval ETIMEOUT ʱ������ʧ��,EOK ʱ�����óɹ�,��������ʱ�ӣ�ENG ʱ�����óɹ�����Ҫ����ʱ��
  */
int Rtc::begin(ClockS clock)
{
	int ret = ENG;
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_PWR_EnableBkUpAccess();
	if (clock)	//LSE
	{
		if ((_getTimeFlag() == 0) )	//ʱ�ӵ��磬��Ҫ��������
		{
				if (_config(clock_lse) != EOK)
				{					
					DEBUG("LSEʱ������ʧ��,ʹ��LSIʱ��\r\n");
					//LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, 0x00);
					_config(clock_lsi);;
				}
		}
		else	// ʱ�ӱ��ֹ���������Ҫ����
		{
			DEBUG("ʹ��LSE,��������ʱ��,Alarm IT is %d \r\n",LL_RTC_IsEnabledIT_ALRA(RTC));      
			ret = EOK;
		}
	}else{	// ��������ʱ��ԴVDD�����RTC״̬������������Ҫ��ʼ��
		_config(clock_lsi);
	}
	_nvic();
	return ret;
}

/**
 *@name     _config
 *@brief    ����ʱ�ӣ���Ƶ����ʱ���ʽ
 *@param    ClockS clock 1 LSE,0 LSI
 *@retval   none
*/
int Rtc::_config(ClockS clock)
{
  uint32_t end = GetEndTime(5000);
  // Ĭ��Ϊ�ڲ�LSI
  uint32_t RTC_ASYNCH_PREDIV = LSI_ASYNCH_PREDIV;
  uint32_t RTC_SYNCH_PREDIV = LSI_SYNCH_PREDIV;

  /* To change the source clock of the RTC feature (LSE, LSI), you have to:
     - Enable the power clock
     - Enable write access to configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain
     - Configure the needed RTC clock source */
  /* ʹ��PWR��Դ&���������*/
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableBkUpAccess();
  /*##-2- Configure LSE/LSI as RTC clock source ###############################*/
  if (clock)		// 1 LSE
  {
    // ǿ�Ʊ���������
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    // �ⲿ����LSE����
    RTC_ASYNCH_PREDIV = LSE_ASYNCH_PREDIV;
    RTC_SYNCH_PREDIV = LSE_SYNCH_PREDIV;
    if (LL_RCC_LSE_IsReady() == 0)
    {
      // ʹ��LSE
      LL_RCC_LSE_Enable();
      while (LL_RCC_LSE_IsReady() != 1)
      {
        if (IsTimeOut(end,5000))
        {
          DEBUG("LSE δ����������ⲿ���� \r\n");
          return ETIMEOUT;
        }
      }
      // ѡ��LSE��Ϊʱ��Դ
      LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
      /* Enable RTC Clock */
      LL_RCC_EnableRTC();
      DEBUG("LSE �����ɹ���Alarm IT is %d \r\n",LL_RTC_IsEnabledIT_ALRA(RTC));
    }
  }else{				// 0 LSI
    /* Enable LSI */
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1)
    {
      if (IsTimeOut(end,5000))
      {
        return ETIMEOUT;
      }
    }
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    /* Enable RTC Clock */
    LL_RCC_EnableRTC();
    DEBUG("LSI �����ɹ���Alarm IT is %d \r\n",LL_RTC_IsEnabledIT_ALRA(RTC));
  }
  /* �ر�д�����������ʼ��ģʽ */
  LL_RTC_DisableWriteProtection(RTC);
  ebox_Enter_RTC_InitMode();
  
  /* ����ʱ���ʽ&��Ƶ���� */
  LL_RTC_SetHourFormat(RTC, LL_RTC_HOURFORMAT_24HOUR);

  LL_RTC_SetAsynchPrescaler(RTC, RTC_ASYNCH_PREDIV);    // �첽
  LL_RTC_SetSynchPrescaler(RTC, RTC_SYNCH_PREDIV);      // ͬ��

  /* �˳���ʼ��ģʽ��ʹ��д���� */
  ebox_Exit_RTC_InitMode();
  LL_RTC_EnableWriteProtection(RTC);
  return EOK;
}

void Rtc::_setFormat(uint32_t format){
  LL_RTC_DisableWriteProtection(RTC);
  ebox_Enter_RTC_InitMode();
  // ����ʱ���ʽ
  LL_RTC_SetHourFormat(RTC, format);
  ebox_Exit_RTC_InitMode();
  LL_RTC_EnableWriteProtection(RTC);
}

/**
 *@brief    ���ʱ���Ƿ����ù�
 *@param    none
 *@retval   0 ��֧�ֻ�δ����
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
 *@brief    ������д��Ϣ,����ʶ��ʱ���Ƿ��Ѿ����ù�
 *@param    none
 *@retval   none
*/
void Rtc::_setTimeFlag(void)
{
#if defined(RTC_BACKUP_SUPPORT)
	/*##-8- Writes a data in a RTC Backup data Register1 #######################*/
	LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, RTC_BKP_DATE_TIME_UPDTATED);
#endif
}

/**
 *@brief    ��������
 *@param    Date_T date ����
 *@retval   none
*/
void Rtc::setDate(Date_T date)
{
	/*##-1- �ر�д�����������ʼ��ģʽ */
	LL_RTC_DisableWriteProtection(RTC);
	ebox_Enter_RTC_InitMode();
	/* Set Date*/
	LL_RTC_DATE_Config(RTC, __LL_RTC_CONVERT_BIN2BCD(date.WeekDay), __LL_RTC_CONVERT_BIN2BCD(date.Day), __LL_RTC_CONVERT_BIN2BCD(date.Month), __LL_RTC_CONVERT_BIN2BCD(date.Year));
	
	/*##-3 �˳���ʼ������д����*/
	ebox_Exit_RTC_InitMode();	
	LL_RTC_EnableWriteProtection(RTC);
}

/**
 *@name     setTime
 *@brief    ����ʱ��
 *@param    Time_T time ʱ��
 *@retval   none
*/
void Rtc::setTime(Time_T time)
{
	/*##-1- �ر�д�����������ʼ��ģʽ */
	LL_RTC_DisableWriteProtection(RTC);
	ebox_Enter_RTC_InitMode();
	
	/* Set Time*/
	LL_RTC_TIME_Config(RTC,__LL_RTC_CONVERT_BIN2BCD(time.Format12_24), __LL_RTC_CONVERT_BIN2BCD(time.Hours), __LL_RTC_CONVERT_BIN2BCD(time.Minutes), __LL_RTC_CONVERT_BIN2BCD(time.Seconds));
	
	/*##-3 �˳���ʼ������д����*/
	ebox_Exit_RTC_InitMode();
	LL_RTC_EnableWriteProtection(RTC);
	
	_setTimeFlag();	
}

/**
 *@name     setAlarm
 *@brief    ��������
 *@param    Time_T time ����ʱ��,mask  ALARMA MASK
 *@retval   none
*/
void Rtc::setAlarm(Time_T time,uint32_t mask)
{
	/*##-1- �ر�д�����������ʼ��ģʽ */
	LL_RTC_DisableWriteProtection(RTC);
	ebox_Enter_RTC_InitMode();
	/*##-2- �ر����� */
	LL_RTC_ALMA_Disable(RTC);
  /*##-3- ��������&����λ */
	LL_RTC_ALMA_ConfigTime(RTC, __LL_RTC_CONVERT_BIN2BCD(time.Format12_24), __LL_RTC_CONVERT_BIN2BCD(time.Hours), __LL_RTC_CONVERT_BIN2BCD(time.Minutes), __LL_RTC_CONVERT_BIN2BCD(time.Seconds));
	LL_RTC_ALMA_SetMask(RTC, mask);

	/*##-4- ������,�����־λ */
	LL_RTC_ALMA_Enable(RTC);
	LL_RTC_ClearFlag_ALRA(RTC);
	/* Enable IT Alarm */
  /*##-5 �˳���ʼ������д����*/
	ebox_Exit_RTC_InitMode();
	LL_RTC_EnableWriteProtection(RTC);
}


void Rtc::_nvic(void)
{
	/* RTC Alarm Interrupt Configuration: EXTI configuration */
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_17);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_17);
	/*## Configure the NVIC for RTC Alarm ##################################*/
	NVIC_SetPriority(RTC_IRQn, 0x0F);
	NVIC_EnableIRQ(RTC_IRQn);
}

void Rtc::attach_alarm_interrupt(void (*cb_fun)(void))
{
    rtc_callback = cb_fun;
}

void Rtc::alarmOnOff(FunctionalState state)
{
	LL_RTC_DisableWriteProtection(RTC);	
	/* Clear the Alarm interrupt pending bit */
	LL_RTC_ClearFlag_ALRA(RTC);
	/* Enable IT Alarm */
	if (state == ENABLE)
		LL_RTC_EnableIT_ALRA(RTC);
	else
		LL_RTC_DisableIT_ALRA(RTC);	
	LL_RTC_EnableWriteProtection(RTC);
}

void Rtc::getDateTime(date_time_t *datetime)
{
	datetime->year 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
	datetime->month = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
	datetime->date 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
	datetime->week 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetWeekDay(RTC));

	datetime->hour 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
	datetime->min 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
	datetime->sec 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
};

void Rtc::getTime(Time_T *time)
{
	time->Format12_24 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetFormat(RTC));
	time->Hours 		= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
	time->Minutes 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
	time->Seconds 	= __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));
}

void Rtc::getDate(Date_T *date)
{
	date->Year	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
	date->Month	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
	date->Day 	 = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));
	date->WeekDay = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetWeekDay(RTC));
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
				rtc_callback();
				/* Clear the Alarm interrupt pending bit */
				LL_RTC_ClearFlag_ALRA(RTC);
			}
		}
		/* Clear the EXTI's Flag for RTC Alarm */
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_17);
	}
}



//Rtc rtc;
////callback_fun_type rtc_callback_table[3];//
//static uint32_t rtc_irq_id;
//static rtc_irq_handler irq_handler;

//int Rtc::begin(uint8_t clock_source)
//{

//    int ret = EOK;
//    /* Enable PWR and BKP clocks */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

//    /* Allow access to BKP Domain */
//    PWR_BackupAccessCmd(ENABLE);
//    /* Reset Backup Domain */
////    BKP_DeInit();


//    if(is_config(RTC_CFG_FLAG) == 0)
//    {
//        if(config(clock_source) != EOK)
//        {
//            config(0);
//            ret = EPARA;
//        }
//    
//        set_config_flag(RTC_CFG_FLAG);
//    }
//	else
//	{
//		/* Check if the Power On Reset flag is set */
//		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
//		{
//			ebox_printf("\r\n\n Power On Reset occurred....");
//		}
//		/* Check if the Pin Reset flag is set */
//		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
//		{
//			ebox_printf("\r\n\n External Reset occurred....");
//		}
//		
//		ebox_printf("\r\n No need to configure RTC....");
//		ebox_printf("\r\n step 0....");
//		/* Wait for RTC registers synchronization */
//		RTC_WaitForSynchro();
//		ebox_printf("\r\n step 1....");
//		
//		/* Enable the RTC Second */
//		RTC_ITConfig(RTC_IT_SEC, ENABLE);
//		ebox_printf("\r\n step 2....");
//		
//		/* Wait until last write operation on RTC registers has finished */
//		RTC_WaitForLastTask();
//        
//		ebox_printf("\r\n step 3....");
//	}
//	
//		rtc_irq_init(Rtc::_irq_handler, (uint32_t)this);
//    return ret;
//}
//int Rtc::config(uint8_t flag)
//{

//    int ret;
//    uint32_t i=0;
//    /* Enable PWR and BKP clocks */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

//    /* Allow access to BKP Domain */
//    PWR_BackupAccessCmd(ENABLE);

//    /* Reset Backup Domain */
//    BKP_DeInit();
//    
//    if(flag == 1)
//    {
//        /* Enable LSE */
//        RCC_LSEConfig(RCC_LSE_ON);
//        /* Wait till LSE is ready */
//        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//        {
//            i++;
//            if(i >= 0x1fffff)
//            {
//                ret = ETIMEOUT;
//                return ret;
//            }
//        }            
//        /* Select LSE as RTC Clock Source */
//        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
//    }
//    else
//    {
//        RCC_LSICmd(ENABLE);
//        /* Wait till LSI is ready */
//        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
//        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

//        /* Enable RTC Clock */
//        RCC_RTCCLKCmd(ENABLE);

//    }

//    /* Wait for RTC registers synchronization */
//    RTC_WaitForSynchro();

//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();

//    /* Enable the RTC Second */
//    RTC_ITConfig(RTC_IT_SEC, ENABLE);

//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//    if(flag == 1)
//    {
//        /* Set RTC prescaler: set RTC period to 1sec */
//        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
//    }
//    else
//    {
//        /* Set RTC prescaler: set RTC period to 1sec */
//        RTC_SetPrescaler(39999); /* RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */

//    }

//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//       
//    
//    nvic(ENABLE);

//    return EOK;
//}
//uint8_t Rtc::is_config(uint16_t configFlag)
//{
//    return (BKP_ReadBackupRegister(BKP_DR1) == configFlag);
//}
//void Rtc::set_config_flag(uint16_t configFlag)
//{
//    BKP_WriteBackupRegister(BKP_DR1, configFlag);
//}
//void Rtc::set_counter(uint32_t count)
//{
//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//    /* Change the current time */
//    RTC_SetCounter(count);

//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//}
//void Rtc::set_alarm(uint32_t count)
//{
//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//    /* Change the current time */
//    RTC_SetAlarm(count);

//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//}


//void Rtc::nvic(FunctionalState state)
//{

//    NVIC_InitTypeDef NVIC_InitStructure;

//    /* Configure one bit for preemption priority */
//    //	NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

//    /* Enable the RTC Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = state;
//    NVIC_Init(&NVIC_InitStructure);
//}

//void Rtc::attach_sec_interrupt(void (*cb_fun)(void))
//{
//   // rtc_callback_table[0] = cb_fun;
//}
//void Rtc::attach_alarm_interrupt(void (*cb_fun)(void))
//{
//    //rtc_callback_table[1] = cb_fun;
//}
//void Rtc::attach_overflow_interrupt(void (*cb_fun)(void))
//{
//   // rtc_callback_table[2] = cb_fun;
//}

//void Rtc::sec_interrupt(FunctionalState state)
//{
//    /* Wait for RTC registers synchronization */
//    RTC_WaitForSynchro();
//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//    if(state == ENABLE)
//        RTC->CRH |= (1<<0);
//    else
//         RTC->CRH &= ~(1<<0);
//}

//void Rtc::alarm_interrupt(FunctionalState state)
//{
//    /* Wait for RTC registers synchronization */
//    RTC_WaitForSynchro();
//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//    if(state == ENABLE)
//        RTC->CRH |= (1<<1);
//    else
//         RTC->CRH &= ~(1<<1);
//}

//void Rtc::overflow_interrupt(FunctionalState state)
//{
//     
//    /* Wait for RTC registers synchronization */
//    RTC_WaitForSynchro();
//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//    if(state == ENABLE)
//        RTC->CRH |= (1<<2);
//    else
//         RTC->CRH &= ~(1<<2);
//}


//void Rtc::attach(void (*fptr)(void), Rtc_IrqType type) {
//    if (fptr) {
//        _irq[type].attach(fptr);
//    }
//}


//void Rtc::_irq_handler(uint32_t id, Rtc_IrqType irq_type) {
//    Rtc *handler = (Rtc*)id;
//    handler->_irq[irq_type].call();
//}

//uint32_t Rtc::get_counter()
//{
//    return RTC_GetCounter();
//};
//extern "C" {
//    void RTC_IRQHandler(void)
//    {
//        if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
//        {
//						irq_handler(rtc_irq_id,Sec_Irq);
//            RTC_ClearITPendingBit(RTC_IT_SEC);
//        }
//        if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
//        {
//						irq_handler(rtc_irq_id,Alr_Irq);
//            RTC_ClearITPendingBit(RTC_IT_ALR);
//        }
//        if (RTC_GetITStatus(RTC_IT_OW) != RESET)
//        {
//						irq_handler(rtc_irq_id,Ow_Irq);
//            RTC_ClearITPendingBit(RTC_IT_OW);
//        }
//    }
//		
//		void rtc_irq_init(rtc_irq_handler handler, uint32_t id)
//		{
//				irq_handler = handler;
//				rtc_irq_id = id;
//		}
//}
