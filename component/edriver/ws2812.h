#ifndef __WS2812_H
#define __WS2812_H
#include "ebox_core.h"
#include "dma.h"
#define TIM_PERIOD		72					// pwmÆµÂÊ£º1Mhz
#define PWM_HIGH_WIDTH		48				// 6.6us
//Duty cycle = PWM_HIGH_WIDTH/TIM_PERIOD*100
#define PWM_LOW_WIDTH		24					// 3.3us
//Duty cycle = PWM_LOW_WIDTH/TIM_PERIOD*100
//9/29*100
#define COLUMBS			32
#define	ROWS			16

#define LED_COUNT		COLUMBS*ROWS

// Buffer size needs to be the number of LEDs times 24 bits plus 42 trailing bit to signify the end of the data being transmitted.
#define LED_BUFFER_SIZE		24*LED_COUNT+42

class WS2812
{
public:
    WS2812(Gpio *p_pin)
    {
        pin = p_pin;

    }
    void begin();
    void reset();
    void display(uint8_t *ptr, uint16_t height, uint16_t width);

    void rainbow_Loop();
    void rainbow_Loop1();



private:
    Gpio *pin;
    TIM_TypeDef *TIMx;
    Dma *dma;
    uint32_t dam_cc;
    uint32_t dma_target;
    uint8_t ch;

    void TIM_Config(void);
    void DMA_Config(void);
    void send_data(uint16_t len);

    void init_info(Gpio *pwm_pin);

};
#endif
