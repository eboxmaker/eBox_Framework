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

//W5500Class w5500(&PC13, &PC14, &PC15, &spi2);

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(192.,168,1,4);  // numeric IP for Google (no DNS)
char server[] = "www.baidu.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, ip);
    }
    // give the Ethernet shield a second to initialize:
    delay_ms(1000);
    Serial.println("connecting...");

    // if you get a connection, report back via serial:
    if (client.connect(server, 80))
    {
        Serial.println("connected");
        // Make a HTTP request:
        client.println("GET /search?q=arduino HTTP/1.1");
        client.println("Host: www.baidu.com");
        client.println("Connection: close");
        client.println();
    }
    else
    {
        // kf you didn't get a connection to the server:
        Serial.println("connection failed");
    }

}
int main(void)
{
    setup();

    while(1)
    {
        // if there are incoming bytes available
        // from the server, read them and print them:
        if (client.available())
        {
            char c = client.read();
            Serial.print(c);
        }

        // if the server's disconnected, stop the client:
        if (!client.connected())
        {
            Serial.println();
            Serial.println("disconnecting.");
            client.stop();

            // do nothing forevermore:
            while (true);
        }
    }


}




