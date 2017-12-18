/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"
/* Extern variables ----------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/

int Ds18b20::begin()
{    
    return reset();
}

int Ds18b20::is_exist()
{
    return reset();
}

int Ds18b20::reset()
{
    uint8_t retry = 0;    
    pin->mode(OUTPUT_PP);
    pin->reset();
    delay_us(480);
    pin->mode(INPUT);
    delay_us(30);//15-60us后，拉低总线60-240us（实测30us）。
    while(pin->read())//等待总线拉低,60us后超时
    {
        delay_us(10);
        retry++;    
        if(retry > 6) return 0;//不存在DS18B20
    }
    retry = 0;
    while(!pin->read())//等待总线拉高
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
    uint8_t bit;
    no_interrupts();
    for(int i = 0; i < 8; i++)
    {
        pin->mode(OUTPUT_PP);
        bit = data&0x01;
        pin->reset();
        delay_us(10);//最大拉低时间15us
        if(bit){
            pin->set();//电阻上拉起作用
        }
        else{
            
        }
        delay_us(40);//
        pin->mode(INPUT);//释放总线
        delay_us(1);//写间隙延迟>1us
        data>>=1;
    }
    interrupts();
}

uint8_t Ds18b20::read()
{
    u8 data=0;
    no_interrupts();
    for(int i=0;i<8;i++)								
    { 
        data>>=1;
        pin->mode(OUTPUT_PP);
        pin->reset();
        delay_us(1);//拉低总线至少1us
        pin->mode(INPUT);//释放总线
        delay_us(1);//从拉低时刻起，到读数据最大不能超过15us
        if(pin->read()) data |= 0x80;
        delay_us(45);//满足系统最小读取时间60us要求
    }    
    interrupts();
    return data;
}
float Ds18b20::get_temp()
{
    u8 flag;
    u8 temp_l,temp_h;
    float temperature;
    uint8_t buf[9];
    
    reset();
    write(0xcc);
    write(0x44);
    
    reset();
    write(0xcc);
    write(0xbe);

    for(int i = 0;i < 9; i++)
    {
      buf[i] = read();			//连续读高速缓存
    }
    
    if (crc8(buf, 9) == 0 )
    {
      temp_l = buf[0];						//温度数据,11位有效数字(精度0.125度)
      temp_h = buf[1];
      if(temp_h > 7)
      {
          temp_h =~ temp_h;
          temp_l =~ temp_l; 
          flag = 0;//温度为负  
      }
      else
          flag = 1;//温度为正
      
    temperature = ((temp_h << 8) | temp_l) * 0.0625; //获得高八位
    if(flag)
      return temperature; //返回温度值
    else
      return -temperature;
    
    }
    else
      return 0;
}


