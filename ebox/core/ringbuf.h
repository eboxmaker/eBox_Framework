/**
  ******************************************************************************
  * @file    color_convert.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RING_H
#define __RING_H
/**
ʹ�÷�����
�û�����һ���Զ����unsigned char/uint8_t���͵Ļ�����.���磺uint8_t buf[512];
ʹ��RINGBUF ringbuf(buf,512);����һ������
�û����Ե���ringbuf.write(c);�������ݣ�����true�ɹ���
����ringbuf.available()���ɻ�û������������ݵĳ��ȣ�0��ʾ�գ�
�����Ϊ�գ����Ե���ringbuf.read()��ȡһ���ֽڣ�
Ҳ���Ե���ringbuf.read(userbuf,length);����ȡ�ض����ȵ����ݣ������userbuf�С�
*/
class RINGBUF
{


public:
    RINGBUF();
    void begin(unsigned char *buf, int lenght);
    void write(unsigned char c);
    unsigned char read(void);
    int available();
    void clear();

private:
    volatile int head;
    volatile int tail;
    int max;
    unsigned char *buf;





};
#endif
