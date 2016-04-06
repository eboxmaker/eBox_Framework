/*+++++++++++++++++++++++++++++++++AT45DB321D++++++++++++++++++++++++++++++++++*/

#include "at45db.h"

void AT45DB::begin(uint8_t dev_num)
{
	spi_dev_AT45DB.dev_num = dev_num;
	spi_dev_AT45DB.mode = SPI_MODE0;
	spi_dev_AT45DB.prescaler = SPI_BaudRatePrescaler_4;
	spi_dev_AT45DB.bit_order = SPI_BITODER_MSB;
	
	spi->begin(&spi_dev_AT45DB);
	cs->mode(OUTPUT_PP);
	cs->set();    
}
void AT45DB::read_id (uint16_t *ID)  
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
��������:��ָ����λ�ÿ�ʼ�����������ݣ�ֱ��CS�źŵ�������
�������룺u16 page_addr:ҳ��ַ��0~4095��
          u16 byte_addr:�ֽڵ�ַ��0~527��
          u8 *buf:���ڴ�Ŷ���������
          unsigned long ByteNum:�������������ֽ��� 
***********************************************/
void AT45DB::read (u8 *buf,u16 page_addr,u16 byte_addr,unsigned long num_to_read)
{
//  unsigned long i;
  if(num_to_read<=2162688)//����ȡ��������Ƭ�� ,��Χ��4096*528 Bytes
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
���������������ȴ��䵽�������ٴ��������е�ָ��ҳ�����ڲ��߳���
�������룺unsigned char BufferNumberѡ�񻺳�����1��2��
          unsigned char PageAddr������ָ��ҳ(0~4095)
          unsigned char ByteAddrҳ��ָ���ֽڵ�ַ(0~527��
          unsigned char *Data��д�������
          unsigned char ByteNumд�������ֽ�����1~528��
***********************************************/
void AT45DB::DataToPage_ViaBuffer_WithErase_OneWay(unsigned char BufferNumber,
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

void AT45DB::DataToPage_ViaBuffer_WithErase_SecWay(unsigned char BufferNumber,unsigned int PageAddr,unsigned int ByteAddr,
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
����������������д�뵽��������
�������룺unsigned char BufferNumberѡ�񻺳�����������1��2��
          unsigned int ByteAddr��������д�����ݵ���ʼ��ַ(0~527)
          unsigned char *Data��д�������
          unsigned int ByteNumд������ֽ���(1~528)

***********************************************/
void AT45DB::Buffer_Write(unsigned char BufferNumber,unsigned int ByteAddr,
                          unsigned char *Data,unsigned int ByteNum) 
{
//  unsigned int i;
  if(((528-ByteAddr)>=ByteNum)&&(ByteNum>0))//д����������ڻ�����ʣ��ķ�Χ��
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
������������Ԥ�߳��Ļ�����ȫ�����ݿ�����������ָ��ҳ(û�б��߳���
�������룺unsigned char BufferNumberѡ�񻺳�����1��2��
          unsigned char PageAddr������ҳ��ַ(0~4095)

***********************************************/
void AT45DB::buffer_MMPage_WithErase(unsigned char BufferNumber,unsigned int PageAddr)
{
  if(PageAddr<4096)    
  {
    spi->take_spi_right (&spi_dev_AT45DB);
    AT45DB::_waitBusy();
    AT45DB::cs ->reset();
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
�������� :  u8 SPI_Flash_ReadSR(void)   
�������� : ��ȡW25X16��״̬�Ĵ���
				BIT7  6   5   4   3   2   1   0
		   RDY/BUSY COMP  device density  X   X
				˵����
				SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
				TB,BP2,BP1,BP0:FLASH����д��������
				WEL:дʹ������
				BUSY:æ���λ(0,æ;1,����)
				Ĭ��:0x00
������� : ��
������� : ��
����ֵ   : byte	�Ĵ���״̬
***************************************************************/
u8 AT45DB::readSR(void)   
{  
	u8 byte=0;  
    spi->take_spi_right (&spi_dev_AT45DB);  
	cs->reset();
	spi->write(AT45DBX_CMDC_RD_STATUS_REG);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte= spi->read();               
	cs->set();
    spi->release_spi_right() ;
	return byte;   
    
}

//�ȴ�����
void AT45DB::_waitBusy(void)    
{      
    uint16_t i = 0;
    while (i++ < 10000)
    {
        if (AT45DB::readSR()& 0x80) 
        {
            return;
        }
        DBG("\r\nSPI_FLASH timeout!!!\r\n");
    }
   

}



