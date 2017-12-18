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
    delay_us(30);//15-60us����������60-240us��ʵ��30us����
    while(pin->read())//�ȴ���������,60us��ʱ
    {
        delay_us(10);
        retry++;    
        if(retry > 6) return 0;//������DS18B20
    }
    retry = 0;
    while(!pin->read())//�ȴ���������
    {
        delay_us(10);
        retry++;    
        if(retry > 24) return -1;//DS18B20δ�ͷ����ߣ����ִ���
    }    
    return 1;
}
void Ds18b20::write(uint8_t data)
{
    //���䷽ʽ��LSB
    uint8_t bit;
    no_interrupts();
    for(int i = 0; i < 8; i++)
    {
        pin->mode(OUTPUT_PP);
        bit = data&0x01;
        pin->reset();
        delay_us(10);//�������ʱ��15us
        if(bit){
            pin->set();//��������������
        }
        else{
            
        }
        delay_us(40);//
        pin->mode(INPUT);//�ͷ�����
        delay_us(1);//д��϶�ӳ�>1us
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
        delay_us(1);//������������1us
        pin->mode(INPUT);//�ͷ�����
        delay_us(1);//������ʱ���𣬵�����������ܳ���15us
        if(pin->read()) data |= 0x80;
        delay_us(45);//����ϵͳ��С��ȡʱ��60usҪ��
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
      buf[i] = read();			//���������ٻ���
    }
    
    if (crc8(buf, 9) == 0 )
    {
      temp_l = buf[0];						//�¶�����,11λ��Ч����(����0.125��)
      temp_h = buf[1];
      if(temp_h > 7)
      {
          temp_h =~ temp_h;
          temp_l =~ temp_l; 
          flag = 0;//�¶�Ϊ��  
      }
      else
          flag = 1;//�¶�Ϊ��
      
    temperature = ((temp_h << 8) | temp_l) * 0.0625; //��ø߰�λ
    if(flag)
      return temperature; //�����¶�ֵ
    else
      return -temperature;
    
    }
    else
      return 0;
}


