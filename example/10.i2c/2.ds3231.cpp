/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"
#include "ds3231.h"

/**
	*	1	此例程需要调用eDrive目录下的ds3231驱动
	*	2	此例程演示了ds3231时钟芯片的基本操作
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"ds3231 example"
#define EXAMPLE_DATE	"2018-08-11"

ChinaCalendar Cdt;

DS3231 ds(&sI2c1,0xD0);

DateTime_t dt;
DateTime_t dt1;

uint32_t last_time;

void setup()
{
    ebox_init();
    UART.begin(115200);
    UART.setTimeout(10);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    ds.begin();
    DataU16_t buf[3];
    
    buf[0].byte[0] = 30;
    buf[0].byte[1] = 37;
    buf[1].byte[0] = 20;
    buf[1].byte[1] = 10;
    buf[2].byte[0] = 11;
    buf[2].byte[1] = 19;
//    ds.set_dt_mb(buf);
    
    dt.year = buf[2].byte[1];
    dt.month = buf[2].byte[0];
    dt.date = buf[1].byte[1];
    dt.hour = buf[1].byte[0];
    dt.min = buf[0].byte[1];
    dt.sec = buf[0].byte[0];
//    ds.set_dt(dt);
    
    
    
    String str = "19-11-21 14:17:00";
//    ds.set_dt_string(str);

    ds.set_dt_string("80-11-7 17:45:0");
//    t.year = 19;
//    t.month = 11;
//    t.date = 10;
//    t.hour = 23;
//    t.min = 59;
//    t.sec = 55;

}
int main(void)
{
    setup();
    while(1)
    {
        ds.loop();

        dt = ds.get_dt();


        String dtstr = uart1.readString();
        if(dtstr != "")
        {
            ds.set_dt_string(dtstr);
        }
        if(millis() - last_time > 1000)
        {
            last_time = millis();
            dt = ds.get_dt();
            UART.printf("========RTC测试======\r\n");
            UART.printf("20%02d-%02d-%02d %02d:%02d:%02d week:%d\r\n", dt.year, dt.month, dt.date, dt.hour, dt.min, dt.sec,dt.week);

            
            uint32_t stamp = get_unix_timestamp(dt);
            UART.printf("UNIX时间戳：%u\r\n",stamp);

            dt = get_utc_dt(dt,8);
            ds.print(UART,dt);
            
            dt1 =  date_next_n_days(dt,30);
            UART.printf("30天之后的日期：20%02d-%02d-%02d\r\n",dt1.year, dt1.month, dt1.date);
            
            dt1 =  date_before_n_days(dt,30);
            UART.printf("30天之前的日期：20%02d-%02d-%02d\r\n",dt1.year, dt1.month, dt1.date);

            String  str;
            Cdt.update_cdt(dt);
            str = Cdt.get_str();
            UART.println(str);
            
            
            str = Cdt.get_jieqi_str(dt);
            UART.println(str);
            
            uint8_t day = Cdt.get_jieqi_mday(dt);
            UART.printf("%d年%d月%d日：%d\r\n",dt.year,dt.month,dt.date,day);

            Cdt.print(UART);
            //            ds.print(UART);
        }
    }


}




