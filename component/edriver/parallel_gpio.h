/**
  ******************************************************************************
  * @file    parallel_gpio.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  *     2019/6/10  cat_li  �޸�ʵ�֣�ʹ֮���Բ�������IO��ͬһ���˿�����IO��ͬһ��
  *       �� Gpios ���Բ������16������IO���
  *       �� Port ���Բ���ͬһ���˿ڵ�����IO��������IO
  * ע�⣺Port��������ebox_gpio�е� port_mode�ӿڣ����ɽ��ýӿ��Ƴ�
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

#ifndef  __PARALLEL_GPIO_H
#define  __PARALLEL_GPIO_H

#include "ebox_core.h"
#include "mcu.h"

class ParallelGpio
{
    public:
        ParallelGpio() {};
        virtual void mode(PinMode_t mode) = 0;
        virtual void write(uint16_t val) = 0;
        virtual void read(uint16_t *val) = 0;
        virtual uint16_t read(void) = 0;
};

/**
************************************************************************
* @brief Ч�ʱȽϵͣ����ǿ���֧�����16������IO���
************************************************************************
*/
class Gpios : public ParallelGpio
{
    public:
        Gpios(Gpio **pin,uint8_t num);
        virtual void mode(PinMode_t mode);
        virtual void write(uint16_t val);
        virtual void read(uint16_t *val);
        virtual uint16_t read(void);
    private:
        Gpio 			*_bit[16];
        uint8_t		_num;
};

/**
************************************************************************
* @brief Ч�ʸߣ���������ͬһ��PORT�˿ڲ���
************************************************************************
*/
class Port : public ParallelGpio
{
    public:
        /**
         *@brief    ����ͬһ��Port�Ķ��pin���������������Բ�����
         *@param    port �˿�,���� GPIOA_BASE �� 
         *          mask Ҫ������pin��
         *    ��1�� Ҫ���� PA2,PA3,PA4; �� port = GPIOA_BASE, mask = 0x1c;����0001 1100��
         *    ��2�� Ҫ���� PB0,PB4,PB5; �� port = GPIOB_BASE, mask = 0x31;����0011 0001��
         *@retval   NONE
         */
        //  Port(uint32_t port, uint32_t mask = 0xffffffff);
        /**
         *@brief    ����ͬһ��Port�Ķ��pin��ֻ��������Pin
         *@param    port �˿�,���� GPIOA_BASE �� 
         *          mask Ҫ������pin��
         *          pinoffset �����pin0��ƫ��
         *    ���� Ҫ���� PA2,PA3,PA4; �� port = GPIOB_BASE, pinnum = 3, pinoffset = 2
         *@retval   NONE
         */
        Port(uint32_t port, uint8_t pinnum, uint8_t pinoffset = 0);
        virtual void mode(PinMode_t mode);
        virtual void write(uint16_t val);
        virtual void read(uint16_t *val);
        virtual uint16_t read(void);
    private:
        uint16_t _mask;
        uint8_t _offset;
        GPIO_TypeDef *_port;

        //  void init_port(uint8_t port);
};

#endif
