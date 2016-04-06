/*
file   : *.cpp
author : hh.l
version: V1.0
date   : 2016/2
define all io in the project
*/
#include "fm25v.h"

void FM25V::begin(uint8_t dev_num)
{
	spi_dev_FM25V.dev_num = dev_num;
	spi_dev_FM25V.mode = SPI_MODE0;
	spi_dev_FM25V.prescaler = SPI_BaudRatePrescaler_4;
	spi_dev_FM25V.bit_order = SPI_BITODER_MSB;
	
	spi->begin(&spi_dev_FM25V);
	cs->mode(OUTPUT_PP);
	cs->set();    
}

void FM25V::read_id (uint16_t *ID)
{
    u8 pp[2];
    *ID=0;
    spi->take_spi_right (&spi_dev_FM25V);
    cs->reset();
    spi->write (RDID);
    spi->read(); //0-0x7f
    spi->read();//1-0x7f
    spi->read(); //2-0x7f
    spi->read(); //3-0x7f
    spi->read(); //4-0x7f
    spi->read();//5-0x7f
   
    pp[0]=spi->read();//6-0xc2
    pp[1]=spi->read(); //7- 22h   01h 128k 02h 256k  03h 512k  04h 1M
    spi->read(); // 8-00h fm25v02     
    cs->set(); 
    spi->release_spi_right(); 
    *ID=(*ID|pp[0])<<8;
    *ID|=pp[1];
}


//等待空闲
void FM25V::_waitBusy(void)   
{   
	while ((readSR()&0x01)==0x01);   // 等待BUSY位清空
}  
//进入掉电模式
void FM25V::powerDown(void)   
{ 
		
	cs->reset();
    spi->write(SLEEP);        //发送掉电命令  
    //等待TPD  
//	delay_ms(1);
	cs->set();

}   
//唤醒
void FM25V::wakeUp(void)   
{  
 	volatile int i;	 	
 	cs->reset();
 
   	delay_us(500);
	cs->set();
}  

/***************************************************************
函数名称 :  void SPI_Flash_Read(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead)   
功能描述 : 读取SPI FLASH， 在指定地址开始读取指定长度的数据
输入参数 : 
			pBuffer:数据存储区
			ReadAddr:开始读取的地址(15bit)
			NumByteToRead:要读取的字节数(最大32767)
输出参数 : 无
返回值   : 无
***************************************************************/
void FM25V::read(u8 *buf,u32 read_addr,u16 num_to_read)
{
    
     spi->take_spi_right(&spi_dev_FM25V );  
     cs->reset(); 
     spi->write(READ);
	 spi->write((u8)((read_addr)>>16));    
	 spi->write((u8)((read_addr)>>8));   
	 spi->write((u8)read_addr);   
     spi->read(buf,num_to_read);
     cs->set(); 
     spi->release_spi_right();
}
/***************************************************************
函数名称 :  void SPI_Flash_Read(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead)   
功能描述 : 读取SPI FLASH， 在指定地址开始读取指定长度的数据
输入参数 : 
			pBuffer:数据存储区
			ReadAddr:开始读取的地址(15bit)
			NumByteToRead:要读取的字节数(最大32767)
输出参数 : 无
返回值   : 无
***************************************************************/
void FM25V::fast_read(u8 *buf,u32 read_addr,u16 num_to_read)   
{ 
	 spi->take_spi_right(&spi_dev_FM25V);
	 cs->reset();
	 spi->write(FSTRD);           
	 spi->write((u8)((read_addr)>>16));      
	 spi->write((u8)((read_addr)>>8));   
	 spi->write((u8)read_addr);   
	 spi->write(0xff);   
	 spi->read(buf,num_to_read);
	 cs->set();
	 spi->release_spi_right();

} 


/***************************************************************
函数名称 : void SPI_Flash_Write(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)     
功能描述 : 写SPI FLASH，在指定地址开始写入指定长度的数据，该函数带擦除操作!
输入参数 : 
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(15bit)
			NumByteToWrite:要写入的字节数(最大32767)  		   
输出参数 : 无
返回值   : 无
***************************************************************/
void FM25V::write(u8 *buf,u32 write_addr,u16 num_to_write)   
{ 
	 spi->take_spi_right(&spi_dev_FM25V);
     FM25V::writeEnable();
     cs->reset();
	 spi->write(WRITE);           
	 spi->write((u8)((write_addr)>>16));    
	 spi->write((u8)((write_addr)>>8));   
	 spi->write((u8)write_addr); 
	 spi->write(buf,num_to_write);
	 cs->set();    
     FM25V::writeEnable();
	 spi->release_spi_right();
	
}

/***************************************************************
函数名称 :  u8 SPI_Flash_ReadSR(void)   
功能描述 : 读取W25X16的状态寄存器
				BIT7  6   5   4   3   2   1   0
				WREN  0   0   0  BP2 BP1 WEL  0
				说明：
				WREN:默认0,状态寄存器保护位,配合WP使用
				BP2,BP1:FLASH区域写保护设置
				WEL:写使能锁定flag				
				默认:0x00
输入参数 : 无
输出参数 : 无
返回值   : byte	寄存器状态
***************************************************************/
u8 FM25V::readSR(void)  
{  
	u8 byte=0;   
    spi->take_spi_right(&spi_dev_FM25V);
	cs->reset();
	 spi->write(RDSR);      
	byte= spi->read();               
	cs->set();
    spi->release_spi_right(); 
	return byte;   
   
} 

/***************************************************************
函数名称 :  void SPI_FLASH_Write_SR(u8 sr)   
功能描述 : 写SPI_FLASH状态寄存器， 只有WREN BP2 BP1 (bit 7,4,3)可以写!!!
输入参数 : sr:状态
输出参数 : 无
返回值   : 无
***************************************************************/
void FM25V::writeSR(u8 sr)   
{   
    spi->take_spi_right(&spi_dev_FM25V);
    writeEnable();    
	cs->reset();
	 spi->write(WRSR);     
	 spi->write(sr);                
	cs->set();
    spi->release_spi_right(); 
}
/***************************************************************
函数名称 : void FM25V::writeEnable(void)     
功能描述 : SPI_FLASH写使能，将WEL置位
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void FM25V::writeEnable(void)   
{
    spi->take_spi_right(&spi_dev_FM25V);
	cs->reset();
     spi->write(WREN);      
	cs->set();
    spi->release_spi_right(); 
} 
/***************************************************************
函数名称 :  void FM25V::writeDisable(void)     
功能描述 : SPI_FLASH写禁止， 将WEL清零 
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void FM25V::writeDisable(void)   
{  
    spi->take_spi_right(&spi_dev_FM25V);
 	cs->reset();
    spi->write(WRDI);        
	cs->set();
    spi->release_spi_right(); 
} 


