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

W25xInfo_t w25x16_info = 
{
    256,
    8192,
    4096,
};

int W25x16::begin()
{

    if(initialized == 0)
    {
        cfg.dev_num = cs->id;
        cfg.mode = Spi::MODE0;
        cfg.bit_order = Spi::MSB;
        cfg.prescaler = Spi::DIV4;
    }
    cs->mode(OUTPUT_PP);
    cs->set();
    
    spi->begin(&cfg);
    
    capacity = w25x16_info.page_size * w25x16_info.page_count;
    sector_size = w25x16_info.sector_size;
    sector_count = capacity / sector_size;
    
    ebox_printf("page_size: %d\r\n",w25x16_info.page_size);
    ebox_printf("page_count: %d\r\n",w25x16_info.page_count);
    ebox_printf("cap: %d\r\n",capacity);
    ebox_printf("sector_size:  %d\r\n",sector_size);
    ebox_printf("sector_count:  %d\r\n",sector_count);
    
	type = read_id();	          //读取FLASH ID.

    
   
}
int W25x16::init()
{
    uint8_t temp;
    
    
//    while(1){
    
	type = read_id();	          //读取FLASH ID.
    
//    temp=read_sr(3);              //读取状态寄存器3，判断地址模式
//    if((temp&0X01)==0)			        //如果不是4字节地址模式,则进入4字节地址模式
//    {
//        cs->reset();
//        spi->write(W25X_Enable4ByteAddr);//发送进入4字节地址模式指令   
//        cs->set();       		       
//    }
    ebox_printf("w25x16 init ok: type:0X%04X\r\n",type);
    delay_ms(1000);
//    }

}
uint16_t W25x16::read_id()
{
    uint16_t id = 0;
    spi->take(&cfg);

    cs->reset();
    spi->write(0x90);
    spi->write(0x00);
    spi->write(0x00);
    spi->write(0x00);
    id |= spi->read() << 8;
    id |= spi->read();
    cs->set();
    spi->release();
    
    return id;
}


/***************************************************************
函数名称 :  int read_sector(uint8_t *buffer, uint32_t sector, uint8_t count)
功能描述 : 读取SPI FLASH， 按照sector去读取
输入参数 :
			buffer:数据存储区
			sector:开始读取的地址(24bit)
			count:要读取的字节数(最大65535)
输出参数 : 无
返回值   : 无
***************************************************************/
int W25x16:: read_sector(uint8_t *buffer, uint32_t sector, uint8_t count)
{
    uint32_t addr = sector * sector_size;
    uint16_t read_count = sector_size  * count;
    spi->take(&cfg);
    cs->reset();
    spi->write(W25X_ReadData);         //发送读取命令
//    if(type == W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
//    {
//        spi->write((uint8_t)((addr)>>24));    
//    }
    spi->write((uint8_t)((addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((addr) >> 8));
    spi->write((uint8_t)addr);
    spi->read_buf(buffer, read_count);
    cs->set();
    spi->release();
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
    spi->take(&cfg);
    cs->reset();
    spi->write(W25X_ReadData);         //发送读取命令
    spi->write((uint8_t)((read_addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((read_addr) >> 8));
    spi->write((uint8_t)read_addr);
    spi->read_buf(buf, num_to_read);
    cs->set();
    spi->release();

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
int W25x16::write_sector(uint8_t *data, uint32_t sector, uint8_t count)
{
    uint32_t addr = sector * sector_size;
    uint16_t write_count = sector_size * count;
    
    for(int i = 0; i < count; i++)
    {
        erase_sector(sector + i);
        for(int j = 0; j < sector_size / 256; j++)
        {
            write_page(&data[j*256 + i*4096], addr+j*256 + i*4096, 256);
        }

    }
    
    

//    write_no_check(data,addr,write_count);
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
    spi->take(&cfg);
    cs->reset();
    spi->write(W25X_FastReadData);         //发送读取命令
    spi->write((uint8_t)((read_addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((read_addr) >> 8));
    spi->write((uint8_t)read_addr);
    spi->write(0xff);   //空字节
    spi->read_buf(buf, num_to_read);
    cs->set();
    spi->release();

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
    write_enable();                  //SET WEL
    spi->take(&cfg);
    cs->reset();
    spi->write(W25X_PageProgram);      //发送写页命令
    if(type == W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        spi->write((uint8_t)((write_addr)>>24));    
    }
    spi->write((uint8_t)((write_addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((write_addr) >> 8));
    spi->write((uint8_t)write_addr);
    spi->write_buf(buf, num_to_write);
    cs->set();
    wait_busy();					   //等待写入结束
    spi->release();
}
/***************************************************************
函数名称 : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
功能描述 : 擦除一个扇区，擦除一个山区的最少时间:150ms
输入参数 : Dst_Addr:扇区地址 0~511 for w25x16
输出参数 : 无
返回值   : 无
***************************************************************/
int W25x16::erase_sector(uint32_t dst_addr)
{
    dst_addr *= 4096;
    write_enable();                  //SET WEL
    wait_busy();
    cs->reset();
    spi->write(W25X_SectorErase);      //发送扇区擦除指令
    spi->write((uint8_t)((dst_addr) >> 16)); //发送24bit地址
    spi->write((uint8_t)((dst_addr) >> 8));
    spi->write((uint8_t)dst_addr);
    cs->set();
    wait_busy();   				   //等待擦除完成

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
    write_enable();                  //SET WEL
    wait_busy();
    cs->reset();
    spi->write(W25X_ChipErase); 	//发送片擦除命令
    cs->set();
    wait_busy();   				   				//等待芯片擦除结束
}





//等待空闲
void W25x16::wait_busy(void)
{
    while ((read_sr() & 0x01) == 0x01); // 等待BUSY位清空
}
//进入掉电模式
void W25x16::power_down(void)
{
    volatile int i;
    cs->reset();
    spi->write(W25X_PowerDown);        //发送掉电命令
    //等待TPD
    for (i = 0; i < 300; i++);
    cs->set();

}
//唤醒
void W25x16::wake_up(void)
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

            //状态寄存器1：
            BIT7  6   5   4   3   2   1   0
            SPR   RV  TB BP2 BP1 BP0 WEL BUSY
            说明：
            SPR:默认0,状态寄存器保护位,配合WP使用
            TB,BP2,BP1,BP0:FLASH区域写保护设置
            WEL:写使能锁定
            BUSY:忙标记位(1,忙;0,空闲)
            默认:0x00
            //状态寄存器2：
            BIT7  6   5   4   3   2   1   0
            SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
            //状态寄存器3：
            BIT7      6    5    4   3   2   1   0
            HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
输入参数 : 状态寄存器号，范:1~3
返回值   : byte	寄存器状态
***************************************************************/
uint8_t W25x16::read_sr(uint8_t index)
{
    uint8_t byte=0,command=0; 
    switch(index)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //读状态寄存器1指令
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //读状态寄存器2指令
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //读状态寄存器3指令
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }  
    cs->reset();
    spi->write(command);    //发送读取状态寄存器命令
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
void W25x16::write_sr(uint8_t index,uint8_t value)
{
    uint8_t byte=0,command=0; 
    switch(index)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //写状态寄存器1指令
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //写状态寄存器2指令
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //写状态寄存器3指令
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    } 
    cs->reset();
    spi->write(command);   //发送写取状态寄存器命令
    spi->write(value);               //写入一个字节
    cs->set();
}


/***************************************************************
函数名称 :  void SPI_FLASH_Write_Enable(void)
功能描述 : SPI_FLASH写使能，将WEL置位
输入参数 : 无
输出参数 : 无
返回值   : 无
***************************************************************/
void W25x16::write_enable(void)
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
void W25x16::write_disable(void)
{
    cs->reset();
    spi->write(W25X_WriteDisable);     //发送写禁止指令
    cs->set();
}
uint16_t W25x16::get_type(void)
{
    return type;
}
