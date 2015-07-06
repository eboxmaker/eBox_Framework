#include "rtc.h"

RTC_CLASS 	rtc;
callbackFun rtcCallbackTable[3];//

void RTC_CLASS::begin()
{
//	if(isConfig() == 0)
//	{
		config();
		setConfigFlag(RTC_CFG_FLAG);
//	}
//	else
//	{
//		/* Check if the Power On Reset flag is set */
//		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
//		{
//			;			
//		}
//		/* Check if the Pin Reset flag is set */
//		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
//		{
//			;
//		}
//		/* Wait for RTC registers synchronization */
//		RTC_WaitForSynchro();
//		
//		/* Wait until last write operation on RTC registers has finished */
//		RTC_WaitForLastTask();			
//	}
}
void RTC_CLASS::config(void)
{
	
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain */
	BKP_DeInit();
	if(RTC_CLOCK_SOURCE)
	{
		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{}
		
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
	
	}
	
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	if(RTC_CLOCK_SOURCE)
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

}
uint8_t RTC_CLASS::isConfig(uint16_t configFlag)
{
	uint8_t flag = 0;
	if(BKP_ReadBackupRegister(BKP_DR1) == configFlag)
		flag = 1;
	return flag;
}
void RTC_CLASS::setConfigFlag(uint16_t configFlag)
{
		BKP_WriteBackupRegister(BKP_DR1, configFlag);
}
void RTC_CLASS::setCounter(uint32_t count)
{
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Change the current time */
	RTC_SetCounter(count);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}	
void RTC_CLASS::setAlarm(uint32_t count)
{
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Change the current time */
	RTC_SetAlarm(count);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}	
	

void RTC_CLASS::interrupt(uint32_t bits,FunctionalState x)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = x;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	RTC_ITConfig(bits, x);
	
}
void RTC_CLASS::attachInterrupt(uint16_t event, void (*callbackFun)(void))
{
	switch(event)
	{
		case RTC_EVENT_OW:
			rtcCallbackTable[0] = callbackFun;
			break;
		case RTC_EVENT_ALR:
			rtcCallbackTable[1] = callbackFun;
			break;
		case RTC_EVENT_SEC:
			rtcCallbackTable[2] = callbackFun;
			break;
	}
}

uint32_t RTC_CLASS::getCounter()
{
	return RTC_GetCounter();
};
void RTC_CLASS::setAlarm(uint8_t h,uint8_t m,uint8_t s)
{
	uint32_t tmp = 0;
	tmp = h*3600 + m*60 + s;
	setAlarm(tmp);
};

void RTC_CLASS::getTimeHMS(uint8_t* h,uint8_t* m,uint8_t* s)
{
	uint32_t tmp = 0;
	tmp = getCounter() % 0x15180;
	*h = (tmp / 3600);
	*m = (tmp % 3600)/60;
	*s = (tmp % 3600) %60;
};
extern "C"{
	void RTC_CLASS::setTimeHMS(uint8_t h,uint8_t m,uint8_t s)
	{
		uint32_t tmp = 0;
		tmp = h*3600 + m*60 + s;
		setCounter(tmp);
	}
	void RTC_IRQHandler(void)
	{
		if (RTC_GetITStatus(RTC_IT_OW) != RESET)
		{	
			rtcCallbackTable[0]();
			RTC_ClearITPendingBit(RTC_IT_OW);
		}	
		if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
		{	
			rtcCallbackTable[1]();
			RTC_ClearITPendingBit(RTC_IT_ALR);
		}	
		if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
		{	

			rtcCallbackTable[2]();
			RTC_ClearITPendingBit(RTC_IT_SEC);
		}
	}
}
