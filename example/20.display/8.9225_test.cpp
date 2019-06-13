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
#include "bsp_ebox.h"
#include "parallel_gpio.h"
#include "tft9325.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

//Port db(GPIOB_BASE,8);		// PB0-PB7
//Lcd tLCD(&PA12,&PB9, &PA15, &PF6, &PF7, &db);

void setup()
{
    ebox_init();
		
		UART.begin(115200);
		print_log(EXAMPLE_NAME, EXAMPLE_DATE);
		
		PF0.mode(OUTPUT_PP);
		PF0.set();
    lcd.begin();
    lcd.clear(WRITE);
//		lcd.off();
//		lcd.draw_pixel(10,10,BLACK);
//		lcd.on();


    lcd.draw_v_line(0, 4, 128, RED);
		lcd.draw_h_line(8, 10, 128, BLUE);
    lcd.draw_circle(50, 50, 40, YELLOW);
    lcd.draw_line(64, 5, 64, 128, GREEN);


}
int main(void)
{
    setup();
    while(1)
    {

    }
}


