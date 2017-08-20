#include "ebox_encoder.h"

Encoder::Encoder(TIM_TypeDef *timer,Gpio *a,Gpio *b)
{
    this->TIMx = timer;
    this->a = a;
    this->b = b;
}
bool Encoder::begin(uint8_t mode)
{
    a->mode(INPUT);
    b->mode(INPUT);

    base_init(0xffff,1);
    set_encoder(mode);
    last_read_speed_time = 0;
}

void Encoder::base_init(uint16_t period, uint16_t prescaler)
{
    this->period = period;//¸üĞÂperiod


    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  
    switch((uint32_t)TIMx)
    {
        case TIM1_BASE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);break;
        case TIM2_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);break;
        case TIM3_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);break;
        case TIM4_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);break;
        case TIM5_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);break;
        case TIM6_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);break;
        case TIM7_BASE:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);break;
        case TIM8_BASE:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);break;
        default:
            return ;
    }
    TIM_TimeBaseStructure.TIM_Period = this->period - 1; //ARR
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    
    
}
void Encoder::set_encoder(uint8_t mode)
{
    TIM_ICInitTypeDef TIMx_ICInitStructure;

    //±àÂëÆ÷Ä£Ê½
    TIM_EncoderInterfaceConfig(TIMx, mode, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);
    
    TIMx_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉıÑØ²¶»ñ
    TIMx_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //Ó³Éäµ½TI1ÉÏ
    TIMx_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ
    TIMx_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨  
    
    TIMx_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(this->TIMx, &TIMx_ICInitStructure);
    TIM_ClearFlag(this->TIMx, TIM_FLAG_Update);
    
    TIMx_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(this->TIMx, &TIMx_ICInitStructure);
    TIM_ClearFlag(this->TIMx, TIM_FLAG_Update); 
    

    TIM_Cmd(TIMx, ENABLE); //

}


float Encoder::read_speed()
{
    uint64_t now ;
    float speed;
    now = millis();
    if(TIMx->CNT == 0)
        return 0;
    else
    {
        double temp = TIMx->CNT;
        TIMx->CNT = 0;
        if(read_direction() == 0)
            speed = temp * 6000 / (now - last_read_speed_time);
        else
            speed = (temp - 65536) * 6000 / (now - last_read_speed_time);

    }
    last_read_speed_time = now;
    return speed;

}

uint16_t Encoder::read_counter()
{
    uint16_t temp = TIMx->CNT;
    TIMx->CNT = 0;
    return  temp;
}
bool Encoder::read_direction()
{
     if(TIMx->CR1&0X00000010)
         return true;
     else
         return false;
}
