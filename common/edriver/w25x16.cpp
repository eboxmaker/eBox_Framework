/**
  ******************************************************************************
  * @file    w25x16.cpp
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
#include "w25x16.h"


void W25x16::begin(uint8_t dev_num)
{
    spi_dev_w25x16.dev_num = dev_num;
    spi_dev_w25x16.mode = SPI_MODE0;
    spi_dev_w25x16.prescaler = SPI_CLOCK_DIV4;
    spi_dev_w25x16.bit_order = SPI_BITODER_MSB;

    spi->begin(&spi_dev_w25x16);
    cs->mode(OUTPUT_PP);
    cs->set();
}
void W25x16::read_id(uint16_t *id)
{
    spi->take_spi_right(&spi_dev_w25x16);

    cs->reset();
    spi->write(0x90);
    spi->write(0x00);
    spi->write(0x00);
    spi->write(0x00);
    *id |= spi->read() << 8;
    *id |= spi->read();
    cs->set();
    spi->release_spi_right();


}
/***************************************************************
函数名称 :  void SPI_Flash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
功能描述 : 读取SPI FLASH， 在指定地址开始读取指定长度的数据
输入参数 :
			pBuffer:数据存储区
			ReadAddr:开始读取的地址(24bit)
			NumByteToRead:要读取的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::read(uint8_t *buf, uint32_t read_addr, uint16_t num_to_read)
{
    spi->take_spi_right(&spi_dev_w25x16);
    cs->reset();
    spi->write(W25X_ReadData);         //发送读取命令
    spi->write((uint8_t)((read_addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((read_addr) >> 8));
    spi->write((uint8_t)read_addr);
    spi->read(buf, num_to_read);
    cs->set();
    spi->release_spi_right();

}
/***************************************************************
函数名称 :  void SPI_Flash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
功能描述 : 读取SPI FLASH， 在指定地址开始读取指定长度的数据
输入参数 :
			pBuffer:数据存储区
			ReadAddr:开始读取的地址(24bit)
			NumByteToRead:要读取的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::fast_read(uint8_t *buf, uint32_t read_addr, uint16_t num_to_read)
{
    spi->take_spi_right(&spi_dev_w25x16);
    cs->reset();
    spi->write(W25X_FastReadData);         //发送读取命令
    spi->write((uint8_t)((read_addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((read_addr) >> 8));
    spi->write((uint8_t)read_addr);
    spi->write(0xff);   //空字节
    spi->read(buf, num_to_read);
    cs->set();
    spi->release_spi_right();

}
/***************************************************************
函数名称 : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 : 写SPI FLASH，在指定地址开始写入指定长度的数据，该函数带擦除操作!
输入参数 :
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::write(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{

    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    spi->take_spi_right(&spi_dev_w25x16);

    secpos = write_addr / 4096; //扇区地址 0~511 for w25x16
    secoff = write_addr % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小

    if(num_to_write <= secremain)secremain = num_to_write; //不大于4096个字节
    while(1)
    {
        read(spi_flash_buf, secpos * 4096, 4096); //读出整个扇区的内容
        for(i = 0; i < secremain; i++) //校验数据
        {
            if(spi_flash_buf[secoff + i] != 0XFF)break; //需要擦除
        }
        if(i < secremain) //需要擦除
        {
            erase_sector(secpos);//擦除这个扇区
            for(i = 0; i < secremain; i++)	 //复制
            {
                spi_flash_buf[i + secoff] = buf[i];
            }
            write_no_check(spi_flash_buf, secpos * 4096, 4096); //写入整个扇区

        }
        else write_no_check(buf, write_addr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
        if(num_to_write == secremain)break; //写入结束了
        else//写入未结束
        {
            secpos++;//扇区地址增1
            secoff = 0; //偏移位置为0

            buf += secremain; //指针偏移
            write_addr += secremain; //写地址偏移
            num_to_write -= secremain;				//字节数递减
            if(num_to_write > 4096)secremain = 4096;	//下一个扇区还是写不完
            else secremain = num_to_write;			//下一个扇区可以写完了
        }
    }
    spi->release_spi_right();

}


/***************************************************************
函数名称 : void SPI_Flash_Write_Page(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 : SPI在一页(0~65535)内写入少于256个字节的数据， 在指定地址开始写入最大256字节的数据
输入参数 :
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::write_page(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
    writeEnable();                  //SET WEL
    cs->reset();
    spi->write(W25X_PageProgram);      //发送写页命令
    spi->write((uint8_t)((write_addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((write_addr) >> 8));
    spi->write((uint8_t)write_addr);
    spi->write(buf, num_to_write);
    cs->set();
    _waitBusy();					   //等待写入结束
}
/***************************************************************
函数名称 : void SPI_Flash_Write_NoCheck(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 :  无检验写SPI FLASH，必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!具有自动换页功能，
			在指定地址开始写入指定长度的数据,但是要确保地址不越界!
输入参数 :
			pBuffer:数据存储区
			WriteAddr:开始写入的地址(24bit)
			NumByteToWrite:要写入的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::write_no_check(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write)
{
    uint16_t pageremain;
    pageremain = 256 - write_addr % 256; //单页剩余的字节数
    if(num_to_write <= pageremain)pageremain = num_to_write; //不大于256个字节
    while(1)
    {
        write_page(buf, write_addr, pageremain);
        if(num_to_write == pageremain)break; //写入结束了
        else //NumByteToWrite>pageremain
        {
            buf += pageremain;
            write_addr += pageremain;

            num_to_write -= pageremain;			 //减去已经写入了的字节数
            if(num_to_write > 256)pageremain = 256; //一次可以写入256个字节
            else pageremain = num_to_write; 	 //不够256个字节了
        }
    };
}
/***************************************************************
函数名称 : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 : 擦除一个扇区，擦除一个山区的最少时间:150ms
输入参数 : Dst_Addr:扇区地址 0~511 for w25x16
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::erase_sector(uint32_t dst_addr)
{
    dst_addr *= 4096;
    writeEnable();                  //SET WEL
    _waitBusy();
    cs->reset();
    spi->write(W25X_SectorErase);      //发送扇区擦除指令
    spi->write((uint8_t)((dst_addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((dst_addr) >> 8));
    spi->write((uint8_t)dst_addr);
    cs->set();
    _waitBusy();   				   //等待擦除完成

}
/***************************************************************
函数名称 : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 :  擦除整个芯片
			整片擦除时间:
			W25X16:25s
			W25X32:40s
			W25X64:40s
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::erase_chip(void)
{
    writeEnable();                  //SET WEL
    _waitBusy();
    cs->reset();
    spi->write(W25X_ChipErase); 	//发送片擦除命令
    cs->set();
    _waitBusy();   				   				//等待芯片擦除结束
}





//等待空闲
void W25x16::_waitBusy(void)
{
    while ((readSR() & 0x01) == 0x01); // 等待BUSY位清空
}
//进入掉电模式
void W25x16::powerDown(void)
{
    volatile int i;
    cs->reset();
    spi->write(W25X_PowerDown);        //发送掉电命令
    //等待TPD
    for (i = 0; i < 300; i++);
    cs->set();

}
//唤醒
void W25x16::wakeUp(void)
{
    volatile int i;
    cs->reset();
    spi->write(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    //等待TRES1
    for (i = 0; i < 300; i++);
    cs->set();
}
/***************************************************************
函数名称 :  uint8_t SPI_Flash_ReadSR(void)
功能描述 : 读取W25X16的状态寄存器
				BIT7  6   5   4   3   2   1   0
				SPR   RV  TB BP2 BP1 BP0 WEL BUSY
				说明：
				SPR:默认0,状态寄存器保护位,配合WP使用
				TB,BP2,BP1,BP0:FLASH区域写保护设置
				WEL:写使能锁定
				BUSY:忙标记位(1,忙;0,空闲)
				默认:0x00
输入参数 : 无
输出参数 : 无
返回值   : byte	寄存器状态
***************************************************************/
uint8_t W25x16::readSR(void)
{
    uint8_t byte = 0;
    cs->reset();
    spi->write(W25X_ReadStatusReg);    //发送读取状态寄存器命令
    byte = spi->read();
    cs->set();
    return byte;
}

/***************************************************************
函数名称 :  void SPI_FLASH_Write_SR(uint8_t sr)
功能描述 : 写SPI_FLASH状态寄存器， 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
输入参数 : sr:状态
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::writeSR(uint8_t sr)
{
    cs->reset();
    spi->write(W25X_WriteStatusReg);   //发送写取状态寄存器命令
    spi->write(sr);               //写入一个字节
    cs->set();
}


/***************************************************************
函数名称 :  void SPI_FLASH_Write_Enable(void)
功能描述 : SPI_FLASH写使能，将WEL置位
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::writeEnable(void)
{
    cs->reset();
    spi->write(W25X_WriteEnable);      //发送写使能
    cs->set();
}
/***************************************************************
函数名称 :  void SPI_FLASH_Write_Disable(void)
功能描述 : SPI_FLASH写禁止， 将WEL清零
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::writeDisable(void)
{
    cs->reset();
    spi->write(W25X_WriteDisable);     //发送写禁止指令
    cs->set();
}
