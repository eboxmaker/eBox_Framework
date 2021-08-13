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
#define EXAMPLE_DATE	"2021-08-11"

//ChinaCalendar Cdt;
SoftI2c iic(&PA10,&PA11);

DS3231 ds(&iic);

uint32_t last_time;

void setup()
{
    ebox_init();
    UART.begin(115200);
    UART.setTimeout(10);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    
    DateTime dt(__DATE__,__TIME__,8);
    ds.begin(dt);
    

}
int main(void)
{
    setup();
    while(1)
    {
        ds.loop();
        String dtstr = uart1.readString();
        if(dtstr != "")
        {
            DateTime dt(dtstr);
            if(dt.err == 0)
                ds.set(dt);
        }
        if(millis() - last_time > 1000)
        {
            last_time = millis();
            
//            DateTime dt = ds.now();
//            dt.print(UART);
            
//            ds.dateTime.print(UART);
            
                UART.println(ds.dateTime.toString());
            
        }
    }


}








