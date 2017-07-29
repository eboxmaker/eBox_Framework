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

void t2it()
{
        adc.read();
        PA0.toggle();
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
int main(void)
{
    setup();
    while(1)
    {
        //串口直接输出，用于前期测试

        
        
        
        //485输出
//        val = adc.read();
//        _485_tx_mode();
//        uart1.printf("val = %x\r\n",val); //如果voltage不对，用示波器看这个值对不对
//        _485_rx_mode();
//        voltage = adc.read_voltage();//电压单位mv
//        _485_tx_mode();
//        uart1.printf("val = %f\r\n",voltage);
//        _485_rx_mode();

//        delay_ms(1000);        
        
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


