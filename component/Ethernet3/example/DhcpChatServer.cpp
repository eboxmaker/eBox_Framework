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
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
uint8_t mac[] =
{
    0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
IPAddress ip(192, 168, 1, 177);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

// telnet defaults to port 23
EthernetServer server(23);
bool gotAMessage = false; // whether or not you got a message from the client yet

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    // start the Ethernet connection:
    Serial.println("Trying to get an IP address using DHCP");
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // initialize the ethernet device not using DHCP:
        Ethernet.begin(mac, ip, subnet, gateway);
    }
    // print your local IP address:
    Serial.print("My IP address: ");
    ip = Ethernet.localIP();
    for (uint8_t thisByte = 0; thisByte < 4; thisByte++)
    {
        // print the value of each uint8_t of the IP address:
        Serial.print(ip[thisByte], DEC);
        Serial.print(".");
    }
    Serial.println();
    // start listening for clients
    server.begin();
}
int main(void)
{
    setup();

    while(1)
    {
        // wait for a new client:
        EthernetClient client = server.available();

        // when the client sends the first uint8_t, say hello:
        if (client)
        {
            if (!gotAMessage)
            {
                Serial.println("We have a new client");
                client.println("Hello, client!");
                gotAMessage = true;
            }

            // read the bytes incoming from the client:
            char thisChar = client.read();
            // echo the bytes back to the client:
            server.write(thisChar);
            // echo the bytes to the server as well:
            Serial.print(thisChar);
        }
    }

}



