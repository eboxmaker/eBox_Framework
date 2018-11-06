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
#define HOST_PORT   (8090)

ESP8266 wifi(uart2, PA4);

void setup()
{
    ebox_init();

    wifi.begin(115200);
    uart1.begin(115200);

    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    uart1.print("setup begin\r\n");

    uart1.print("FW Version:");
    uart1.flush();
    //    uint8_t *x;
    //
    //    *x = 123;

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
    uart1.println(wifi.getVersion());


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

    if (wifi.createTCP(HOST_NAME, HOST_PORT))
    {
        uart1.print("create tcp ok\r\n");
    }
    else
    {
        uart1.print("create tcp err\r\n");
    }
    uart1.print("setup end\r\n");

}
int main()
{
    setup();
    while(1)
    {
        uint8_t buffer[128] = {0};



        char *hello = "Hello, this is client!";
        wifi.send((const uint8_t *)hello, strlen(hello));

        uint32_t len = wifi.recv(buffer, sizeof(buffer), 3);
        if (len > 0)
        {
            uart1.print("Received:[");
            for(uint32_t i = 0; i < len; i++)
            {
                uart1.print((char)buffer[i]);
            }
            uart1.print("]\r\n");
        }

        //        if (wifi.releaseTCP()) {
        //            uart1.print("release tcp ok\r\n");
        //        } else {
        //            uart1.print("release tcp err\r\n");
        //        }
        //        delay_ms(1000);
    }

}