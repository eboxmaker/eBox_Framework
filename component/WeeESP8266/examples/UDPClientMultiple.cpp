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
#define HOST_NAME   "192.168.0.102"
#define HOST_PORT   (1234)

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

    uart1.print("setup end\r\n");

}
int main()
{
    setup();
    while(1)
    {
        uint8_t buffer[128] = {0};
        static uint8_t mux_id = 0;

        if (wifi.registerUDP(mux_id, HOST_NAME, HOST_PORT))
        {
            Serial.print("register udp ");
            Serial.print(mux_id);
            Serial.println(" ok");
        }
        else
        {
            Serial.print("register udp ");
            Serial.print(mux_id);
            Serial.println(" err");
        }

        char *hello = "Hello, this is client!";
        wifi.send(mux_id, (const uint8_t *)hello, strlen(hello));

        uint32_t len = wifi.recv(mux_id, buffer, sizeof(buffer), 10000);
        if (len > 0)
        {
            Serial.print("Received:[");
            for(uint32_t i = 0; i < len; i++)
            {
                Serial.print((char)buffer[i]);
            }
            Serial.print("]\r\n");
        }

        if (wifi.unregisterUDP(mux_id))
        {
            Serial.print("unregister udp ");
            Serial.print(mux_id);
            Serial.println(" ok");
        }
        else
        {
            Serial.print("unregister udp ");
            Serial.print(mux_id);
            Serial.println(" err");
        }
        delay_ms(5000);
        mux_id++;
        if (mux_id >= 5)
        {
            mux_id = 0;
        }

    }
}