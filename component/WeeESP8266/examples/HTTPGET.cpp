/*
  Modbus-Arduino Example - Lamp (Modbus uart1)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/


#include "bsp_ebox.h"
#include "ESP8266.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Analog_Write example"
#define EXAMPLE_DATE	"2018-08-05"



#define SSID        "DVmaster"
#define PASSWORD    "3302389nsfq"
#define HOST_NAME   "www.baidu.com"
#define HOST_PORT   (80)

ESP8266 wifi;

void setup()
{
    ebox_init();


    wifi.begin(&uart2, PA4, 115200);
    uart1.begin(115200);

    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    uart1.print("setup begin\r\n");
    uart1.print("FW Version:");


    //    while(1)
    //    {
    //        String x;
    //        x = uart1.readStringUntil(':');
    //        if(x != NULL)
    //            uart1.println(x);
    //        delay_ms(1);
    //
    //    }
    //

    String str = wifi.getVersion();
    uart1.println(str);


    if (wifi.setOprToStationSoftAP())
    {
        uart1.print("to station + softap ok\r\n");
    }
    else
    {
        uart1.print("to station + softap err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD))
    {
        uart1.print("Join AP success\r\n");
        uart1.print("IP:");
        uart1.println( wifi.getLocalIP().c_str());
    }
    else
    {
        uart1.print("Join AP failure\r\n");
    }

    if (wifi.disableMUX())
    {
        uart1.print("single ok\r\n");
    }
    else
    {
        uart1.print("single err\r\n");
    }

    uart1.print("setup end\r\n");

}
int main()
{
    setup();
    while(1)
    {
        uint8_t buffer[1024] = {0};

        if (wifi.createTCP(HOST_NAME, HOST_PORT))
        {
            Serial.print("create tcp ok\r\n");
        }
        else
        {
            Serial.print("create tcp err\r\n");
        }

        char *hello = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n";
        wifi.send((const uint8_t *)hello, strlen(hello));

        uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
        if (len > 0)
        {
            Serial.print("Received:[");
            for(uint32_t i = 0; i < len; i++)
            {
                Serial.print((char)buffer[i]);
            }
            Serial.print("]\r\n");
        }

        if (wifi.releaseTCP())
        {
            Serial.print("release tcp ok\r\n");
        }
        else
        {
            Serial.print("release tcp err\r\n");
        }

        while(1);
    }

}