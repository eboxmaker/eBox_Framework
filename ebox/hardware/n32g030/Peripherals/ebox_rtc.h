/**
  ******************************************************************************
  * @file    rtc.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  *          2018/11/3  LQM
  *          修改接口，和f0保持一直，利用f1的rtc计数器，提供 年，月，日，时，分，秒
  *          驱动，提供一个闹铃
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
#include "time.h"
/**
  *1.提供一个32位的循环计数,每秒加1.
  *2.一个中断源，三个中断事件
  *3.支持一个简易时钟
  *4.支持一个闹钟
  */
typedef struct
{
    uint32_t Format12_24;	//
    uint8_t Hours;			// 0-12 or 0-23
    uint8_t Minutes;		// 0-59
    uint8_t Seconds;		// 0-59
} Time_T;

typedef struct
{
    Time_T time;
    uint32_t Day;
    uint32_t WeekDaySel;
} Alarm_T;

typedef struct
{
    uint8_t WeekDay;	// 周一到周日 1-7
    uint8_t Day;			// 1-31
    uint8_t Month;		// 1-12
    uint8_t Year;			// 0-99
} Date_T;

typedef struct
{
    uint8_t Year;
    uint8_t Month;
    uint8_t Day;
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
    uint8_t WeekDay;
} date_time_t;

typedef enum
{
    clock_lsi = 0,
    clock_lse
} ClockS;

enum Rtc_IrqType
{
    Sec_Irq = 0,
    Alr_Irq = 1,
    Ow_Irq = 2
};

#define eRtc_NoMask                         0//LL_RTC_ALMA_MASK_NONE          // 不屏蔽,date,hour,minutes,seconds均参与匹配
#define eRtc_MaskDateWeekDay                0//LL_RTC_ALMA_MASK_DATEWEEKDAY   // 不匹配年月日
#define eRtc_MaskHours                      0//LL_RTC_ALMA_MASK_HOURS         // 不匹配小时
#define eRtc_MaskMinutes                    0//LL_RTC_ALMA_MASK_MINUTES       // 不匹配分钟
#define eRtc_MaskSeconds                    0//LL_RTC_ALMA_MASK_SECONDS       // 不匹配秒
#define eRtc_MaskAll                        0//(RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1) //屏蔽所有位,此时闹铃每秒触发一次

typedef void (*rtc_irq_handler)(uint32_t id, Rtc_IrqType type);

class Rtc
{
public:
    /**
    	* @brief  初始化RTC,默认LSE,如果LSE启动失败,会自动转为LSI
    	* @param  ClockS clock 时钟源，clock_lse,clock_lsi
    	* @retval E_TIMEOUT 时钟配置失败,E_OK 时钟配置成功,无需设置时钟，E_NG 时钟配置成功，需要设置时钟
    	*/
    int begin(ClockS clock = clock_lse);


    /** 设置闹铃
     *  @param time 定时时间
     *  @param mask 屏蔽闹铃响应位(详细看define)，默认不匹配日期。f1系列中该参数无效，f0中有效。
     */
    void setAlarm(Time_T time, uint32_t mask = eRtc_MaskDateWeekDay);
    void setInterrupt(FunctionalState state, Rtc_IrqType type = Alr_Irq);

    // 设置日期，时间
    void setDate(Date_T date);
    void setTime(Time_T time);
    // 读取日期，时间
    void getDateTime(date_time_t *datetime);
    void getTime(Time_T *time);
    void getDate(Date_T *date);


    /** 绑定回调函数
     *  @param fptr 函数指针，指向一个无返回值的子函数, 或 0 指向空函数
     *  @param type 中断类型，默认 alarm中断。f1系列中该参数有效，f0中无效
     */
    void attach(void (*fptr)(void), Rtc_IrqType type = Alr_Irq);

    /** 绑定成员回调函数
     *  @param tptr 对象指针，一般为类名
     *  @param mptr 成员指针，格式一般为 &类名::成员名
     *  @param type 中断类型，默认 alarm中断。f1系列中该参数有效，f0中无效
     */
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void), Rtc_IrqType type = Sec_Irq)
    {
        if ((mptr != NULL) && (tptr != NULL))
        {
            _irq[type].attach(tptr, mptr);
        }
    }
private:
    int         _config(ClockS clock);
    void        _setTimeFlag(uint16_t configFlag);
    uint8_t     _getTimeFlag(void);
    void        _nvic(FunctionalState state);
    void        _setTimeCounter(uint32_t count);
    uint32_t    _getTimeCounter(void);
    uint32_t    _getAlarmCounter(void);
    void        _updateDate(uint32_t dayElapsed);
    uint8_t     _isLeapYear(uint16_t nYear);
protected:
    FunctionPointer _irq[3];
    static void _irq_handler(uint32_t id, Rtc_IrqType irq_type);
};

#endif
