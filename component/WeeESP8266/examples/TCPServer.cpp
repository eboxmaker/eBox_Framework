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

    if (wifi.enableMUX())
    {
        uart1.print("single ok\r\n");
    }
    else
    {
        uart1.print("single err\r\n");
    }
    if (wifi.startTCPServer(8090))
    {
        Serial.print("start tcp server ok\r\n");
    }
    else
    {
        Serial.print("start tcp server err\r\n");
    }

    if (wifi.setTCPServerTimeout(10))
    {
        Serial.print("set tcp server timout 10 seconds\r\n");
    }
    else
    {
        Serial.print("set tcp server timout err\r\n");
    }

    uart1.print("setup end\r\n");

}
int main()
{
    setup();
    while(1)
    {
        uint8_t buffer[128] = {0};
        uint8_t mux_id;
        uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
        if (len > 0)
        {
            Serial.print("Status:[");
            Serial.print(wifi.getIPStatus().c_str());
            Serial.println("]");

            Serial.print("Received from :");
            Serial.print(mux_id);
            Serial.print("[");
            for(uint32_t i = 0; i < len; i++)
            {
                Serial.print((char)buffer[i]);
            }
            Serial.print("]\r\n");

            if(wifi.send(mux_id, buffer, len))
            {
                Serial.print("send back ok\r\n");
            }
            else
            {
                Serial.print("send back err\r\n");
            }

            if (wifi.releaseTCP(mux_id))
            {
                Serial.print("release tcp ");
                Serial.print(mux_id);
                Serial.println(" ok");
            }
            else
            {
                Serial.print("release tcp");
                Serial.print(mux_id);
                Serial.println(" err");
            }

            Serial.print("Status:[");
            Serial.print(wifi.getIPStatus().c_str());
            Serial.println("]");
        }
    }
}