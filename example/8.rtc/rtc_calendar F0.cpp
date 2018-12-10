/**
  ******************************************************************************
  * @file    rtc_calendar.cpp
  * @author  cat_li
  * @version V2.0
  * @date    2017/7/17
  * @brief   rtc 例程
	*     STM32F0RTC例程，注意，和F1系列不通用
  *    1 创建rtc对象
  * 	 2 调用begin()初始化时钟。1 LSE  0 LSI
  *	   3 设置时钟
  *	   4 设置闹铃，附加闹铃中断回调函数
	*		 5 时钟从LSI改为LSE时，需要断电重启一次LSE才生效

	已知bug：LSE时闹铃无效
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

#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1 此例程演示了如何设置时间，日期，闹铃
  * 2 F0内置RTC可以提供年月日，时分秒的运算。 F1仅内置了一个简单定时器，年月日，时分秒是通过软件形式提供。
  *   每天至少调用依次getTime函数，以更新年月日信息
  * 3 setAlarm需要提供定时时间和屏蔽位两个参数，默认屏蔽日期和星期。F0中屏蔽位有效，F1中无效
  * 4 setInterrupt用来开启关闭中断,第二个参数为中断类型，F0中无效，F1中可以分别响应闹铃，秒，计数溢出。默
      认为闹铃。attach中的irq_type同样默认闹铃，F0无效，F1有效
  * 5 setDate时，F0中星期由用户指定，F1中星期由软件运算得到。两者有差异
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32 RTC example"
#define EXAMPLE_DATE	"2018-11-04"

// 注意E_RTC没有显示构造函数，默认为无参构造，所以rtc后面不跟括号
Rtc rtc;

// 每分钟输出一次闹铃
void exit()
{
    Time_T time;
    UART.printf("\n\r\n\r");
    UART.printf("#######################  闹铃  ##############################\n\r");
    rtc.getTime(&time);
    UART.printf("#####                 %2d:%02d:%02d                       ######", time.Hours, time.Minutes, time.Seconds);
    time.Minutes += 1;
    rtc.setAlarm(time);
    UART.printf("\r\n");
    UART.printf("#############################################################\n\r");
}

void setup()
{
    //	Date_T date = {LL_RTC_WEEKDAY_WEDNESDAY, 13, LL_RTC_MONTH_SEPTEMBER, 16};
    //	Time_T time = {LL_RTC_TIME_FORMAT_AM_OR_24, 22, 56, 1};
    Date_T date = {0, 4, 11, 18};
    Time_T time = {0, 22, 56, 1};

    ebox_init();
    UART.begin(115200);

    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    // EOK,初始化成功，并且RTC时间在运行，不需要设置日期，时间。否则需要设置
    if (rtc.begin(clock_lse) != EOK)
    {
        rtc.setDate(date);
        rtc.setTime(time);
    }
    // 设置闹铃

    rtc.getTime(&time);
    time.Minutes += 1;
    time.Seconds += 0;
    rtc.setAlarm(time, eRtc_MaskDateWeekDay | eRtc_MaskHours);
    UART.printf("\n\r 闹铃时间设定为：%2d:%02d:%02d秒", time.Hours, time.Minutes, time.Seconds);
    rtc.attach(&exit, Alr_Irq);
    rtc.setInterrupt(ENABLE);
}

uint8_t Week[7][3] = {{"一"}, {"二"}, {"三"}, {"四"}, {"五"}, {"六"}, {"日"}};

int main(void)
{
    Time_T time;
    Date_T date;
    setup();
    while (1)
    {
        // 每30s读取一次时间
        rtc.getTime(&time);
        rtc.getDate(&date);
        UART.printf("\n\r %2d:%02d:%02d秒", time.Hours, time.Minutes, time.Seconds);
        UART.printf("\n\r 20%2d年%02d月%2d日 星期%02d", date.Year, date.Month, date.Day, date.WeekDay);
        delay_ms(30000);
    }
}
