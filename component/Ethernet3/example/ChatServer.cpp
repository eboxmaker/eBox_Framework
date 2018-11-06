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
byte mac[] =
{
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);


// telnet defaults to port 23
EthernetServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    // initialize the ethernet device
    Ethernet.begin(mac, ip, subnet, gateway);
    // start listening for clients
    server.begin();


    Serial.print("Chat server address:");
    Serial.println(Ethernet.localIP());

}
int main(void)
{
    setup();

    while(1)
    {
        // wait for a new client:
        EthernetClient client = server.available();

        // when the client sends the first byte, say hello:
        if (client)
        {
            if (!alreadyConnected)
            {
                // clead out the input buffer:
                client.flush();
                Serial.println("We have a new client");
                client.println("Hello, client!");
                alreadyConnected = true;
            }

            if (client.available() > 0)
            {
                // read the bytes incoming from the client:
                char thisChar = client.read();
                // echo the bytes back to the client:
                server.write(thisChar);
                // echo the bytes to the server as well:
                Serial.write(thisChar);
            }
        }
    }

}



