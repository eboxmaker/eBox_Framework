/**
  ******************************************************************************
  * @file    fm25v.cpp
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
    uint8_t pp[2];
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


//�ȴ�����
void FM25V::_waitBusy(void)   
{   
	while ((readSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void FM25V::powerDown(void)   
{ 
		
	cs->reset();
    spi->write(SLEEP);        //���͵�������  
    //�ȴ�TPD  
//	delay_ms(1);
	cs->set();

}   
//����
void FM25V::wakeUp(void)   
{  
 	volatile int i;	 	
 	cs->reset();
 
   	delay_us(500);
	cs->set();
}  

/***************************************************************
�������� :  void SPI_Flash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
�������� : ��ȡSPI FLASH�� ��ָ����ַ��ʼ��ȡָ�����ȵ�����
������� : 
			pBuffer:���ݴ洢��
			ReadAddr:��ʼ��ȡ�ĵ�ַ(15bit)
			NumByteToRead:Ҫ��ȡ���ֽ���(���32767)
������� : ��
����ֵ   : ��
***************************************************************/
void FM25V::read(uint8_t *buf,uint32_t read_addr,uint16_t num_to_read)
{
    
     spi->take_spi_right(&spi_dev_FM25V );  
     cs->reset(); 
     spi->write(READ);
	 spi->write((uint8_t)((read_addr)>>16));    
	 spi->write((uint8_t)((read_addr)>>8));   
	 spi->write((uint8_t)read_addr);   
     spi->read(buf,num_to_read);
     cs->set(); 
     spi->release_spi_right();
}
/***************************************************************
�������� :  void SPI_Flash_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
�������� : ��ȡSPI FLASH�� ��ָ����ַ��ʼ��ȡָ�����ȵ�����
������� : 
			pBuffer:���ݴ洢��
			ReadAddr:��ʼ��ȡ�ĵ�ַ(15bit)
			NumByteToRead:Ҫ��ȡ���ֽ���(���32767)
������� : ��
����ֵ   : ��
***************************************************************/
void FM25V::fast_read(uint8_t *buf,uint32_t read_addr,uint16_t num_to_read)   
{ 
	 spi->take_spi_right(&spi_dev_FM25V);
	 cs->reset();
	 spi->write(FSTRD);           
	 spi->write((uint8_t)((read_addr)>>16));      
	 spi->write((uint8_t)((read_addr)>>8));   
	 spi->write((uint8_t)read_addr);   
	 spi->write(0xff);   
	 spi->read(buf,num_to_read);
	 cs->set();
	 spi->release_spi_right();

} 


/***************************************************************
�������� : void SPI_Flash_Write(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)     
�������� : дSPI FLASH����ָ����ַ��ʼд��ָ�����ȵ����ݣ��ú�������������!
������� : 
			pBuffer:���ݴ洢��
			WriteAddr:��ʼд��ĵ�ַ(15bit)
			NumByteToWrite:Ҫд����ֽ���(���32767)  		   
������� : ��
����ֵ   : ��
***************************************************************/
void FM25V::write(uint8_t *buf,uint32_t write_addr,uint16_t num_to_write)   
{ 
	 spi->take_spi_right(&spi_dev_FM25V);
     FM25V::writeEnable();
     cs->reset();
	 spi->write(WRITE);           
	 spi->write((uint8_t)((write_addr)>>16));    
	 spi->write((uint8_t)((write_addr)>>8));   
	 spi->write((uint8_t)write_addr); 
	 spi->write(buf,num_to_write);
	 cs->set();    
     FM25V::writeEnable();
	 spi->release_spi_right();
	
}

/***************************************************************
�������� :  uint8_t SPI_Flash_ReadSR(void)   
�������� : ��ȡW25X16��״̬�Ĵ���
				BIT7  6   5   4   3   2   1   0
				WREN  0   0   0  BP2 BP1 WEL  0
				˵����
				WREN:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
				BP2,BP1:FLASH����д��������
				WEL:дʹ������flag				
				Ĭ��:0x00
������� : ��
������� : ��
����ֵ   : byte	�Ĵ���״̬
***************************************************************/
uint8_t FM25V::readSR(void)  
{  
	uint8_t byte=0;   
    spi->take_spi_right(&spi_dev_FM25V);
	cs->reset();
	 spi->write(RDSR);      
	byte= spi->read();               
	cs->set();
    spi->release_spi_right(); 
	return byte;   
   
} 

/***************************************************************
�������� :  void SPI_FLASH_Write_SR(uint8_t sr)   
�������� : дSPI_FLASH״̬�Ĵ����� ֻ��WREN BP2 BP1 (bit 7,4,3)����д!!!
������� : sr:״̬
������� : ��
����ֵ   : ��
***************************************************************/
void FM25V::writeSR(uint8_t sr)   
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
�������� : void FM25V::writeEnable(void)     
�������� : SPI_FLASHдʹ�ܣ���WEL��λ
������� : ��
������� : ��
����ֵ   : ��
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
�������� :  void FM25V::writeDisable(void)     
�������� : SPI_FLASHд��ֹ�� ��WEL���� 
������� : ��
������� : ��
����ֵ   : ��
***************************************************************/
void FM25V::writeDisable(void)   
{  
    spi->take_spi_right(&spi_dev_FM25V);
 	cs->reset();
    spi->write(WRDI);        
	cs->set();
    spi->release_spi_right(); 
} 


