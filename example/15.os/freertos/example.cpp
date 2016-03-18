/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/
#include "freertos.h"
#include "task.h"
#include "queue.h"

//STM32 RUN IN eBox
#include "ebox.h"
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3

void led_on(int n)
{
    switch (n)
    {
    case LED_0:
        PB8 = 1;
        break;
    case LED_1:
        PB9 = 1;
        break;
    case LED_2:
        PB10 = 1;
        break;
    default:
        break;
    }
}

void led_off(int n)
{
    switch (n)
    {
    case LED_0:
        PB8 = 0;
        break;
    case LED_1:
        PB9 = 0;
        break;
    case LED_2:
        PB10 = 0;
        break;
    default:
        break;
    }
}


static void vLEDTask( void *pvParameters )
{
    while(1)
    {
        led_on((int)pvParameters);
        vTaskDelay(500 / portTICK_RATE_MS);
        led_off((int)pvParameters);
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}

void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    PB10.mode(OUTPUT_PP);

    set_systick_user_event_per_sec(configTICK_RATE_HZ);
    attch_systick_user_event(xPortSysTickHandler);

    xTaskCreate( vLEDTask,  "LED0", configMINIMAL_STACK_SIZE, (void *)0, tskIDLE_PRIORITY + 3, NULL );
    xTaskCreate( vLEDTask, "LED1", configMINIMAL_STACK_SIZE, (void *)1, tskIDLE_PRIORITY + 3, NULL );
    xTaskCreate( vLEDTask, "LED2", configMINIMAL_STACK_SIZE, (void *)2, tskIDLE_PRIORITY + 3, NULL );

    /* Start the scheduler. */
    vTaskStartScheduler();

}
int main(void)
{
    setup();
    while(1)
    {
    }

    return 0;

}


