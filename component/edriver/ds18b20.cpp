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
//    uart1.printf("оƬ��ʼ��(ret = %d)\n",ret);
    update_temp();//���¶�ȡ
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
    delay_us(30);//15-60us����������60-240us��ʵ��30us����
    while(pin->read() == 1)//�ȴ���������,240us��ʱ
    {
        delay_us(10);
        retry++;
        if(retry > 10) return 0;//������DS18B20
    }
    retry = 0;
    while(pin->read() == 0)//�ȴ�DS18B20�ͷ�����
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
    uint8_t bit = 0;
    pin->mode(OUTPUT_PP_PU);
    for(int i = 0; i < 8; i++)
    {
        bit = data & 0x01;
        pin->reset();
        if(bit)
        {
//            pin->mode(INPUT_PU);//�ͷ�����
            delay_us(1);//�������ʱ��1-15us
            pin->set();//��������������
            delay_us(14);//�������ʱ��1-15us

        }
        else//�������͵�ƽ����60΢��ֱ��д���ڽ���
        {
            delay_us(15);//�������ʱ��1-15us
        }
        delay_us(45);//��������͵�ƽ����60΢��ֱ��д���ڽ���
        pin->set();//��������������
        delay_us(1);//д��϶�ӳ�>1us
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
        delay_us(1);//������������1-15us
        pin->mode(INPUT_PU);//�ͷ�����
        delay_us(1);//������ʱ����
        if(pin->read()) data |= 0x80;//������ʱ���𣬵�����������ܳ���15us
        delay_us(30);//����ϵͳ��С��ȡʱ��60usҪ��
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
        buf[i] = read();			//���������ٻ���
    }
    interrupts();

    xdata.byte[0] = buf[0];//�¶�����,11λ��Ч����(����0.125��)
    xdata.byte[1] = buf[1];
    xdata.value = (xdata.value << 5)/32;//��11λ�����ʽ�任Ϊ16�����ʽ
    temperature = xdata.value * 0.0625; 

    ret = true;
//    uart1.printf("�������%0.1f��,0X%04X(0X%02X,0X%02X)\n",temperature,xdata.value,buf[0],buf[1]);
    return ret;
}

float Ds18b20::get_temp()
{
//    uart1.printf("�¶ȣ�%0.1f\n",temperature);
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
//            uart1.printf("�����¶�\n");
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

