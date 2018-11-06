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

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
uint8_t mac[] =
{
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

// Enter the IP address of the server you're connecting to:
IPAddress server(1, 1, 1, 1);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use  port 10002):
EthernetClient client;


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    // start the Ethernet connection:
    Ethernet.begin(mac, ip);



    // give the Ethernet shield a second to initialize:
    delay_ms(1000);
    Serial.println("connecting...");

    // if you get a connection, report back via serial:
    if (client.connect(server, 10002))
    {
        Serial.println("connected");
    }
    else
    {
        // if you didn't get a connection to the server:
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

        // as long as there are bytes in the serial queue,
        // read them and send them out the socket if it's open:
        while (Serial.available() > 0)
        {
            char inChar = Serial.read();
            if (client.connected())
            {
                client.print(inChar);
            }
        }

        // if the server's disconnected, stop the client:
        if (!client.connected())
        {
            Serial.println();
            Serial.println("disconnecting.");
            client.stop();
            // do nothing:
            while (true);
        }

    }



