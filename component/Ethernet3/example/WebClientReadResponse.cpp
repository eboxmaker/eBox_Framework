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

const int led = 6;
uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char serverName[] = "www.arduino.php5.sk"; // webserver
IPAddress ip(192, 168, 1, 254); //for instance in 192.168.1.0 /24 network
EthernetClient client;
String readString;        //our variable
int x = 0; //counter of line
char lf = 10; //line feed character



void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    led1.begin();
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        Ethernet.begin(mac, ip);
    }

}
int main(void)
{
    setup();

    while(1)
    {
        if (client.connect(serverName, 80))    //starts client connection, checks for connection
        {
            Serial.println("connected");
            client.println("GET rele/rele1.txt HTTP/1.1"); //download text
            client.println("Host: www.arduino.php5.sk");
            client.println("Connection: close");  //close 1.1 persistent connection
            client.println(); //end of get request
        }
        else
        {
            Serial.println("Pripojenie neuspesne"); //error if i am not connected
            Serial.println();
        }

        while(client.connected() && !client.available()) delay_ms(1); //wait for datas
        while (client.connected() || client.available())   //check response
        {
            char c = client.read(); //get bits from buffer
            Serial.print(c); //full http header
            if (c == lf) x = (x + 1); //pocitaj
            else if (x == 12) readString += c; //our variable
        }
        Serial.print("Variable: ");
        Serial.print(readString); //our variable parsed like string
        if(readString == "ZAP")
        {
            led1.on();
        }
        else if(readString == "VYP")
        {
            led1.off();
        }
        else
        {
            Serial.println("Unsupported variable");
        }
        readString = ("");          //delete our variable
        x = 0;                       //reset counter
        client.stop(); //end connection
        delay_ms(5000); //wait 5 seconds and start again
    }

}
