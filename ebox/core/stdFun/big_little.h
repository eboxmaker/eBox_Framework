#ifndef __BIG_LITTLE_H
#define __BIG_LITTLE_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
void write_u8(unsigned char **pptr,unsigned char data);//��С������ת���ɴ��ģʽ�洢
void write_u16_little(unsigned char **pptr,unsigned short int data);//С��ģʽ
void write_u16_big(unsigned char **pptr,unsigned short int data);//���ģʽ
unsigned short int read_u8(unsigned char **pptr);//��С������ת���ɴ��ģʽ�洢
unsigned short int read_u16_little(unsigned char **pptr);//С��ģʽ��ȡ���ֽ�
unsigned short int read_u16_big(unsigned char **pptr);//���ģʽ��ȡ���ֽ�
    
#ifdef __cplusplus
} 
#endif // __cplusplus
#endif
