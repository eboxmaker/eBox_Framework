/**
  ******************************************************************************
  * @file    rtc.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __RTC_H
#define __RTC_H
#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"

typedef struct{
	uint32_t Format12_24;	//
	uint32_t Hours;			// 0-12 or 0-23
	uint32_t Minutes;		// 0-59
	uint32_t Seconds;		// 0-59
}Time_T;

typedef struct{
	Time_T time;
	uint32_t Day;
	uint32_t WeekDaySel;
}Alarm_T;

typedef struct{
	uint32_t WeekDay;	// 1-7
	uint32_t Day;		// 1-31
	uint32_t Month;		// 1-12
	uint32_t Year;		// 0-99
}Date_T;

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t week;
}date_time_t;

typedef enum 
{
	clock_lsi = 0,
	clock_lse
}ClockS;

enum Rtc_IrqType {
		Sec_Irq = 0,
		Alr_Irq = 1,
		Ow_Irq = 2,
};

#define eRtc_NoMask                         //LL_RTC_ALMA_MASK_NONE          // 不屏蔽,date,hour,minutes,seconds均参与匹配
#define eRtc_MaskDateWeekDay                0x00//LL_RTC_ALMA_MASK_DATEWEEKDAY   // 不匹配年月日
#define eRtc_MaskHours                      //LL_RTC_ALMA_MASK_HOURS         // 不匹配小时
#define eRtc_MaskMinutes                    //LL_RTC_ALMA_MASK_MINUTES       // 不匹配分钟
#define eRtc_MaskSeconds                    //LL_RTC_ALMA_MASK_SECONDS       // 不匹配秒
#define eRtc_MaskAll                        //(RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1) //屏蔽所有位,此时闹铃每秒触发一次

typedef void (*rtc_irq_handler)(uint32_t id, Rtc_IrqType type);

/*
	1.提供一个32位的循环计数,每秒加1.
	2.一个中断源，三个中断事件
	3.支持一个简易时钟
	4.支持一个闹钟
    
*/

class Rtc
{

public:
	/**
		* @brief  初始化RTC,默认LSE,如果LSE启动失败,会自动转为LSI
		* @param  ClockS clock 时钟源，clock_lse,clock_lsi
		* @retval E_TIMEOUT 时钟配置失败,E_OK 时钟配置成功,无需设置时钟，E_NG 时钟配置成功，需要设置时钟
		*/
    int begin(ClockS clock = clock_lse);

    // 设置闹铃，默认不匹配日期
    void setAlarm(Time_T time,uint32_t mask = eRtc_MaskDateWeekDay);	
    void attach_alarm_interrupt(void (*cb_fun)(void));
	void alarmOnOff(FunctionalState state);
  
  // 设置日期，时间
	void setDate(Date_T date);
	void setTime(Time_T time);
	// 读取日期，时间	
	void getDateTime(date_time_t *datetime);
	void getTime(Time_T *time);
	void getDate(Date_T *date); 

    void attach_overflow_interrupt(void (*cb_fun)(void));
    void attach_sec_interrupt(void (*cb_fun)(void));

    void overflow_interrupt(FunctionalState state);
    void alarm_interrupt(FunctionalState state);
    void sec_interrupt(FunctionalState state);


    void set_counter(uint32_t count);
    void set_alarm(uint32_t count);
    uint32_t get_counter();
    /** Attach a function to call whenever a serial interrupt is generated
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    void attach(void (*fptr)(void), Rtc_IrqType type=Alr_Irq);

    /** Attach a member function to call whenever a serial interrupt is generated
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void), Rtc_IrqType type=Sec_Irq) {
        if((mptr != NULL) && (tptr != NULL)) {
            _irq[type].attach(tptr, mptr);
        }
    }
		
		static void _irq_handler(uint32_t id, Rtc_IrqType irq_type);

private:
    int    	    _config(ClockS clock);
    void        _setTimeFlag(uint16_t configFlag);
	uint8_t     _getTimeFlag(void);
    void        nvic(FunctionalState state);
    uint32_t    _getAlarmCount(void);
    void        _updateDate(uint32_t dayElapsed);
    uint8_t     _isLeapYear(uint16_t nYear);
protected:
    FunctionPointer _irq[3];
};
class RtcClock
{
    public:
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
    public:
        void begin();
        void sec_event()
        {
            count ++;
            count %=3600*24;
            sec  = (count % 60);
            min  = (count % 3600)/ 60;
            hour = (count / 3600);
        }
        void set_clock(uint8_t h,uint8_t m,uint8_t s)
        {
            count = h*3600 + m*60 + s;        
        }
    private:
       uint32_t count;
};
extern Rtc 	rtc;




#ifdef __cplusplus
extern "C" {
#endif

void rtc_irq_init(rtc_irq_handler handler, uint32_t id);


#ifdef __cplusplus
}
#endif


#endif

