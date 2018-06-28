/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */



#include "ebox.h"
//#include "ebox_analog.h"

int x;
char a[] = "12";
void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB1.mode(INPUT_PD);    
}

uint8_t last;
uint32_t last1,last2;
int main(void)
{
    setup();
    last = PB1.read();
            uart1.printf("VAL=%D\r\n",PB1.read());
    while(1)
    {
        if(last != PB1.read())
        {
            uart1.printf("dif=%D\r\n",millis() - last1);
            last1 = millis();
            last = PB1.read();
            uart1.printf("VAL=%D\r\n",PB1.read());
        }
       // uart1.printf("-------------NOW=%D\r\n",PB1.read());
       // delay_ms(1);

//    x = analog_read(&PB1);
//   // uart1.printf("hex = %05d\r\n", x);
//    x = analog_read_voltage(&PB1);
//    uart1.printf("val = %04dmv\r\n", x);
   // uart1.printf("==============\r\n", x);
        //delay_ms(1000);
    }
}




