#ifndef __EBOX_ENCODER_H
#define __EBOX_ENCODER_H

#include "ebox_common.h"


class Encoder
{
    public:
        Encoder(TIM_TypeDef *timer,Gpio *a,Gpio *b);
        Encoder(TIM_TypeDef *timer,Gpio *a);
        bool begin(uint32_t pulse_per_circle,uint8_t mode = TIM_EncoderMode_TI12);
        double read_speed();//µ¥Î»£ºpulse/second;
        uint16_t read_counter();
        bool read_direction();

    private:
        void base_init(uint16_t period, uint16_t prescaler);
        void set_encoder(uint8_t mode = TIM_EncoderMode_TI12);

    private:
        TIM_TypeDef *TIMx;
        Gpio        *a;
        Gpio        *b;    
        uint16_t    period;
        uint64_t    last_read_speed_time;
        uint32_t    pulse_per_circle;
};

#endif
