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

#include "w25x16.h"



/**
	*	1	此例程需要调用eDrive目录下的w25x16驱动
	*	2	此例程演示了w25x16的读写功能
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"w25x16 example"
#define EXAMPLE_DATE	"2018-08-11"


W25x16 flash(&PA15, &spi1);

void setup()
{
    ebox_init();
    UART.begin(256000);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    flash.begin();


}
int16_t tmp[7];
uint16_t id;
uint8_t rbuf[4096];
uint8_t wbuf[4096];
uint16_t sec_counter = 0;
uint16_t flag = 0;
int main(void)
{
    setup();
    flash.read_id();
    uart1.printf("\r\n==readid=======\r\n");
    uart1.printf("id = %x", flash.get_type());

    while(1)
    {
        for(int i = 0; i < 4096; i++)
            wbuf[i] = random() % 256;

//        uart1.printf("\r\n==write========\r\n");
//        flash.write_sector(wbuf, 0, 100);
//        for(int i = 0; i < 100; i++)
//            uart1.printf(" %x", wbuf[i]);

//        uart1.printf("\r\n==read========\r\n");
//        flash.read_sector(rbuf, 63, 0);
//        for(int i = 0; i < 4096; i++)
//            uart1.printf(" 0X%02X", rbuf[i]);

//        uart1.printf("\r\n\r\n");

        
        flash.erase_sector(sec_counter);
        flash.write_sector(wbuf, sec_counter, 1);
        flash.read_sector(rbuf, sec_counter, 1);
        for(int i = 0; i < 4096; i++)
        {
            if(wbuf[i] != rbuf[i])
            {
                flag++;
                uart1.printf("[err] sec:%d  i:%d  ",sec_counter,i);
                uart1.printf("write: %d", wbuf[i]);
                uart1.printf("read: %d\r\n", wbuf[i]);
                
            }
        }
        uart1.printf("sec:%d  flag: %d\r\n", sec_counter,flag);
        flag = 0;
        
        sec_counter++;
        if(sec_counter >= flash.get_sector_count())
            while(1);
//        delay_ms(1000);
    }
}




