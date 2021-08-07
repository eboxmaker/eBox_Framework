#include "midea.h"

#define MideaWindAuto   5   
#define MideaWindLow    4      
#define MideaWindMid    2
#define MideaWindHigh   1
#define MideaWindConst  0

#define MideaModeAuto   2
#define MideaModeCool   0
#define MideaModeDehumidification    1
#define MideaModeHeat    3
#define MideaModeWind   1 //�ͷ�ģʽ�¶��޶���

#define MideaTemp17 B0000
#define MideaTemp18 B0001
#define MideaTemp19 B0011
#define MideaTemp20 B0010
#define MideaTemp21 B0110
#define MideaTemp22 B0111
#define MideaTemp23 B0101
#define MideaTemp24 B0100
#define MideaTemp25 B1100
#define MideaTemp26 B1101
#define MideaTemp27 B1001
#define MideaTemp28 B1000
#define MideaTemp29 B1010
#define MideaTemp30 B1011
#define MideaTempNone   B1110


uint8_t mideaTempTable[15] = {
    MideaTemp17,MideaTemp18,MideaTemp19,MideaTemp20,MideaTemp21,MideaTemp22,MideaTemp23,MideaTemp24,
    MideaTemp25,MideaTemp26,MideaTemp27,MideaTemp28,MideaTemp29,MideaTemp30,MideaTempNone,
};
uint8_t mideaWindTable[] = {MideaWindAuto,MideaWindLow,MideaWindMid,MideaWindHigh,MideaWindConst};
uint8_t mideaModeTable[] = {MideaModeAuto,MideaModeCool,MideaModeDehumidification,MideaModeHeat,MideaModeWind};
        
void MideaNormal::cool_26()
{
    settings.temp = 26;
    settings.modeIndex = ModeCool;
    settings.windIndex = WindAuto;
    B = B10111111;//����Auto
    C = B11010000;//26�㣬Auto
    send();
}


void MideaNormal::wind(Wind_t index)
{
    uint8_t i = index;
    if(mideaModeTable[settings.modeIndex] == MideaModeAuto || mideaModeTable[settings.modeIndex] == MideaModeDehumidification)//������Զ�ģʽ��ֹ�ı����
        return ;
    limit(i,(uint8_t)0,(uint8_t)(getArraySize(mideaWindTable)-1));
    settings.windIndex = (Wind_t)i;
    B &= B00011111;
    B |= (mideaWindTable[settings.windIndex]<<5);
    send();
}
void MideaNormal::windNext()
{
    uint8_t i = settings.windIndex;
    if(mideaModeTable[settings.modeIndex] == MideaModeAuto)//������Զ�ģʽ��ֹ�ı����
        return ;
    i++;
    if(i >= getArraySize(mideaWindTable))
        i = 0;
    settings.windIndex = (Wind_t)i;

    B &= B00011111;
    B |= (mideaWindTable[settings.windIndex]<<5);
    send();

}
void MideaNormal::temp(uint8_t value)
{
    limit(value,(uint8_t)17,(uint8_t)30);
    settings.temp = value;
    C &= B00001111;
    C |= mideaTempTable[settings.temp - 17] << 4;
    send();
}

void MideaNormal::tempAdd()
{
    settings.temp++;
    if(settings.temp > 30) settings.temp = 17;
    C &= B00001111;
    C |= mideaTempTable[settings.temp - 17] << 4;
    send();
}
void MideaNormal::tempSub()
{
    settings.temp--;
    if(settings.temp < 17) settings.temp = 30;
    C &= B00001111;
    C |= mideaTempTable[settings.temp - 17] << 4;
    send();
}        
void MideaNormal::mode(Mode_t index)
{
    Wind_t curWindIndex;
    uint8_t curTemp;
    uint8_t i = index;
    limit(i,(uint8_t)0,(uint8_t)(getArraySize(mideaModeTable)-1));
    settings.modeIndex = (Mode_t)i;
    
    switch(settings.modeIndex)
    {
        case 0://�Զ�ģʽ���̶���
        case 2://��ʪ,�̶���
            curWindIndex = WindConst;
            curTemp = settings.temp;
            break;  
        case 1://����
        case 3://����
            curWindIndex = settings.windIndex;
            curTemp = settings.temp;
            break;        
        case 4://�ͷ磬���¶�
            curWindIndex = settings.windIndex;
            curTemp = 31;//31�ȶ�Ӧ�����¶ȴ���������
            break;        
    }
    //���÷�
    B &= B00011111;
    B |= (mideaWindTable[curWindIndex]<<5);

    //�����¶�
    C &= B00001111;
    C |= mideaTempTable[curTemp - 17] << 4;     
    //����ģʽ
    C &= B11110011;
    C |= mideaModeTable[settings.modeIndex] << 2;
    send();
}
void MideaNormal::modeNext()
{
    Wind_t curWindIndex;
    uint8_t curTemp;
    uint8_t i = settings.modeIndex;
    i++;
    if(i >= getArraySize(mideaModeTable))
        i = 0;
    settings.modeIndex = (Mode_t)i;

    switch(settings.modeIndex)
    {
        case 0://�Զ�ģʽ���̶���
        case 2://��ʪ,�̶���
            curWindIndex = WindConst;
            curTemp = settings.temp;
            break;  
        case 1://����
        case 3://����
            curWindIndex = settings.windIndex;
            curTemp = settings.temp;
            break;        
        case 4://�ͷ磬���¶�
            curWindIndex = settings.windIndex;
            curTemp = 31;//31�ȶ�Ӧ�����¶ȴ���������
            break;        
    }
    //���÷�
    B &= B00011111;
    B |= (mideaWindTable[curWindIndex]<<5);

    //�����¶�
    C &= B00001111;
    C |= mideaTempTable[curTemp - 17] << 4;     
    //����ģʽ
    C &= B11110011;
    C |= mideaModeTable[settings.modeIndex] << 2;
    send();
}
void MideaNormal::powerOff()
{
    uint8_t b = B01111011;
    uint8_t c = B11100000;
    uint8_t Q = 0;
    send_l();
    send_byte(A);
    send_byte(~A);
    send_byte(b);
    send_byte(~b);
    send_byte(c);
    send_byte(~c);

    send_s();
    send_l();
    send_byte(A);
    send_byte(~A);
    send_byte(b);
    send_byte(~b);
    send_byte(c);
    send_byte(~c);
    
    send_s();
    send_l();
    send_byte(A);
    send_byte(~A);
    send_byte(Q);
    send_byte(~Q);
    send_byte(Q);
    send_byte(~Q);
    
    
    send_e();
}

void MideaNormal::send()
{
    send_l();
    send_byte(A);
    send_byte(~A);
    send_byte(B);
    send_byte(~B);
    send_byte(C);
    send_byte(~C);

    send_s();
    send_l();
    send_byte(A);
    send_byte(~A);
    send_byte(B);
    send_byte(~B);
    send_byte(C);
    send_byte(~C);
    send_e();
}
String MideaNormal::modetostr(Mode_t mode)
{
    switch(mode)
    {
        case ModeAuto:
            return "�Զ�";
        case ModeCool:
            return "����";
        case ModeDehumidification:
            return "��ʪ";
        case ModeHeat:
            return "����";
        case ModeWind:
            return "�ͷ�";
        default:
            return "xxx";
    }
}
String MideaNormal::windtostr(Wind_t mode)
{
    switch(mode)
    {
        case WindAuto:
            return "�Զ�";
        case WindConst:
            return "�̶�";
        case WindHigh:
            return "��";
        case WindLow:
            return "��";
        case WindMid:
            return "��";
        default:
            return "xxx";        
    }
}
void MideaNormal::print(Uart &uart)
{
    uart.print("�¶ȣ�");
    uart.println(settings.temp);
    uart.print("ģʽ��");
    uart.println(modetostr(settings.modeIndex));
    uart.print("���٣�");
    uart.println(windtostr(settings.windIndex));
}
