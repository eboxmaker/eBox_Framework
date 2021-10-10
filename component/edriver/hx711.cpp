#include "hx711.h"


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_HX711_ENABLE       false
#define EBOX_DEBUG_HX711_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_HX711_ENABLE
#define hxDebug(...)  ebox_printf("[hx DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define hxDebug(...)
#endif

#if EBOX_DEBUG_HX711_ENABLE_ERR
#define hxDebugErr(fmt, ...)  ebox_printf("[hx err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define hxDebugErr(fmt, ...)
#endif

HX711::HX711(Gpio *sck, Gpio *dout)
{
    _sck = sck;
    _dout = dout;
}
void HX711::begin()
{
    _sck->mode(OUTPUT_PP_PD);
    _dout->mode(INPUT_PU);
    filter = new FilterMoveWindow(10);
   
}
float HX711::get_raw()
{
    return raw;
}

uint32_t HX711::read()
{
    unsigned long Count;
    unsigned char i;
//    ADSK=0; //使能AD（PD_SCK 置低）
    _sck->reset();
    Count=0;
    while(_dout->read()); //AD转换未结束则等待，否则开始读取
    for (i=0;i<24;i++)
    {
        _sck->set(); //PD_SCK 置高（发送脉冲）
        Count=Count<<1; //下降沿来时变量Count左移一位，右侧补零
        _sck->reset(); //PD_SCK 置低
        if(_dout->read()) Count++;
    }
    _sck->set();
    Count=Count^0x800000;//第25个脉冲下降沿来时，转换数据
    _sck->reset();
    return(Count);

}
void HX711::loop()
{
    if(millis() - last > 100)
    {
        last = millis();
        uint32_t value = read();
        raw = filter->in(value);
        hxDebug("value:%d,\t filter:%0.1f\n",value,raw);

    }
}


Balance::Balance(Gpio *sck, Gpio *dout):HX711(sck,dout)
{
    linear = new Linear;
    
}
void Balance::add_row(double y)
{
    linear->add_row(get_raw(),y);
}
void Balance::calibration()
{
    linear->exe();
    rate = linear->rate;
    offset = linear->offset;
}
void Balance::set_zero()
{
    offset = 0;
}

float Balance::get_weight()
{
    return (rate * get_raw() + offset);
}
