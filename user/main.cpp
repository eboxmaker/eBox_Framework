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
#include "ioevent.h"
#include "ebox_mem.h"
uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
String macString;
char serverAddress[] = "192.168.3.172"; // server address
int port = 8080;
EthernetClient eClient;
HttpClient client = HttpClient(eClient, serverAddress, port);
String contentType = "application/json";
String postData = "mac=123";
uint32_t last = 0;
IoChang btn;
IoNode  io[] = 
{
	{&BTN1,"btn0"},
	{&PB3,"btn1"},

};
LinkedList<String *> list;
Timer timer1(TIM1);
void t_event()
{
        btn.loop();

}
void event(char *msg)
{
    String *str = new String();
    *str = msg;
    list.add(str);
}

void setup()
{
    ebox_init();
    Serial.begin(115200);
    LED1.mode(OUTPUT_PP);
    LED2.mode(OUTPUT_PP);
    print_log();
    
    Ethernet.begin(mac);

    
    
    Serial.printf("started..............\n");

    for(int i = 0; i < getArraySize(io);i++)
    {
        btn.add(&io[i]);
    }
    btn.attach(event);
    timer1.begin(100);
    timer1.attach(t_event);
    timer1.interrupt(ENABLE);
    timer1.start();
}
int main(void)
{
    setup();

    while(1)
    {
        if(list.size() > 0)
        {
            uart1.printf("free:%d\n",ebox_get_free());

            client.post("/", contentType, list[0]->c_str());
            int statusCode = client.responseStatusCode();
            Serial.print("Status code: ");
            Serial.println(statusCode);
            if (statusCode == 200)
            {
                delete list[0];
                list.remove(0);
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


