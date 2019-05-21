/**
  ******************************************************************************
  * @file    at24cxx.h
  * @author  wele
  * @version V1.0
  * @date    2018/12/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of wele. This specification is
  * preliminary and is subject to change at any time without notice. wele assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2018 wele. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __AT24CXX_H
#define __AT24CXX_H
#include "ebox_core.h"

#define E2P_SPORT_MULTIPLE   0           /**< EEP 是否支持多个不同容量的EEPROM*/

#define AT24Cxx
#define SLAVE_ADDR 0xA0                  /**< EEP 默认A2 A1 A0 低电平*/

/*--------------------------------- DrvEeprom -------------------------------*/
#define  E2P_01_PSIZE        8            /**< EEP的每页的字节 */
#define  E2P_01_TSIZE        0x007f       /**< EEP的总字节数 */
#define  E2P_02_PSIZE        8
#define  E2P_02_TSIZE        0x00ff
#define  E2P_04_PSIZE        16
#define  E2P_04_TSIZE        0x01ff
#define  E2P_08_PSIZE        16
#define  E2P_08_TSIZE        0x03ff
#define  E2P_16_PSIZE        16
#define  E2P_16_TSIZE        0x07ff
#define  E2P_32_PSIZE        32
#define  E2P_32_TSIZE        0x0fff
#define  E2P_64_PSIZE        32
#define  E2P_64_TSIZE        0x1fff
#define  E2P_128_PSIZE       64
#define  E2P_128_TSIZE       0x3fff
#define  E2P_256_PSIZE       64
#define  E2P_256_TSIZE       0x7fff
#define  E2P_512_PSIZE       128
#define  E2P_512_TSIZE       0xffff
#define  E2P_1024_PSIZE      256       //暂不支持，需要32位地址
#define  E2P_1024_TSIZE      0x1ffff
#define  E2P_2048_PSIZE      256       //暂不支持，需要32位地址
#define  E2P_2048_TSIZE      0x3ffff

#define  E2P_1024_DELAY      5         /**< EEP延时时间，页写周期或单次写最大为5ms */
#define  E2P_2048_DELAY      10        /**< EEP延时时间，页写周期或单次写最大为10ms */
#define  E2P_DEFAULT_DELAY   4         /**< EEP延时时间，页写周期或单次写最大为4ms */


class At24cxx
{
public:
    At24cxx(I2c *i2c)
    {
        this->i2c = i2c;
    }
    void begin(I2c::Config_t cfg,uint16_t eetype);
    
    int8_t slaveadj(uint16_t byte_addr);           //slave_add 自动调整
    
    int8_t write_byte(uint16_t byte_addr, uint8_t *buf, uint16_t num_to_write);

    int8_t	read_byte(uint16_t byte_addr, uint8_t *buf, uint16_t num_to_read);

    int8_t write_page(uint8_t *buf, uint32_t addr, uint16_t num_to_write);
private:
    I2c *i2c;
    I2c::Config_t cfg;
    
    uint16_t  ee_type; 
    uint8_t   slave_add;
	uint16_t  page_num;
	uint16_t  page_size;
	uint32_t  add_max;
	uint8_t   reg_add8;
	uint16_t  reg_add16;
	uint8_t   is_add8;
};

#endif
