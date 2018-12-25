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
#include "ebox_mem.h"

#include "../Ethernet3/utility/w5500.h"
#include "../Ethernet3/Ethernet3.h"
#include "../Ethernet3/EthernetUdp3.h"         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

/**
	*	1	此例程需要调用eDrive目录下的w5500模块
	*	2	此例程演示了w5500的初始化，基本信息打印
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"TCP client example"
#define EXAMPLE_DATE	"2018-08-11"


uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress local_ip(192, 168, 1, 177);

IPAddress server_ip(192,168,1,101);
uint16_t server_port = 6000;


EthernetClient client;


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    Ethernet.begin(mac, local_ip);
    if (client.connect(server_ip, server_port))
    {
        Serial.println("connected");
    }
    else
    {
        Serial.println("connection failed");
    }

}
uint32_t last_time = 0;
int main(void)
{
    setup();

    last_time = millis();
    while(1)
    {
        if (!client.connected())
        {
            client.stop();
            client.connect(server_ip, server_port);
       }
        else
        {
            if (client.available())
            {
                char c = client.read();
                client.print(c);
            }

            if(millis() - last_time > 1000)
            {
                last_time = millis();
                client.println("hello world");
                client.println();
            }
        }
    }


}




