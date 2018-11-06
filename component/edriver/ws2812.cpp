#include "font.h"
#include "rcc.h"
#include "ebox_pwm.h"
#include "color_convert.h"

#include "ws2812.h"
/* Buffer that holds one complete DMA transmission.
 *
 * The buffer size can be calculated as followas:
 * number of LEDs * 24 bytes + 42 bytes.
 *
 * This leaves us with a maximum string length of
 * (2^16 bytes per DMA stream - 42 bytes)/24 bytes per LED = 2728 LEDs.
 */
//uint8_t rgb1[LED_COUNT][3];						//Array that will store color data

uint8_t ledBuff[LED_BUFFER_SIZE];					//Array of data to be sent to leds.


void WS2812::begin()
{
    init_info(pin);
    pin->mode(AF_PP);


    switch(ch)
    {
    case TIMxCH1:
        dam_cc = TIM_DMA_CC1;
        dma_target = (uint32_t)&TIMx->CCR1;
        break;
    case TIMxCH2:
        dam_cc = TIM_DMA_CC2;
        dma_target = (uint32_t)&TIMx->CCR2;
        break;
    case TIMxCH3:
        dam_cc = TIM_DMA_CC3;
        dma_target = (uint32_t)&TIMx->CCR3;
        break;
    case TIMxCH4:
        dam_cc = TIM_DMA_CC4;
        dma_target = (uint32_t)&TIMx->CCR4;
        break;
    }

    TIM_Config();
    DMA_Config();
}

void WS2812::reset()
{
    pin->reset();
    delay_us(500);
}
//显示缓冲区
void WS2812::display(uint8_t *ptr, uint16_t height, uint16_t width)
{

    uint8_t i, j = 0;
    uint16_t buffersize = (24 * height * width), memaddr = 0;


    uint8_t temp;

    for(int index = 0; index < height * width; index++)
    {
        for (i = 0; i < 3; i++)  						// Set RGB LED color R -> i=0, G -> i=1, B -> i=2
        {
            temp = ptr[index * 3 + i];
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
    }

    // Add needed delay at end of byte cycle, pulsewidth = 0
    while(memaddr < buffersize)
    {
        ledBuff[memaddr++] = 0;
    }

    dma->set_current_len(LED_BUFFER_SIZE);
    dma->enable();
    TIM_Cmd(TIMx, ENABLE); 						// enable Timer 2
    while(!dma->get_flag_status()); 			// wait until transfer complete
    TIM_Cmd(TIMx, DISABLE); 					// disable Timer 2
    dma->disable(); 				// disable DMA channel 1
    dma->clear_flag();
}

//static uint8_t rain_table[] =
//{
//    '-', '-', '-', 'x', 'x', '-', '-', '-',
//    '-', '-', 'x', 'x', 'x', 'x', '-', '-',
//    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',
//    'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
//    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',
//    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',
//    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',
//    '-', 'x', 'x', 'x', 'x', 'x', 'x', '-',

//};
//显示图形
//void WS2812::rainbow_Loop()
//{
//    COLOR_HSV hsv;
//    //COLOR_HSL hsl;
//    COLOR_RGB rgb;

//    //	u8 intStageNum = 0;
//    //	float   r = 255, g = 0, b = 0;
//    uint16_t i, k;
//    //    hsl.s = 1;
//    //    hsl.l = 0.5;

//    hsv.s = 1;
//    hsv.v = 0.1;
//    for(i = 0; i < 360; i++)
//    {
//        for(int j = 0; j < 64 ; j ++)
//        {
//            if(rain_table[ j] == 'x')
//            {
//                //HSV模式
//                hsv.h = (i + j) % 360;
//                hsv.s = 1;
//                hsv.v = 0.1;
//                HSV_to_RGB(hsv, rgb);
//                //HSL模式
//                //                hsl.h = (i+j)%360;
//                //                hsl.s = 1;
//                //                hsl.l = 0.5;
//                //                HSL_to_RGB(hsl,rgb);

//                rgb1[j][0] = rgb.r;// (uint8_t)floor(r/20+j);
//                rgb1[j][1] = rgb.g;//(uint8_t)floor(g/20+j);
//                rgb1[j][2] = rgb.b;//(uint8_t)floor(b/20+j);

//            }
//            else
//            {
//                //HSV模式
//                hsv.h = (i + j) % 360;
//                hsv.s = 1;
//                hsv.v = 0;
//                HSV_to_RGB(hsv, rgb);


//                //                hsl.h = (i+j)%360;
//                //                hsl.s = 1;
//                //                hsl.l = 0.5;
//                //                HSL_to_RGB(hsl,rgb);


//                rgb1[j][0] = rgb.r;// (uint8_t)floor(r/20+j);
//                rgb1[j][1] = rgb.g;//(uint8_t)floor(g/20+j);
//                rgb1[j][2] = rgb.b;//(uint8_t)floor(b/20+j);

//            }
//        }
//        send_data( LED_COUNT);
//        delay_ms(2);
//    }
//}

//遍历字符串
//void WS2812::rainbow_Loop1()
//{
//    COLOR_HSV hsv;
//    //COLOR_HSL hsl;
//    COLOR_RGB rgb;
//    //	float   r = 255, g = 0, b = 0;
//    uint16_t i, k;
//    //		hsl.s = 1;
//    //		hsl.l = 0.5;

//    hsv.s = 1;
//    hsv.v = 0.1;
//    for(int t = 0; t < 92; t++)
//        for(i = 0; i < 64; i++)
//        {
//            for(int j = 0; j < 8 ; j ++)
//                for(k = 0; k < 8; k ++)
//                {
//                    if(j > 1)
//                    {
//                        if(font6x8[t][8 - j] & (1 << k))
//                        {
//                            hsv.h = (i * 4) % 360;
//                            hsv.s = 1;
//                            hsv.v = 0.1;


//                            //								hsl.h = (i + j)%360;
//                            //								hsl.s = 1;
//                            //								hsl.l = 0.5;

//                            HSV_to_RGB(hsv, rgb);
//                            //HSL_to_RGB(hsl,rgb);
//                            rgb1[j * 8 + k][0] = rgb.r; // (uint8_t)floor(r/20+j);
//                            rgb1[j * 8 + k][1] = rgb.g; //(uint8_t)floor(g/20+j);
//                            rgb1[j * 8 + k][2] = rgb.b; //(uint8_t)floor(b/20+j);
//                        }
//                        else
//                        {
//                            hsv.h = (i * 4) % 360;
//                            hsv.s = 1;
//                            hsv.v = 0;


//                            //								hsl.h = (i + j)%360;
//                            //								hsl.s = 1;
//                            //								hsl.l = 0.5;

//                            HSV_to_RGB(hsv, rgb);
//                            //HSL_to_RGB(hsl,rgb);

//                            rgb1[j * 8 + k][0] = rgb.r; // (uint8_t)floor(r/20+j);
//                            rgb1[j * 8 + k][1] = rgb.g; //(uint8_t)floor(g/20+j);
//                            rgb1[j * 8 + k][2] = rgb.b; //(uint8_t)floor(b/20+j);

//                        }
//                    }
//                    else
//                    {
//                        hsv.h = 240;
//                        hsv.s = 1;
//                        hsv.v = 0;


//                        //								hsl.h = (i + j)%360;
//                        //								hsl.s = 1;
//                        //								hsl.l = 0.5;

//                        HSV_to_RGB(hsv, rgb);
//                        //	HSL_to_RGB(hsl,rgb);

//                        rgb1[j * 8 + k][0] = rgb.g; // (uint8_t)floor(r/20+j);
//                        rgb1[j * 8 + k][1] = rgb.r; //(uint8_t)floor(g/20+j);
//                        rgb1[j * 8 + k][2] = rgb.b; //(uint8_t)floor(b/20+j);

//                    }
//                    int l, h, l_offset, h_offset;
//                    if(i > 8)
//                        l_offset = i * 8 / 64;
//                    h_offset = 0;

//                    if(l_offset > 0)
//                    {
//                        l = j + l_offset;
//                        l %= 8 ;
//                    }
//                    else
//                    {
//                        if(j + l_offset >= 0)
//                            l = j + l_offset;
//                        else
//                            l = 8 + j + l_offset;
//                    }

//                    if(h_offset > 0)
//                    {
//                        h = k + h_offset;
//                        h %= 8 ;
//                    }
//                    else
//                    {
//                        if(k + h_offset >= 0)
//                            h = k + h_offset;
//                        else
//                            h =  k + h_offset + 8;
//                    }


//                }

//            // Send data to LEDs
//            send_data(LED_COUNT);
//            delay_ms(5);
//        }
//}




//void WS2812::send_data(uint16_t len)
//{
//    uint8_t i, j = 0;
//    uint16_t buffersize = (24 * len) + 42, memaddr = 0;


//    uint8_t temp, led = 0;

//    for(int ln =0; ln < len;ln++)
//    {
//        for (i = 0; i < 3; i++)  						// Set RGB LED color R -> i=0, G -> i=1, B -> i=2
//        {
//            temp = rgb1[ln][i];
//            for (j = 0; j < 8; j++)  					// Set 8 bits of color
//            {
//                if ((temp) & 0x80)  					// Data sent MSB first, j = 0 is MSB j = 7 is LSB
//                {
//                    ledBuff[memaddr++] = PWM_HIGH_WIDTH; 		// Compare value for logical 1
//                }
//                else
//                {
//                    ledBuff[memaddr++] = PWM_LOW_WIDTH;		// Compare value for logical 0
//                }
//                temp = temp << 1;
//            }
//        }
//    }

//    // Add needed delay at end of byte cycle, pulsewidth = 0
//    while(memaddr < buffersize)
//    {
//        ledBuff[memaddr++] = 0;
//    }

//    dma->set_current_len(LED_BUFFER_SIZE);
//    dma->enable();
//    TIM_Cmd(TIMx, ENABLE); 						// enable Timer 2
//    while(!dma->get_flag_status()); 			// wait until transfer complete
//    TIM_Cmd(TIMx, DISABLE); 					// disable Timer 2
//    dma->disable(); 				// disable DMA channel 1
//    dma->clear_flag();

//}

void WS2812::TIM_Config(void)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
    TIM_OCInitTypeDef  TIM_OCInitStruct;

    rcc_clock_cmd((uint32_t)TIMx, ENABLE);


    if(TIMx == TIM1 ||  TIMx == TIM8 )
    {
        TIM_CtrlPWMOutputs(TIMx, ENABLE);
    }

    uint16_t PrescalerValue = (uint16_t)0;

    /* Time base configuration */
    TIM_TimeBaseStruct.TIM_Period = TIM_PERIOD; // 超频到1Mhz 				// Species the period value
    TIM_TimeBaseStruct.TIM_Prescaler = PrescalerValue;				// Specifies the prescaler value used to divide the TIM clock.
    TIM_TimeBaseStruct.TIM_ClockDivision = 0;					// Specifies the clock division.
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;			//
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStruct);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;					// Specifies the TIM mode.
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;				//
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_Pulse = 0;
    switch(ch)
    {
    case TIMxCH1:
        TIM_OC1Init(TIMx, &TIM_OCInitStruct);
        TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);				// This line is required, but im not sure why.
        break;
    case TIMxCH2:
        TIM_OC2Init(TIMx, &TIM_OCInitStruct);
        TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);				// This line is required, but im not sure why.
        break;
    case TIMxCH3:
        TIM_OC3Init(TIMx, &TIM_OCInitStruct);
        TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);				// This line is required, but im not sure why.
        break;
    case TIMxCH4:
        TIM_OC4Init(TIMx, &TIM_OCInitStruct);
        TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);				// This line is required, but im not sure why.
        break;
    }
    TIM_Cmd(TIMx, ENABLE);


}

void WS2812::DMA_Config(void)
{


    dma->rcc_enable();
    dma->nvic(DISABLE, 0, 0);
    dma->interrupt(DmaItTc, DISABLE);
    dma->interrupt(DmaItTe, DISABLE);
    dma->interrupt(DmaItHt, DISABLE);

    DMA_InitTypeDef DMA_InitStruct;

    dma->deInit();


    DMA_InitStruct.DMA_PeripheralBaseAddr = dma_target; 			// Specifies Physical address of the peripheral in this case Timer 2 CCR1
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

    dma->init(&DMA_InitStruct);

    TIM_DMACmd(TIMx, dam_cc, ENABLE);						// Enables TIM2's DMA request. TIM_DMA_CC1 : TIM Capture Compare 1 DMA source

}


void WS2812::init_info(Gpio *pwm_pin)
{
    switch((uint8_t)pwm_pin->id)
    {
    //TIM2
    case PA0_ID:
        TIMx = TIM2;
        ch = TIMxCH1;//irq = TIM2_IRQn;
        dma = &Dma1Ch5;
        break;
    case PA1_ID:
        TIMx = TIM2;
        ch = TIMxCH2;//irq = TIM2_IRQn;
        dma = &Dma1Ch7;
        break;
    case PA2_ID:
        TIMx = TIM2;
        ch = TIMxCH3;//irq = TIM2_IRQn;
        dma = &Dma1Ch1;
        break;
    case PA3_ID:
        TIMx = TIM2;
        ch = TIMxCH4;//irq = TIM2_IRQn;
        dma = &Dma1Ch7;
        break;



    //TIM1
    case PA8_ID:
        TIMx = TIM1;
        ch = TIMxCH1;//irq = TIM4_IRQn;
        dma = &Dma1Ch2;
        break;
    case PA9_ID:
        TIMx = TIM1;
        ch = TIMxCH2;//irq = TIM4_IRQn;
        dma = &Dma1Ch3;
        break;
    case PA10_ID:
        TIMx = TIM1;
        ch = TIMxCH3;//irq = TIM4_IRQn;
        dma = &Dma1Ch6;
        break;


    //TIM4
    case PB6_ID:
        TIMx = TIM4;
        ch = TIMxCH1;//irq = TIM4_IRQn;
        dma = &Dma1Ch1;
        break;
    case PB7_ID:
        TIMx = TIM4;
        ch = TIMxCH2;//irq = TIM4_IRQn;
        dma = &Dma1Ch4;
        break;
    case PB8_ID:
        TIMx = TIM4;
        ch = TIMxCH3;//irq = TIM4_IRQn;
        dma = &Dma1Ch5;
        break;


    //TIM3
    case PA6_ID:
        TIMx = TIM3;
        ch = TIMxCH1;//irq = TIM3_IRQn;
        dma = &Dma1Ch6;
        break;
    case PB0_ID:
        TIMx = TIM3;
        ch = TIMxCH3;//irq = TIM3_IRQn;
        dma = &Dma1Ch2;
        break;
    case PB1_ID:
        TIMx = TIM3;
        ch = TIMxCH4;//irq = TIM3_IRQn;
        dma = &Dma1Ch3;
        break;
    }
}
