/**
  ******************************************************************************
  * @file    .cpp
  * @author  shentq
  * @version V1.2
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


/* Includes ------------------------------------------------------------------*/

#include "ebox.h"
#include "bsp_ebox.h"
#include <Ethernet3.h>
#include <Modbus.h>
#include <ModbusIP.h>

/**
  *	1	此例程演示了MODBUS TCP/IP例程
  */

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"MODBUS TCP/IP example"
#define EXAMPLE_DATE	"2018-08-08"

/*
  Modbus-Arduino Example - Lamp (Modbus IP)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/


//Modbus Registers Offsets (0-9999)
const int LAMP1_COIL = 0; 

ModbusIP mb;



void setup()
{
    ebox_init();
//    os_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    LED1.mode(OUTPUT_PP);
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
    // The IP address for the shield
    byte ip[] = { 192, 168, 0, 120 };   
    //Config Modbus IP 
    mb.config(mac, ip);
    //Set ledPin mode
    // Add LAMP1_COIL register - Use addCoil() for digital outputs
    mb.addCoil(LAMP1_COIL);

}

#include "ebox_mem.h"
int main(void)
{
    setup();
    while(1)
    {
        //Call once inside loop() - all magic here
        mb.task();

        //Attach ledPin to LAMP1_COIL register     
        LED1.write( mb.Coil(LAMP1_COIL));
    }


}








