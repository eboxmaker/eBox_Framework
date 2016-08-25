/**
  ******************************************************************************
  * @file    at45db.cpp
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

#include "At45db.h"

void At45db::begin(uint8_t dev_num)
{
	spi_dev_AT45DB.dev_num = dev_num;
	spi_dev_AT45DB.mode = SPI_MODE0;
	spi_dev_AT45DB.prescaler = SPI_BaudRatePrescaler_4;
	spi_dev_AT45DB.bit_order = SPI_BITODER_MSB;
	
	spi->begin(&spi_dev_AT45DB);
	cs->mode(OUTPUT_PP);
	cs->set();    
}
void At45db::read_id (uint16_t *ID)  
{
    *ID=0;
    spi->take_spi_right (&spi_dev_AT45DB);
    _waitBusy();
    cs->reset();
    spi->write (0x9F);  
    *ID=(*ID|spi->read())<<8; //0x1f Atmel
    *ID|=spi->read();      //0x27  32M
    spi->read(); 
    spi->read(); 
    cs->set();
    spi->release_spi_right(); 
}

/**********************************************
功能描述:从指定的位置开始连续读出数据，直到CS信号的上升沿
参数输入：uint16_t page_addr:页地址（0~4095）
          uint16_t byte_addr:字节地址（0~527）
          uint8_t *buf:用于存放读出的数据
          unsigned long ByteNum:欲读出的数据字节数 
***********************************************/
void At45db::read (uint8_t *buf,uint16_t page_addr,uint16_t byte_addr,unsigned long num_to_read)
{
//  unsigned long i;
  if(num_to_read<=2162688)//欲读取的数据在片内 ,范围：4096*528 Bytes
  {
     spi->take_spi_right (&spi_dev_AT45DB);
    _waitBusy();
    cs->reset();  
   
    spi->write (AT45DBX_CMDA_RD_ARRAY_AF_SM);
    spi->write((unsigned char)(page_addr>>6));
    spi->write((unsigned char)((page_addr<<2)|(byte_addr>>8)));
    spi->write ((unsigned char)byte_addr);
    spi->write (0x00); 
    
    spi->read(buf,num_to_read);  
    cs->set();    
    spi->release_spi_right();   
  }
 
}



/**********************************************
功能描述：数据先传输到缓冲器再传到主存中的指定页（带内部檫除）
参数输入：unsigned char BufferNumber选择缓冲器（1或2）
          unsigned char PageAddr主存中指定页(0~4095)
          unsigned char ByteAddr页中指定字节地址(0~527）
          unsigned char *Data欲写入的数据
          unsigned char ByteNum写入数据字节数（1~528）
***********************************************/
void At45db::DataToPage_ViaBuffer_WithErase_OneWay(unsigned char BufferNumber,
                                           unsigned int PageAddr,unsigned int ByteAddr,
                                           unsigned char *Data,unsigned int ByteNum)
{
    spi->take_spi_right (&spi_dev_AT45DB );
    _waitBusy();
  if((ByteNum<=(528-ByteAddr))&&(ByteNum>0))
  {        
    Buffer_Write(BufferNumber,ByteAddr,Data,ByteNum);
    buffer_MMPage_WithErase(BufferNumber,PageAddr); 
  } 
  spi->release_spi_right();
}

void At45db::DataToPage_ViaBuffer_WithErase_SecWay(unsigned char BufferNumber,unsigned int PageAddr,unsigned int ByteAddr,
                                                   unsigned char *Data,unsigned int ByteNum)
 {
//   unsigned int i;
   if((ByteNum<=(528-ByteAddr))&&(ByteNum>0))
   {
      spi->take_spi_right (&spi_dev_AT45DB );
      _waitBusy();
      cs->reset();
          if(BufferNumber==1)
    {
        spi->write(AT45DBX_CMDB_PR_PAGE_TH_BUF1);
        
    }
    else if (BufferNumber==2)
    {
        spi->write(AT45DBX_CMDB_PR_PAGE_TH_BUF2);
    }
      spi->write ((unsigned char)(PageAddr>>6));
      spi->write((unsigned char)((PageAddr<<2)|(ByteAddr>>8)));
      spi->write((unsigned char)ByteAddr);
      spi->write(Data,ByteNum);    
      cs->set();
      spi->release_spi_right();
  }
   
}


/**********************************************
功能描述：将数据写入到缓冲器中
参数输入：unsigned char BufferNumber选择缓冲器缓冲器（1或2）
          unsigned int ByteAddr缓冲器中写入数据的起始地址(0~527)
          unsigned char *Data需写入的数据
          unsigned int ByteNum写入的总字节数(1~528)

***********************************************/
void At45db::Buffer_Write(unsigned char BufferNumber,unsigned int ByteAddr,
                          unsigned char *Data,unsigned int ByteNum) 
{
//  unsigned int i;
  if(((528-ByteAddr)>=ByteNum)&&(ByteNum>0))//写入的数据量在缓冲器剩余的范围内
  {
    spi->take_spi_right(&spi_dev_AT45DB );
    _waitBusy();
    cs->reset();
    if(BufferNumber==1)
    {
        spi->write(AT45DBX_CMDC_WR_BUF1);
        
    }
    else if (BufferNumber==2)
    {
        spi->write(AT45DBX_CMDC_WR_BUF2);
    }
    spi->write(0x00);
    spi->write((unsigned char)(ByteAddr>>8));
    spi->write ((unsigned char)ByteAddr);
    spi->write (Data,ByteNum);
    cs->set();
    spi->release_spi_right();
  }   
}
/**********************************************
功能描述：带预檫除的缓冲器全部数据拷贝到主存中指定页(没有被檫除）
参数输入：unsigned char BufferNumber选择缓冲器（1或2）
          unsigned char PageAddr主存中页地址(0~4095)

***********************************************/
void At45db::buffer_MMPage_WithErase(unsigned char BufferNumber,unsigned int PageAddr)
{
  if(PageAddr<4096)    
  {
    spi->take_spi_right (&spi_dev_AT45DB);
    At45db::_waitBusy();
    At45db::cs ->reset();
    if(BufferNumber==1)
    {
        spi->write(AT45DBX_CMDB_PR_BUF1_TO_PAGE_ER);
        
    }
    else if (BufferNumber==2)
    {
        spi->write(AT45DBX_CMDB_PR_BUF2_TO_PAGE_ER);
    }
    spi->write((unsigned char)(PageAddr>>6));
    spi->write((unsigned char)(PageAddr<<2));
    spi->write(0x00);
    cs->set();     
  }  
  spi->release_spi_right();
}



/***************************************************************
函数名称 :  uint8_t SPI_Flash_ReadSR(void)   
功能描述 : 读取W25X16的状态寄存器
				BIT7  6   5   4   3   2   1   0
		   RDY/BUSY COMP  device density  X   X
				说明：
				SPR:默认0,状态寄存器保护位,配合WP使用
				TB,BP2,BP1,BP0:FLASH区域写保护设置
				WEL:写使能锁定
				BUSY:忙标记位(0,忙;1,空闲)
				默认:0x00
输入参数 : 无
输出参数 : 无
返回值   : byte	寄存器状态
***************************************************************/
uint8_t At45db::readSR(void)   
{  
	uint8_t byte=0;  
    spi->take_spi_right (&spi_dev_AT45DB);  
	cs->reset();
	spi->write(AT45DBX_CMDC_RD_STATUS_REG);    //发送读取状态寄存器命令    
	byte= spi->read();               
	cs->set();
    spi->release_spi_right() ;
	return byte;   
    
}

//等待空闲
void At45db::_waitBusy(void)    
{      
    uint16_t i = 0;
    while (i++ < 10000)
    {
        if (At45db::readSR()& 0x80) 
        {
            return;
        }
        DBG("\r\nSPI_FLASH timeout!!!\r\n");
    }
   

}



