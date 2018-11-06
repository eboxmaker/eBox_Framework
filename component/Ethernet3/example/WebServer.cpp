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

u8 buf[100];

uint8_t mac[] =
{
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

EthernetServer server(80);

unsigned int localPort = 8888;      // local port to listen on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
uint8_t  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    // start the Ethernet connection and the server:
    Ethernet.begin(mac, ip);
    server.begin();
    UART.print("server is at ");
    UART.println(Ethernet.localIP());


}
int main(void)
{
    setup();

    while(1)
    {
        // listen for incoming clients
        EthernetClient client = server.available();
        if (client)
        {
            UART.println("new client");
            // an http request ends with a blank line
            bool currentLineIsBlank = true;
            while (client.connected())
            {
                if (client.available())
                {
                    char c = client.read();
                    UART.write(c);
                    // if you've gotten to the end of the line (received a newline
                    // character) and the line is blank, the http request has ended,
                    // so you can send a reply
                    if (c == '\n' && currentLineIsBlank)
                    {
                        // send a standard http response header
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close");  // the connection will be closed after completion of the response
                        client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println();
                        client.println("<!DOCTYPE HTML>");
                        client.println("<html>");
                        // output the value of each analog input pin
                        for (int analogChannel = 0; analogChannel < 6; analogChannel++)
                        {
                            int sensorReading = millis(); // analog_read(analogChannel);
                            client.print("analog input ");
                            client.print(analogChannel);
                            client.print(" is ");
                            client.print(sensorReading);
                            client.println("<br />");
                        }
                        client.println("</html>");
                        break;
                    }
                    if (c == '\n')
                    {
                        // you're starting a new line
                        currentLineIsBlank = true;
                    }
                    else if (c != '\r')
                    {
                        // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
            // give the web browser time to receive the data
            delay_ms(1);
            // close the connection:
            client.stop();
            UART.println("client disconnected");
        }
    }


}




