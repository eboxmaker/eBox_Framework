#ifndef __BIG_LITTLE_H
#define __BIG_LITTLE_H

void write_u8(unsigned char **pptr,unsigned char data);//将小端数据转换成大端模式存储
void write_u16_little(unsigned char **pptr,unsigned short int data);//小端模式
void write_u16_big(unsigned char **pptr,unsigned short int data);//大端模式
unsigned short int read_u8(unsigned char **pptr);//将小端数据转换成大端模式存储
unsigned short int read_u16_little(unsigned char **pptr);//小端模式读取两字节
unsigned short int read_u16_big(unsigned char **pptr);//大端模式读取两字节
#endif
