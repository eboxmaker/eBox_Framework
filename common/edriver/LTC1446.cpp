/**
  ******************************************************************************
  * @file    ltc1446.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "LTC1446.h"


void LTCX::begin(uint8_t dev_num)
{
    spi_dev_LTC1446.dev_num = dev_num;
    spi_dev_LTC1446.mode = SPI_MODE0;
    spi_dev_LTC1446.prescaler = SPI_CLOCK_DIV256;
    spi_dev_LTC1446.bit_order = SPI_BITODER_MSB;

    spi->begin(&spi_dev_LTC1446);
    cs->mode(OUTPUT_PP);
    cs->set();
}
/**
  ******************************************************************************
  * @brief  DAC写数据
  * @param  unsigned int
	*			_DAC_A	通道A的值，低12位有效
	*			_DAC_B	通道B的值，低12位有效
  * @note   特别说明,  可略, 仅加在必要的地方.
  * @retval None, 返回值描述
  ******************************************************************************
  */
void LTCX::write(uint16_t _DACA, uint16_t _DACB)
{
    uint32_t _DAC_volue = _DACA << 12 | (_DACB & 0x0fff);
    spi->take_spi_right(&spi_dev_LTC1446);
    cs->reset();
    // 将_DAC_volue的值从24-->0的顺序写入设备，每次8bit
    spi->write(((uint8_t *)(&(_DAC_volue)))[2]);
    spi->write(((uint8_t *)(&(_DAC_volue)))[1]);
    spi->write(((uint8_t *)(&(_DAC_volue)))[0]);
    cs->set();
    spi->release_spi_right();
}

