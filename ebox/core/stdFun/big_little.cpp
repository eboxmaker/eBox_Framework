#include "big_little.h"



void write_u8(unsigned char **pptr,unsigned char data)//将小端数据转换成大端模式存储
{
    **pptr = data;//高字节
    (*pptr)++;
}

void write_u16_little(unsigned char **pptr,unsigned short int data)//小端模式
{
    **pptr = (data % 256);//低字节放在前位
    (*pptr)++;
    **pptr = (data / 256);//高字节放在后位
    (*pptr)++;

}
void write_u16_big(unsigned char **pptr,unsigned short int data)//大端模式
{
    **pptr = (data / 256);//高字节放在前位
    (*pptr)++;
    **pptr = (data % 256);//低字节放在后位
    (*pptr)++;
}
unsigned short int read_u8(unsigned char **pptr)//将小端数据转换成大端模式存储
{
    unsigned short int data;
     data = **pptr;//高字节
    (*pptr)++;
    return data;
}
unsigned short int read_u16_little(unsigned char **pptr)//小端模式读取两字节
{
    unsigned short int data;
    data = **pptr ;     //前位为低字节
    (*pptr)++; 
    data += **pptr * 256;//后位为高字节
    (*pptr)++;

    return data;
}
unsigned short int read_u16_big(unsigned char **pptr)//大端模式读取两字节
{
    unsigned short int data;
    data = **pptr * 256;//前位为高字节
    (*pptr)++;
    data += **pptr ;     //后位为低字节
    (*pptr)++; 
    return data;
}
