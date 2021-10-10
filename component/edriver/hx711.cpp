#include "hx711.h"


#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
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
//    ADSK=0; //ʹ��AD��PD_SCK �õͣ�
    _sck->reset();
    Count=0;
    while(_dout->read()); //ADת��δ������ȴ�������ʼ��ȡ
    for (i=0;i<24;i++)
    {
        _sck->set(); //PD_SCK �øߣ��������壩
        Count=Count<<1; //�½�����ʱ����Count����һλ���Ҳಹ��
        _sck->reset(); //PD_SCK �õ�
        if(_dout->read()) Count++;
    }
    _sck->set();
    Count=Count^0x800000;//��25�������½�����ʱ��ת������
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
