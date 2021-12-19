/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-7-31	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "bsp_ebox.h"
#include "Ethernet3.h"
#include "EthernetClient.h"
#include "HttpClient.h"

uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
String macString;
char serverAddress[] = "39.97.216.195";  // server address
uint16_t port = 6009;
//char serverAddress[] = "192.168.3.163"; // server address
//int port = 8080;
EthernetClient eClient;
HttpClient client = HttpClient(eClient, serverAddress, port);
String contentType = "application/x-www-form-urlencoded";
String postData = "mac=123";
Exti   userbt1(&BTN1);
class MsgIOCheck
{
    public :
        MsgIOCheck(String _name,String _state)
        {
            name = _name;
            state = _state;
        }
        String name;
        String state;
};
LinkedList<MsgIOCheck *> list;
uint32_t last = 0;
void exti_event()
{
    MsgIOCheck *msg;
    LED1.toggle();
    bool newValue = userbt1.read();
    if(newValue)
    {
        msg = new MsgIOCheck("btn1", "1");
        UART.printf("\r\n rise ");
    }
    else
    {
        msg = new MsgIOCheck("btn1", "0");
        UART.printf("\r\n fall");
    }
    list.add(msg);
}

void setup()
{
    ebox_init();
    Serial.begin(115200);
    LED1.mode(OUTPUT_PP);
    print_log();
    
    Ethernet.begin(mac);
    char buf[32];
    memset(buf, 0, 32);
    snprintf(buf,32,"%02X %02X %02X %02X %02X %02X",cpu.chip_id[0],cpu.chip_id[1],cpu.chip_id[2],cpu.chip_id[3],cpu.chip_id[4],cpu.chip_id[5]);
    macString = buf;

    userbt1.begin();
    userbt1.attach(exti_event);
    userbt1.interrupt(Exti::TrigFallRise,ENABLE);
    
    Serial.printf("started..............\n");

}
int main(void)
{
    setup();

    while(1)
    {
        if(list.size() > 0)
        {
            MsgIOCheck *msg = list[0];
		    postData = "mac=" + macString + "&name=" + msg->name + "&state=" + msg->state;
            client.post("/", contentType, postData);
            int statusCode = client.responseStatusCode();
            Serial.print("Status code: ");
            Serial.println(statusCode);
            if (statusCode == 200)
            {
                list.remove(0);
                delete msg;
                String response = client.responseBody();
                Serial.print("Response: ");
                Serial.println(response);
            }
            client.stop();
        }

        if(millis() - last >500)
        {
            last = millis();
            LED1.toggle();
        }

    }
}


