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
uint8_t mac[] =
{
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

unsigned int localPort = 8888;       // local port to listen for UDP packets

char timeServer[] = "ntp1.aliyun.com"; // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

uint8_t packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    // start Ethernet and UDP
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        for (;;)
            ;
    }
    Udp.begin(localPort);

}
unsigned long sendNTPpacket(char *address);
int main(void)
{
    setup();

    while(1)
    {
        sendNTPpacket(timeServer); // send an NTP packet to a time server

        // wait to see if a reply is available
        delay_ms(1000);
        if ( Udp.parsePacket() )
        {
            // We've received a packet, read the data from it
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

            //the timestamp starts at uint8_t 40 of the received packet and is four bytes,
            // or two words, long. First, esxtract the two words:

            unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
            unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
            // combine the four bytes (two words) into a long integer
            // this is NTP time (seconds since Jan 1 1900):
            unsigned long secsSince1900 = highWord << 16 | lowWord;
            Serial.print("Seconds since Jan 1 1900 = " );
            Serial.println(secsSince1900);

            // now convert NTP time into everyday time:
            Serial.print("Unix time = ");
            // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
            const unsigned long seventyYears = 2208988800UL;
            // subtract seventy years:
            unsigned long epoch = secsSince1900 - seventyYears;
            // print Unix time:
            Serial.println(epoch);


            // print the hour, minute and second:
            Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
            Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
            Serial.print(':');
            if ( ((epoch % 3600) / 60) < 10 )
            {
                // In the first 10 minutes of each hour, we'll want a leading '0'
                Serial.print('0');
            }
            Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
            Serial.print(':');
            if ( (epoch % 60) < 10 )
            {
                // In the first 10 seconds of each minute, we'll want a leading '0'
                Serial.print('0');
            }
            Serial.println(epoch % 60); // print the second
        }
        // wait ten seconds before asking for the time again
        delay_ms(10000);
    }

}
// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(char *address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = B11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}



