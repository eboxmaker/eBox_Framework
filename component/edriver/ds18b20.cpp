/* Includes ------------------------------------------------------------------*/
#include "crc.h"
#include "ds18b20.h"
/* Extern variables ----------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/

bool Ds18b20::begin()
{
    
    int ret = 0;
    pin->mode(OUTPUT_PP_PU);
    ret = reset();
    if(ret == 0) return false;
    write(0xcc);
    write(0x44);
    delay_ms(20);
//    uart1.printf("芯片初始化(ret = %d)\n",ret);
    update_temp();//更新读取
    last = millis();
    state = 0;
    return true;
}

int Ds18b20::is_exist()
{
    return reset();
}

int Ds18b20::reset()
{
    uint8_t retry = 0;
    pin->mode(OUTPUT_PP_PU);
    pin->reset();
    delay_us(500);
    pin->mode(INPUT_PU);
    delay_us(30);//15-60us后，拉低总线60-240us（实测30us）。
    while(pin->read() == 1)//等待总线拉低,240us后超时
    {
        delay_us(10);
        retry++;
        if(retry > 10) return 0;//不存在DS18B20
    }
    retry = 0;
    while(pin->read() == 0)//等待DS18B20释放总线
    {
        delay_us(10);
        retry++;
        if(retry > 24) return -1;//DS18B20未释放总线，出现错误
    }
    return 1;
}
void Ds18b20::write(uint8_t data)
{
    //传输方式：LSB
    uint8_t bit = 0;
    pin->mode(OUTPUT_PP_PU);
    for(int i = 0; i < 8; i++)
    {
        bit = data & 0x01;
        pin->reset();
        if(bit)
        {
//            pin->mode(INPUT_PU);//释放总线
            delay_us(1);//最大拉低时间1-15us
            pin->set();//电阻上拉起作用
            delay_us(14);//最大拉低时间1-15us

        }
        else//继续拉低电平最少60微秒直至写周期结束
        {
            delay_us(15);//最大拉低时间1-15us
        }
        delay_us(45);//则继续拉低电平最少60微秒直至写周期结束
        pin->set();//电阻上拉起作用
        delay_us(1);//写间隙延迟>1us
        data >>= 1;
    }
}

uint8_t Ds18b20::read()
{
    uint8_t data = 0;
    for(int i = 0; i < 8; i++)
    {
        pin->mode(OUTPUT_PP_PU);
        data >>= 1;
        pin->reset();
        delay_us(1);//拉低总线至少1-15us
        pin->mode(INPUT_PU);//释放总线
        delay_us(1);//从拉低时刻起，
        if(pin->read()) data |= 0x80;//从拉低时刻起，到读数据最大不能超过15us
        delay_us(30);//满足系统最小读取时间60us要求
    }
    return data;
}
bool Ds18b20::update_temp()
{
    bool ret = false;
    Data16_t xdata;
//    memset(buf,0,10);
    no_interrupts();
    reset();
    write(0xcc);
    write(0xbe);

    for(int i = 0; i < 2; i++)
    {
        buf[i] = read();			//连续读高速缓存
    }
    interrupts();

    xdata.byte[0] = buf[0];//温度数据,11位有效数字(精度0.125度)
    xdata.byte[1] = buf[1];
    xdata.value = (xdata.value << 5)/32;//将11位补码格式变换为16补码格式
    temperature = xdata.value * 0.0625; 

    ret = true;
//    uart1.printf("更新完成%0.1f℃,0X%04X(0X%02X,0X%02X)\n",temperature,xdata.value,buf[0],buf[1]);
    return ret;
}

float Ds18b20::get_temp()
{
//    uart1.printf("温度：%0.1f\n",temperature);
    return temperature;
}
void Ds18b20::loop()
{
    switch(state)
    {
        case 0:
            reset();
            write(0xcc);
            write(0x44);
            state = 1;
            last = millis();
//            uart1.printf("更新温度\n");
            break;
        case 1:
            if(millis() - last > 750)
            {
                update_temp();
                state = 0;
            }
            break;
    }


}

