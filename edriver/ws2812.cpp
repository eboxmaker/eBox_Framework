#include "ws2812.h"
#include <stdlib.h>							// For rand() 		function
#include <math.h>							// For floor() 		function
#include "color_convert.h"
#include "font.h"

/* Buffer that holds one complete DMA transmission.
 *
 * The buffer size can be calculated as followas:
 * number of LEDs * 24 bytes + 42 bytes.
 *
 * This leaves us with a maximum string length of
 * (2^16 bytes per DMA stream - 42 bytes)/24 bytes per LED = 2728 LEDs.
 */
uint8_t rgb1[LED_COUNT][3];						//Array that will store color data

uint8_t led_Colors[LED_COUNT];						//Array of integers that will function as indexes for the rgb array
uint8_t ledBuff[LED_BUFFER_SIZE];					//Array of data to be sent to leds.



void WS2812::begin()
{
    pin->mode(AF_PP);

    RCC_Config();
    TIM_Config();
    DMA_Config();
}
void WS2812::reset()
{
    pin->reset();
    delay_us(500);
}
void WS2812::RCC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void WS2812::TIM_Config(void)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
    TIM_OCInitTypeDef  TIM_OCInitStruct;

    uint16_t PrescalerValue = (uint16_t)(72000000 / 24000000) - 1;			// PrescalerValue = 2, Clock is scaled down to 72MHz /(PrescalerValue + 1) = 762MHz/3 = 24MHz

    /* Time base configuration */
    TIM_TimeBaseStruct.TIM_Period = TIM_PERIOD; // 800kHz 				// Species the period value
    TIM_TimeBaseStruct.TIM_Prescaler = PrescalerValue;				// Specifies the prescaler value used to divide the TIM clock.
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;					// Specifies the clock division.
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;			//
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;					// Specifies the TIM mode.
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;				//
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM3, &TIM_OCInitStruct);
    TIM_Cmd(TIM3, ENABLE);

    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);				// This line is required, but im not sure why.

}

void WS2812::DMA_Config(void)
{

    DMA_InitTypeDef DMA_InitStruct;

    DMA_DeInit(DMA1_Channel2);							// Deinitialize DAM1 Channel 1 to their default reset values.

    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&TIM3->CCR3; 			// Specifies Physical address of the peripheral in this case Timer 2 CCR1
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&ledBuff;				// Specifies the buffer memory address
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;					// Data transfered from memory to peripheral
    DMA_InitStruct.DMA_BufferSize = LED_BUFFER_SIZE;				// Specifies the buffer size
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// Do not incrament the peripheral address
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;				// Incrament the buffer index
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	// Specifies the peripheral data width
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		// Specifies the memory data width
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;					// Specifies the operation mode. Normal or Circular
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;				// Specifies the software priority
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;					//

    DMA_Init(DMA1_Channel2, &DMA_InitStruct);					// Initialize DAM1 Channel 1 to values specified in the DMA_InitStruct structure.

    TIM_DMACmd(TIM3, TIM_DMA_CC3, ENABLE);						// Enables TIM2's DMA request. TIM_DMA_CC1 : TIM Capture Compare 1 DMA source

}

void WS2812::send_data(uint8_t *led_Colors, uint16_t len)
{


    uint8_t i, j = 0;
    uint16_t buffersize = (24 * len) + 42, memaddr = 0;


    uint8_t temp, led = 0;

    while(len)
    {
        for (i = 0; i < 3; i++)  						// Set RGB LED color R -> i=0, G -> i=1, B -> i=2
        {
            temp = rgb1[led_Colors[led]][i];
            for (j = 0; j < 8; j++)  					// Set 8 bits of color
            {
                if ((temp) & 0x80)  					// Data sent MSB first, j = 0 is MSB j = 7 is LSB
                {
                    ledBuff[memaddr++] = PWM_HIGH_WIDTH; 		// Compare value for logical 1
                }
                else
                {
                    ledBuff[memaddr++] = PWM_LOW_WIDTH;		// Compare value for logical 0
                }
                temp = temp << 1;
            }
        }

        led++;
        len--;
    }

    // Add needed delay at end of byte cycle, pulsewidth = 0
    while(memaddr < buffersize)
    {
        ledBuff[memaddr++] = 0;
    }

    DMA_SetCurrDataCounter(DMA1_Channel2, LED_BUFFER_SIZE); 	// load number of bytes to be transferred
    DMA_Cmd(DMA1_Channel2, ENABLE); 				// enable DMA channel 1
    TIM_Cmd(TIM3, ENABLE); 						// enable Timer 2
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2)); 			// wait until transfer complete
    TIM_Cmd(TIM3, DISABLE); 					// disable Timer 2
    DMA_Cmd(DMA1_Channel2, DISABLE); 				// disable DMA channel 1
    DMA_ClearFlag(DMA1_FLAG_TC2); 					// clear DMA1 Channel 1 transfer complete flag
}
uint8_t table[] =
{
    '-', '-', '-', 'x', 'x', '-', '-', '-',
    '-', '-', 'x', 'x', 'x', 'x', '-', '-',
    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',
    'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',
    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',
    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',
    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',

};
//显示图形
void WS2812::rainbow_Loop()
{
    COLOR_HSV hsv;
    //COLOR_HSL hsl;
    COLOR_RGB rgb;

    //	u8 intStageNum = 0;
    //	float   r = 255, g = 0, b = 0;
    uint16_t i, k;
    //    hsl.s = 1;
    //    hsl.l = 0.5;

    hsv.s = 1;
    hsv.v = 0.1;
    for(i = 0; i < 360; i++)
    {
        for(int j = 0; j < 64 ; j ++)
        {
            if((table[ j] == 'x') & (1 << k))
            {
                //HSV模式
                hsv.h = (i + j) % 360;
                hsv.s = 1;
                hsv.v = 0.1;
                HSV_to_RGB(hsv, rgb);
                //HSL模式
                //                hsl.h = (i+j)%360;
                //                hsl.s = 1;
                //                hsl.l = 0.5;
                //                HSL_to_RGB(hsl,rgb);

                rgb1[j][0] = rgb.r;// (uint8_t)floor(r/20+j);
                rgb1[j][1] = rgb.g;//(uint8_t)floor(g/20+j);
                rgb1[j][2] = rgb.b;//(uint8_t)floor(b/20+j);

            }
            else
            {
                //HSV模式
                hsv.h = (i + j) % 360;
                hsv.s = 1;
                hsv.v = 0;
                HSV_to_RGB(hsv, rgb);


                //                hsl.h = (i+j)%360;
                //                hsl.s = 1;
                //                hsl.l = 0.5;
                //                HSL_to_RGB(hsl,rgb);


                rgb1[j][0] = rgb.r;// (uint8_t)floor(r/20+j);
                rgb1[j][1] = rgb.g;//(uint8_t)floor(g/20+j);
                rgb1[j][2] = rgb.b;//(uint8_t)floor(b/20+j);

            }

            led_Colors[j] = j;
        }
        // Send data to LEDs
        send_data(led_Colors, LED_COUNT);
        delay_ms(2);
    }
}

//遍历字符串
void WS2812::rainbow_Loop1()
{
    COLOR_HSV hsv;
    //COLOR_HSL hsl;
    COLOR_RGB rgb;
    //	float   r = 255, g = 0, b = 0;
    uint16_t i, k;
    //		hsl.s = 1;
    //		hsl.l = 0.5;

    hsv.s = 1;
    hsv.v = 0.1;
    for(int t = 0; t < 92; t++)
        for(i = 0; i < 64; i++)
        {
            for(int j = 0; j < 8 ; j ++)
                for(k = 0; k < 8; k ++)
                {
                    if(j > 1)
                    {
                        if(font6x8[t][8 - j] & (1 << k))
                        {
                            hsv.h = (i * 4) % 360;
                            hsv.s = 1;
                            hsv.v = 0.1;


                            //								hsl.h = (i + j)%360;
                            //								hsl.s = 1;
                            //								hsl.l = 0.5;

                            HSV_to_RGB(hsv, rgb);
                            //HSL_to_RGB(hsl,rgb);
                            rgb1[j * 8 + k][0] = rgb.r; // (uint8_t)floor(r/20+j);
                            rgb1[j * 8 + k][1] = rgb.g; //(uint8_t)floor(g/20+j);
                            rgb1[j * 8 + k][2] = rgb.b; //(uint8_t)floor(b/20+j);
                        }
                        else
                        {
                            hsv.h = (i * 4) % 360;
                            hsv.s = 1;
                            hsv.v = 0;


                            //								hsl.h = (i + j)%360;
                            //								hsl.s = 1;
                            //								hsl.l = 0.5;

                            HSV_to_RGB(hsv, rgb);
                            //HSL_to_RGB(hsl,rgb);

                            rgb1[j * 8 + k][0] = rgb.r; // (uint8_t)floor(r/20+j);
                            rgb1[j * 8 + k][1] = rgb.g; //(uint8_t)floor(g/20+j);
                            rgb1[j * 8 + k][2] = rgb.b; //(uint8_t)floor(b/20+j);

                        }
                    }
                    else
                    {
                        hsv.h = 240;
                        hsv.s = 1;
                        hsv.v = 0;


                        //								hsl.h = (i + j)%360;
                        //								hsl.s = 1;
                        //								hsl.l = 0.5;

                        HSV_to_RGB(hsv, rgb);
                        //	HSL_to_RGB(hsl,rgb);

                        rgb1[j * 8 + k][0] = rgb.g; // (uint8_t)floor(r/20+j);
                        rgb1[j * 8 + k][1] = rgb.r; //(uint8_t)floor(g/20+j);
                        rgb1[j * 8 + k][2] = rgb.b; //(uint8_t)floor(b/20+j);

                    }
                    int l, h, l_offset, h_offset;
                    if(i > 8)
                        l_offset = i * 8 / 64;
                    h_offset = 0;

                    if(l_offset > 0)
                    {
                        l = j + l_offset;
                        l %= 8 ;
                    }
                    else
                    {
                        if(j + l_offset >= 0)
                            l = j + l_offset;
                        else
                            l = 8 + j + l_offset;
                    }

                    if(h_offset > 0)
                    {
                        h = k + h_offset;
                        h %= 8 ;
                    }
                    else
                    {
                        if(k + h_offset >= 0)
                            h = k + h_offset;
                        else
                            h =  k + h_offset + 8;
                    }

                    led_Colors[j * 8 + k] = l * 8 + h;

                }

            // Send data to LEDs
            send_data(led_Colors, LED_COUNT);
            delay_ms(5);
        }
}
