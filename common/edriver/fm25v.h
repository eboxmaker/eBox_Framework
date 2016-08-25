/**
  ******************************************************************************
  * @file    fm25v.h
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

#ifndef __SPI2_RAM_H
#define __SPI2_RAM_H

#include "ebox.h"


/*+++++++++++++++++++++++++++++++++FM25V02++++++++++++++++++++++++++++++++++*/
// FM25V02 ADDR: 0x000000 ~ 0x007FFF.
//   1/4         0x006000 ~ 0x007FFF
//   1/2         0x004000 ~ 0x007FFF
// FM25V02 Control Command
#define		WREN			0x06						// Set Write Enable Latch
#define		WRDI			0x04						// Write Disable
#define		RDSR			0x05						// Read Status Register
#define		WRSR			0x01						// Write Status Register 
#define		READ			0x03						// Read Memory Data.
#define 	FSTRD			0x0B						// Fast Read Memory Data.
#define		WRITE			0x02 						// Write Memory Data
#define		SLEEP			0xB9						// Enter Sleep Mode.
#define		RDID			0x9F						// Read Device ID.
#define 	SNR				0xC3						// Read S/N.







/*+++++++++++++++++++++++++++++++++FM25V02++++++++++++++++++++++++++++++++++*/


class FM25V
{
	public:
		FM25V(Gpio *cs,Spi *spi)
		{
			this->cs  = cs;
			this->spi = spi;
		}
		void begin(uint8_t dev_num);//
		void read_id(uint16_t *id); //
		void read(uint8_t *buf,u32 read_addr,u16 num_to_read);// 
		void fast_read(uint8_t *buf,u32 read_addr,u16 num_to_read);//
		void write(uint8_t *buf,u32 write_addr,u16 num_to_write);

	private:
		uint8_t                  spi_flash_buf[1024];
		SPI_CONFIG_TYPE     spi_dev_FM25V;
		Gpio                *cs;
		Spi                 *spi;

		uint8_t   readSR(void);  //
		void _waitBusy(void);//
		void powerDown(void);//	
		void wakeUp(void);   //
		void writeSR(uint8_t sr);//
		void writeEnable(void);//
		void writeDisable(void); //
	

};


#endif





