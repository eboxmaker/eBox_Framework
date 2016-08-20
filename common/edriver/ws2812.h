#ifndef __WS2812_H
#define __WS2812_H
#include "ebox.h"

#define TIM_PERIOD		29					// Number of CPU cycles that will constitute 1 period
#define PWM_HIGH_WIDTH		17				// Duty cycle of pwm signal for a logical 1 to be read by the ws2812 chip. 
//Duty cycle = PWM_HIGH_WIDTH/TIM_PERIOD*100
#define PWM_LOW_WIDTH		9					// Duty cycle of pwm signal for a logical 0 to be read by the ws2812 chip.
//Duty cycle = PWM_LOW_WIDTH/TIM_PERIOD*100
//9/29*100
#define COLUMBS			8
#define	ROWS			8

#define LED_COUNT		COLUMBS*ROWS

#define LED_BUFFER_SIZE		24*LED_COUNT+42				// Buffer size needs to be the number of LEDs times 24 bits plus 42 trailing bit to signify the end of the data being transmitted.


extern uint8_t led_Colors[LED_COUNT];						//Array of integers that will function as indexes for the rgb array

class WS2812
{
public:
    WS2812(Gpio *p_pin)
    {
        pin = p_pin;
    }
    void	begin();
    void	set_color(u8 g, u8 r, u8 b);
    void	set_color(u32 color);
    void reset();
    void write(u32 color);
    void send_bit_high();
    void send_bit_low();
    void RCC_Config(void);
    void TIM_Config(void);
    void DMA_Config(void);
    void send_data(uint8_t *led_Colors, uint16_t len);
    void rainbow_Loop();
    void rainbow_Loop1();
private:
    Gpio *pin;

};
#endif
