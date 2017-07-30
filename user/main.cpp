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
#include "ads7816.h"

Timer timer2(TIM2);

Ads7816 adc(&PB3,&PB7,&PB4);//PB4是jtag引脚。不要用20pin的jlink去调试程序
uint16_t val;
float voltage;

void _485_tx_mode();
void _485_rx_mode();
void adc_read_ch(uint8_t ch);

uint8_t adc_ch;
float   val_ch1;
float   val_ch2;
float   val_ch3;
float   val_ch4;
uint8_t ch1_updata_flag;
uint8_t ch2_updata_flag;
uint8_t ch3_updata_flag;
uint8_t ch4_updata_flag;

void t2it()
{
    adc_ch++;
    if(adc_ch >= 4)
        adc_ch = 0;
    adc_read_ch(adc_ch);
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    adc.begin(); 
    
    timer2.begin(1);
    timer2.attach(t2it);
    timer2.interrupt(ENABLE);
    timer2.start();

    //485
    PA11.mode(OUTPUT_PP);
    PA4.mode(OUTPUT_PP);
    
    PA0.mode(OUTPUT_PP);
}
uint64_t last_updata_time;
int main(void)
{
    setup();
    last_updata_time = millis();
    while(1)
    {
        if(millis() - last_updata_time >= 500)
        {
            last_updata_time = millis();            
            _485_tx_mode();
            uart1.printf("%4.2f,%4.2f,%4.2f,%4.2f",val_ch1,val_ch2,val_ch3,val_ch4);
            _485_rx_mode();
        }
    }

}

//485输出
void _485_tx_mode()//如果不输出，将引脚设置翻转
{
    PA11.set();
}
void _485_rx_mode()
{
    PA11.reset();
}

void adc_read_ch(uint8_t ch)
{
    switch(ch)
    {
        case 0:
            val_ch1 = adc.read_voltage();
            ch2_updata_flag = 1;
            break;
        case 1:
            val_ch2 = adc.read_voltage();
            break;
        default:
            break;
    }
    
    
}

