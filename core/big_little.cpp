#include "big_little.h"



void write_u8(unsigned char **pptr,unsigned char data)//��С������ת���ɴ��ģʽ�洢
{
    **pptr = data;//���ֽ�
    (*pptr)++;
}

void write_u16_little(unsigned char **pptr,unsigned short int data)//С��ģʽ
{
    **pptr = (data % 256);//���ֽڷ���ǰλ
    (*pptr)++;
    **pptr = (data / 256);//���ֽڷ��ں�λ
    (*pptr)++;

}
void write_u16_big(unsigned char **pptr,unsigned short int data)//���ģʽ
{
    **pptr = (data / 256);//���ֽڷ���ǰλ
    (*pptr)++;
    **pptr = (data % 256);//���ֽڷ��ں�λ
    (*pptr)++;
}
unsigned short int read_u8(unsigned char **pptr)//��С������ת���ɴ��ģʽ�洢
{
    unsigned short int data;
     data = **pptr;//���ֽ�
    (*pptr)++;
    return data;
}
unsigned short int read_u16_little(unsigned char **pptr)//С��ģʽ��ȡ���ֽ�
{
    unsigned short int data;
    data = **pptr ;     //ǰλΪ���ֽ�
    (*pptr)++; 
    data += **pptr * 256;//��λΪ���ֽ�
    (*pptr)++;

    return data;
}
unsigned short int read_u16_big(unsigned char **pptr)//���ģʽ��ȡ���ֽ�
{
    unsigned short int data;
    data = **pptr * 256;//ǰλΪ���ֽ�
    (*pptr)++;
    data += **pptr ;     //��λΪ���ֽ�
    (*pptr)++; 
    return data;
}
