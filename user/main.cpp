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
#include <Modbus.h>
#include <ModbusIP.h>
#include "../Ethernet3/utility/w5500.h"
#include "../Ethernet3/Ethernet3.h"
#include "../Ethernet3/EthernetUdp3.h"         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

/**
	*	1	此例程需要调用eDrive目录下的w5500模块
	*	2	此例程演示了w5500的初始化，基本信息打印
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"w5500 io test example"
#define EXAMPLE_DATE	"2018-08-11"


//Modbus Registers Offsets (0-9999)
const int LAMP1_COIL = 100; 
//Used Pins
#define led  PB8

//ModbusIP object
ModbusIP mb;

void setup()
{
    ebox_init();
    UART.begin(256000);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
    // The IP address for the shield
    byte ip[] = { 192, 168, 1, 120 };   
    //Config Modbus IP 
    mb.config(mac, ip);
    //Set ledPin mode
    led.mode(OUTPUT);
    // Add LAMP1_COIL register - Use addCoil() for digital outputs
    mb.addCoil(LAMP1_COIL);

    
}
uint32_t last;
int main(void)
{
    setup();

    while(1)
    {
        mb.task();
       
       //Attach ledPin to LAMP1_COIL register     
        led.write(mb.Coil(LAMP1_COIL));
        if(millis() - last > 1000)
        {
            last = milli_seconds;
//            uart1.printf("free:%d\r\n",ebox_get_free());
        }
    }
}
