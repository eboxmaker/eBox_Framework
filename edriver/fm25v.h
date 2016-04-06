/*
file   : *.h
author : hh.l
version: V1.0
date   : 2016/2
define all io in the project
*/
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
		FM25V(GPIO *cs,SPI *spi)
		{
			this->cs  = cs;
			this->spi = spi;
		}
		void begin(uint8_t dev_num);//
		void read_id(uint16_t *id); //
		void read(u8 *buf,u32 read_addr,u16 num_to_read);// 
		void fast_read(u8 *buf,u32 read_addr,u16 num_to_read);//
		void write(u8 *buf,u32 write_addr,u16 num_to_write);

	private:
		u8                  spi_flash_buf[1024];
		SPI_CONFIG_TYPE     spi_dev_FM25V;
		GPIO                *cs;
		SPI                 *spi;

		u8   readSR(void);  //
		void _waitBusy(void);//
		void powerDown(void);//	
		void wakeUp(void);   //
		void writeSR(u8 sr);//
		void writeEnable(void);//
		void writeDisable(void); //
	

};


#endif





