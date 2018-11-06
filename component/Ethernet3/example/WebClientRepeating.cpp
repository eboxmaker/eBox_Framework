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

W5500Class w5500(&PC13, &PC14, &PC15, &spi2);

// assign a MAC address for the ethernet controller.
// fill in your address here:
uint8_t mac[] =
{
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192, 168, 1, 177);

IPAddress subnet(255, 255, 255, 0);

IPAddress gateway(192, 168, 1, 1);

// fill in your Domain Name Server address here:
IPAddress myDns(1, 1, 1, 1);

// initialize the library instance:
EthernetClient client;

char server[] = "www.arduino.cc";
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers
void httpRequest();

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    // give the ethernet module time to boot up:
    delay_ms(1000);
    // start the Ethernet connection using a fixed IP address and DNS server:
    Ethernet.begin(mac, ip, subnet, gateway, myDns);
    // print the Ethernet board/shield's IP address:
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
}
int main(void)
{
    setup();

    while(1)
    {
        // if there's incoming data from the net connection.
        // send it out the serial port.  This is for debugging
        // purposes only:
        if (client.available())
        {
            char c = client.read();
            Serial.write(c);
        }

        // if ten seconds have passed since your last connection,
        // then connect again and send data:
        if (millis() - lastConnectionTime > postingInterval)
        {
            httpRequest();
        }

    }




}

// this method makes a HTTP connection to the server:
void httpRequest()

{
    // close any connection before send a new request.
    // This will free the socket on the WiFi shield
    client.stop();

    // if there's a successful connection:
    if (client.connect(server, 80))
    {
        Serial.println("connecting...");
        // send the HTTP PUT request:
        client.println("GET /latest.txt HTTP/1.1");
        client.println("Host: www.arduino.cc");
        client.println("User-Agent: arduino-ethernet");
        client.println("Connection: close");
        client.println();

        // note the time that the connection was made:
        lastConnectionTime = millis();
    }
    else
    {
        // if you couldn't make a connection:
        Serial.println("connection failed");
    }

}

