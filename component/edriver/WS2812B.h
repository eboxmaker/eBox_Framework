
#ifndef __WS2812B_H
#define	__WS2812B_H
#include "ebox.h"
#include "stm32f10x.h"
//#include "delay.h"
#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

#define TIMING_ONE  50
#define TIMING_ZERO 25

class WS2812B
{
	public:
		WS2812B(Gpio *pin);//Pwm *pwmw,
		void begin();
		void initialize(uint8_t length);
		void set_color(uint8_t green,uint8_t red,uint8_t bule, uint16_t num);
		void enable();

		void send(uint8_t green,uint8_t red,uint8_t bule, uint16_t len);
	private:
		DMA_Channel_TypeDef *dma_ch;
		Gpio        *pwm_pin;           //arduino pin number
		TIM_TypeDef *TIMx;
	    uint32_t    rcc;
	    uint8_t     ch;
	    uint16_t    oc_polarity;
		uint16_t LED_BYTE_Buffer[300];
		uint16_t bmemaddr;
		uint16_t bbuffersize;

		void _init_info(Gpio *pwm_pin);
		void _set_duty(void);
		void _base_init(void);
		char _set_dma();
};







#endif /* __LED_H */
