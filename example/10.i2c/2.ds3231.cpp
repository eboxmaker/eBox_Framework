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
	*	1	��������Ҫ����eDriveĿ¼�µ�ds3231����
	*	2	��������ʾ��ds3231ʱ��оƬ�Ļ�������
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"ds3231 example"
#define EXAMPLE_DATE	"2018-08-11"

SoftI2c si2c(&PC6,&PC7);


DS3231 ds(&si2c,0xD0);

DateTime_t dt;
DateTime_t dt1;
char time[9];
char date[9];

uint32_t last_time;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    ds.begin();
    DataU16_t buf[3];
    
    buf[0].byte[0] = 30;
    buf[0].byte[1] = 37;
    buf[1].byte[0] = 20;
    buf[1].byte[1] = 10;
    buf[2].byte[0] = 11;
    buf[2].byte[1] = 19;
    
    dt.year = buf[2].byte[1];
    dt.month = buf[2].byte[0];
    dt.date = buf[1].byte[1];
    dt.hour = buf[1].byte[0];
    dt.min = buf[0].byte[1];
    dt.sec = buf[0].byte[0];
    
//    t.year = 19;
//    t.month = 11;
//    t.date = 10;
//    t.hour = 23;
//    t.min = 59;
//    t.sec = 55;
    ds.set_dt_mb(buf);

}
int main(void)
{
    setup();
    while(1)
    {
        ds.loop();

        dt = ds.get_dt();
        String time = ds.get_time();
        String date = ds.get_date();


//        UART.printf(date);
//        UART.printf(" ");
//        UART.printf(time);
//        UART.printf("\r\n");
        if(millis() - last_time > 1000)
        {
            last_time = millis();
            
            UART.printf("========RTC����======\r\n");
            UART.printf("20%02d-%02d-%02d %02d:%02d:%02d\r\n", dt.year, dt.month, dt.date, dt.hour, dt.min, dt.sec);
            UART.println(date);
            UART.println(time);
            
            uint32_t stamp = ds.get_unix_timestamp(8,ds.dateTime);
            UART.printf("UNIXʱ�����%u\r\n",stamp);

            dt1 =  ds.date_next_n_days(10);
            UART.printf("10��֮������ڣ�20%02d-%02d-%02d\r\n",dt1.year, dt1.month, dt1.date);
//            ds.print(UART);
        }
    }


}




