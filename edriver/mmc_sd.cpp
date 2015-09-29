/*
file   : mmc_sd.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "mmc_sd.h"


int SD::begin(uint8_t dev_num)
{
	int ret = 0;
	SPIDevSDCard.devNum = dev_num;
	SPIDevSDCard.mode = SPI_MODE0;
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	SPIDevSDCard.bit_order = SPI_BITODER_MSB;
		
	cs->mode(OUTPUT_PP);
	cs->set();
	SD_Type = 0;
	
	ret = init();
	return ret;
}
/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : �ȴ�SD��Ready
* Input          : None
* Output         : None
* Return         : u8 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/

uint8_t SD::_wait(void)
{
  u8 r1;
  u16 retry=0;
  do
  {
    r1 = spi->read();
    retry++;
    if(retry==0xfffe)
      return 1; 
  }while(r1!=0xFF); 
	return 0;
}
/*******************************************************************************
* Function Name  : SD_SendCommand
* Description    : ��SD������һ������
* Input          : u8 cmd   ���� 
*                  u32 arg  �������
*                  u8 crc   crcУ��ֵ
* Output         : None
* Return         : u8 r1 SD�����ص���Ӧ
*******************************************************************************/
uint8_t SD::_sendCommand(u8 cmd, u32 arg,u8 crc)
{
  unsigned char r1;
  unsigned int Retry = 0;

 
	cs->set();
	spi->write(0xff);//��߼����ԣ����û�������ЩSD�����ܲ�֧��
	cs->reset();//Ƭѡ���õͣ�ѡ��SD��
	
  /* ������������ */
  spi->write(cmd | 0x40);                      
  spi->write((u8)(arg >> 24));//����[31..24]
  spi->write((u8)(arg >> 16));//����[23..16]
  spi->write((u8)(arg >> 8));//����[15..8]
  spi->write((u8)arg);    //����[7..0]
  spi->write(crc);
    
  //�ȴ���Ӧ����ʱ�˳�
  while((r1 = spi->read())==0xFF)
  {
    Retry++;
    if(Retry > 800)break; //����ʵ���ã�������Դ������
  }   
  //�ر�Ƭѡ
	cs->set();
  //�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
  spi->write(0xFF);

  //����״ֵ̬
  return r1;
}
/*******************************************************************************
* Function Name  : SD_SendCommand_NoDeassert
* Description    : ��SD������һ������(�����ǲ�ʧ��Ƭѡ�����к������ݴ�����
* Input          : u8 cmd   ���� 
*                  u32 arg  �������
*                  u8 crc   crcУ��ֵ
* Output         : None
* Return         : u8 r1 SD�����ص���Ӧ
*******************************************************************************/
uint8_t SD::_sendCommandNoDeassert(u8 cmd, u32 arg,u8 crc)
{
  unsigned char r1;
  unsigned int Retry = 0;
	

	cs->set();
	spi->write(0xff);//��߼����ԣ����û�������ЩSD�����ܲ�֧��
	cs->reset();//Ƭѡ���õͣ�ѡ��SD��
  
  /* ������������ */
  spi->write(cmd | 0x40);                      
  spi->write((u8)(arg >> 24));//����[31..24]
  spi->write((u8)(arg >> 16));//����[23..16]
  spi->write((u8)(arg >> 8));//����[15..8]
  spi->write((u8)arg);    //����[7..0]
  spi->write(crc);
  
  //�ȴ���Ӧ����ʱ�˳�
  while((r1 = spi->read())==0xFF)
  {
    Retry++;
    if(Retry > 600)break; //����ʵ���ã�������Դ������  
  }
  //������Ӧֵ
  return r1;
}
/*******************************************************************************
* Function Name  : SD_Init
* Description    : ��ʼ��SD��
* Input          : None
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  99��NO_CARD
*******************************************************************************/
uint8_t SD::init()
{
  u16 i;      // ����ѭ������
  u8 r1;      // ���SD���ķ���ֵ
  u16 retry;  // �������г�ʱ����
  u8 buff[6];
	
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV256;
	spi->begin(&SPIDevSDCard);
	cs->reset();
	  // ����ʱ���ȴ�SD���ϵ����
  for(i=0;i<0xf00;i++);

  //�Ȳ�������74�����壬��SD���Լ���ʼ�����
  for(i=0;i<10;i++)
  {
    spi->write(0xFF);//80clks
  }
	  //-----------------SD����λ��idle��ʼ-----------------
  //ѭ����������CMD0��ֱ��SD������0x01,����IDLE״̬
  //��ʱ��ֱ���˳�
  retry = 0;
  do
  {
    //����CMD0����SD������IDLE״̬
    r1 = _sendCommand(CMD0, 0,0x95);
    retry++;
  }while((r1 != 0x01) && (retry<200));
  //����ѭ���󣬼��ԭ�򣺳�ʼ���ɹ���or ���Գ�ʱ��
  if(retry==200) return 1;   //��ʱ����1	
	
	
  //-----------------SD����λ��idle����-----------------	 
  //��ȡ��Ƭ��SD�汾��Ϣ
  r1 = _sendCommandNoDeassert(CMD8, 0x1aa,0x87);	     
    //�����Ƭ�汾��Ϣ��v1.0�汾�ģ���r1=0x05����������³�ʼ��
  if(r1 == 0x05)
  {
     //���ÿ�����ΪSDV1.0����������⵽ΪMMC�������޸�ΪMMC
     SD_Type = SD_TYPE_V1;	   
     //�����V1.0����CMD8ָ���û�к�������
     //Ƭѡ�øߣ�������������
     cs->set();
     //�෢8��CLK����SD������������
     spi->write(0xFF);	  
     //-----------------SD����MMC����ʼ����ʼ-----------------	 
     //������ʼ��ָ��CMD55+ACMD41
     // �����Ӧ��˵����SD�����ҳ�ʼ�����
     // û�л�Ӧ��˵����MMC�������������Ӧ��ʼ��
     retry = 0;
     do
     {
       //�ȷ�CMD55��Ӧ����0x01���������
       r1 = _sendCommand(CMD55, 0, 0);
       if(r1 != 0x01)
         return r1;	  
       //�õ���ȷ��Ӧ�󣬷�ACMD41��Ӧ�õ�����ֵ0x00����������400��
       r1 = _sendCommand(ACMD41, 0, 0);
       retry++;
     }while((r1!=0x00) && (retry<400));
     // �ж��ǳ�ʱ���ǵõ���ȷ��Ӧ
     // ���л�Ӧ����SD����û�л�Ӧ����MMC��
        
     //----------MMC�������ʼ��������ʼ------------
     if(retry==400)
     {
        retry = 0;
        //����MMC����ʼ�����û�в��ԣ�
        do
        {
           r1 = _sendCommand(CMD1, 0, 0);
           retry++;
        }while((r1!=0x00)&& (retry<400));
        if(retry==400)return 1;   //MMC����ʼ����ʱ		    
        //д�뿨����
        SD_Type = SD_TYPE_MMC;
     }
     //----------MMC�������ʼ����������------------	    
     //����SPIΪ����ģʽ
		 SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
		 spi->config(&SPIDevSDCard);

     spi->write(0xFF);
        
     //��ֹCRCУ��	   
     r1 = _sendCommand(CMD59, 0, 0x95);
     if(r1 != 0x00)return r1;  //������󣬷���r1   	   
     //����Sector Size
     r1 = _sendCommand(CMD16, 512, 0x95);
     if(r1 != 0x00)return r1;//������󣬷���r1		 
     //-----------------SD����MMC����ʼ������-----------------

  }//SD��ΪV1.0�汾�ĳ�ʼ������	 
  //������V2.0���ĳ�ʼ��
  //������Ҫ��ȡOCR���ݣ��ж���SD2.0����SD2.0HC��
  else if(r1 == 0x01)
  {
    //V2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ����ݣ�Ҫ�����ٽ���������
    buff[0] = spi->read();  //should be 0x00
    buff[1] = spi->read();  //should be 0x00
    buff[2] = spi->read();  //should be 0x01
    buff[3] = spi->read();  //should be 0xAA	   
		cs->set();
		spi->write(0xFF);//the next 8 clocks			 
    //�жϸÿ��Ƿ�֧��2.7V-3.6V�ĵ�ѹ��Χ
    //if(buff[2]==0x01 && buff[3]==0xAA) //�粻�жϣ�����֧�ֵĿ�����
   // {	  
       retry = 0;
       //������ʼ��ָ��CMD55+ACMD41
       do
       {
    	  r1 = _sendCommand(CMD55, 0, 0);
    	  if(r1!=0x01)return r1;	   
    	  r1 = _sendCommand(ACMD41, 0x40000000, 1);
          if(retry>200)return r1;  //��ʱ�򷵻�r1״̬  
       }while(r1!=0);		  
       //��ʼ��ָ�����ɣ���������ȡOCR��Ϣ		   
       //-----------����SD2.0���汾��ʼ-----------
       r1 = _sendCommandNoDeassert(CMD58, 0, 0);
       if(r1!=0x00)return r1;  //�������û�з�����ȷӦ��ֱ���˳�������Ӧ��		 
       //��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
       buff[0] = spi->read();
       buff[1] = spi->read(); 
       buff[2] = spi->read();
       buff[3] = spi->read();

       //OCR������ɣ�Ƭѡ�ø�
			 cs->set();
       spi->write(0xFF);

       //�����յ���OCR�е�bit30λ��CCS����ȷ����ΪSD2.0����SDHC
       //���CCS=1��SDHC   CCS=0��SD2.0
       if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC;    //���CCS	 
       else SD_Type = SD_TYPE_V2;	    
       //-----------����SD2.0���汾����----------- 
       //����SPIΪ����ģʽ
			 SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
			spi->config(&SPIDevSDCard);
			
   // }	    
  }
    return r1;
}
/*******************************************************************************
* Function Name  : SD_ReceiveData
* Description    : ��SD���ж���ָ�����ȵ����ݣ������ڸ���λ��
* Input          : u8 *data(��Ŷ������ݵ��ڴ�>len)
*                  u16 len(���ݳ��ȣ�
*                  u8 release(������ɺ��Ƿ��ͷ�����CS�ø� 0�����ͷ� 1���ͷţ�
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  other��������Ϣ
*******************************************************************************/
int SD::_receiveData(u8 *data, u16 len, u8 release)
{
  u16 retry;
  u8 r1;
  // ����һ�δ���
  cs->reset();
  //�ȴ�SD������������ʼ����0xFE
  retry = 0;										   
  do
  {
    r1 = spi->read();
    retry++;
    if(retry>4000)  //4000�εȴ���û��Ӧ���˳�����(����ʵ����ԣ��˴���ö��Լ���
    {
      cs->set();
        return 1;
    }
    }while(r1 != 0xFE);
		   
    //��ʼ��������
    while(len--)
    {
      *data = spi->read();
      data++;
    }
    //������2��αCRC��dummy CRC��
    spi->write(0xFF);
    spi->write(0xFF);
    //�����ͷ����ߣ���CS�ø�
    if(release == RELEASE)
    {
      //�������
      cs->set();
      spi->write(0xFF);
    }											  					    
    return 0;
}
/*******************************************************************************
* Function Name  : SD_GetCID
* Description    : ��ȡSD����CID��Ϣ��������������Ϣ
* Input          : u8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
int SD::getCID(u8 *cid_data)
{
  u8 r1;
	
	spi->take_spi_right(&SPIDevSDCard);

  //��CMD10�����CID
  r1 = _sendCommand(CMD10, 0, 0xFF);
  if(r1 != 0x00)return r1;  //û������ȷӦ�����˳�������    
  //����16���ֽڵ�����
  _receiveData(cid_data, 16, RELEASE);	
	spi->release_spi_right();	
  return 0;
}
/*******************************************************************************
* Function Name  : SD_GetCSD
* Description    : ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
* Input          : u8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : u8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
int SD::getCSD(u8 *csd_data)
{
  u8 r1;

	spi->take_spi_right(&SPIDevSDCard);
  //��CMD9�����CSD
  r1 = _sendCommand(CMD9, 0, 0xFF);
  if(r1 != 0x00)return r1;  //û������ȷӦ�����˳�������  
  //����16���ֽڵ�����
  _receiveData(csd_data, 16, RELEASE);
	spi->release_spi_right();	

  return 0;
}
/*******************************************************************************
* Function Name  : SD_GetCapacity
* Description    : ��ȡSD�����������ֽڣ�
* Input          : None
* Output         : None
* Return         : u32 capacity 
*                   0�� ȡ�������� 
*******************************************************************************/
u32 SD::getCapacity(void)
{
  u8 csd[16];
  u32 Capacity;
  u8 r1;
  u16 i;
  u16 temp;
	spi->take_spi_right(&SPIDevSDCard);

  //ȡCSD��Ϣ������ڼ��������0
  if(getCSD(csd)!=0) return 0;	    
  //���ΪSDHC�����������淽ʽ����
  if((csd[0]&0xC0)==0x40)
  {									  
    Capacity=((u32)csd[8])<<8;
    Capacity+=(u32)csd[9]+1;	 
    Capacity = (Capacity)*1024;//�õ�������
    Capacity*=512;//�õ��ֽ���			   
  }
  else
  {		    
    i = csd[6]&0x03;
    i<<=8;
    i += csd[7];
    i<<=2;
    i += ((csd[8]&0xc0)>>6);
    
    //C_SIZE_MULT
    r1 = csd[9]&0x03;
    r1<<=1;
    r1 += ((csd[10]&0x80)>>7);	 
    r1+=2;//BLOCKNR
    temp = 1;
    while(r1)
    {
      temp*=2;
      r1--;
    }
    Capacity = ((u32)(i+1))*((u32)temp);	 
    // READ_BL_LEN
    i = csd[5]&0x0f;
    //BLOCK_LEN
    temp = 1;
    while(i)
    {
      temp*=2;
      i--;
    }
    //The final result
    Capacity *= (u32)temp;//�ֽ�Ϊ��λ 	  
  }
		spi->release_spi_right();	

  return (u32)Capacity;
}
/*******************************************************************************
* Function Name  : SD_ReadSingleBlock
* Description    : ��SD����һ��block
* Input          : u32 sector ȡ��ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD::readSingleBlock(u32 sector, u8 *buffer)
{
  u8 r1;

  //����Ϊ����ģʽ
	spi->take_spi_right(&SPIDevSDCard);
	
  SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	spi->config(&SPIDevSDCard);
  
  //�������SDHC����sector��ַת��byte��ַ
  sector = sector<<9;

  r1 = _sendCommand(CMD17, sector, 1);//������
												    
  if(r1 != 0x00)return r1; 		   							  
  r1 = _receiveData(buffer, 512, RELEASE);
	
	spi->release_spi_right();	
	
  if(r1 != 0)
    return r1;   //�����ݳ���
  else 
    return 0; 
}
/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : д��SD����һ��block
* Input          : u32 sector ������ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD::writeSingleBlock(u32 sector,  u8 *data)
{
  u8 r1;
  u16 i;
  u16 retry;
  //����Ϊ����ģʽ
	spi->take_spi_right(&SPIDevSDCard);
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	spi->config(&SPIDevSDCard);

  //�������SDHC����������sector��ַ������ת����byte��ַ
  if(SD_Type!=SD_TYPE_V2HC)
  {
     sector = sector<<9;//512*sector�����������ı߽�����ַ
  }

  r1 = _sendCommand(CMD24, sector, 0x01);
  if(r1 != 0x00)
  {
    return r1;  //Ӧ����ȷ��ֱ�ӷ���
  }
    
  //��ʼ׼�����ݴ���
  cs->reset();
  //�ȷ�3�������ݣ��ȴ�SD��׼����
  spi->write(0xff);
  spi->write(0xff);
  spi->write(0xff);
  //����ʼ����0xFE
  spi->write(0xFE);

  //��һ��sector������
  for(i=0;i<512;i++)
  {
     spi->write(*data++);
  }
  //��2��Byte��dummy CRC
  spi->write(0xff);
  spi->write(0xff);
    
  //�ȴ�SD��Ӧ��
  r1 = spi->read();
  if((r1&0x1F)!=0x05)
  {
     cs->set();
     return r1;
  }
    
  //�ȴ��������
  retry = 0;
  while(!spi->write(0xff))//���Ա��ʱ�������߱�����
  {
     retry++;
     if(retry>65534)        //�����ʱ��д��û����ɣ������˳�
     {
        cs->set();
        return 1;           //д�볬ʱ����1
     }
  }

  //д����ɣ�Ƭѡ��1
  cs->set();
  spi->write(0xff);
	
	spi->release_spi_right();	
	

  return 0;
}
/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : ��SD���Ķ��block
* Input          : u32 sector ȡ��ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte��
*                  u8 count ������count��block
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD::readMultiBlock(u32 sector, u8 *buffer, u8 count)
{
  u8 r1;	 			 
	
	spi->take_spi_right(&SPIDevSDCard);
	
	SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	spi->config(&SPIDevSDCard);
  sector = sector<<9;//�������SDHC����sector��ַת��byte��ַ
 //SD_WaitReady();
 //�����������
  r1 = _sendCommand(CMD18, sector, 1);//������
  if(r1 != 0x00)return r1;	 
  do//��ʼ��������
  {
    if(_receiveData(buffer, 512, NO_RELEASE) != 0x00)
    {
       break;
    }
    buffer += 512;
  } while(--count);		 
  //ȫ��������ϣ�����ֹͣ����
  _sendCommand(CMD12, 0, 1);
  //�ͷ�����
  cs->set();
  spi->write(0xFF);    
	
	spi->release_spi_right();	
	
  if(count != 0)
    return count;   //���û�д��꣬����ʣ�����	 
  else 
    return 0;	 
}
/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : д��SD����N��block
* Input          : u32 sector ������ַ��sectorֵ���������ַ�� 
*                  u8 *buffer ���ݴ洢��ַ����С����512byte��
*                  u8 count д���block��Ŀ
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD::writeMultiBlock(u32 sector,  const u8 *data, u8 count)
{
  u8 r1;
  u16 i;	 		 
	
	spi->take_spi_right(&SPIDevSDCard);

	SPIDevSDCard.prescaler = SPI_CLOCK_DIV2;
	spi->config(&SPIDevSDCard);
	
  if(SD_Type != SD_TYPE_V2HC)
    sector = sector<<9;//�������SDHC����������sector��ַ������ת����byte��ַ  
  if(SD_Type != SD_TYPE_MMC) 
    r1 = _sendCommand(ACMD23, count, 0x01);//���Ŀ�꿨����MMC��������ACMD23ָ��ʹ��Ԥ����   
  r1 = _sendCommand(CMD25, sector, 0x01);//�����д��ָ��
  if(r1 != 0x00)return r1;  //Ӧ����ȷ��ֱ�ӷ���	 
  cs->reset();//��ʼ׼�����ݴ���   
  spi->write(0xff);//�ȷ�3�������ݣ��ȴ�SD��׼����
  spi->write(0xff);   
  //--------������N��sectorд���ѭ������
  do
  {
    //����ʼ����0xFC �����Ƕ��д��
     spi->write(0xFC);	  
     //��һ��sector������
     for(i=0;i<512;i++)
     {
        spi->write(*data++);
     }
     //��2��Byte��dummy CRC
     spi->write(0xff);
     spi->write(0xff);
        
     //�ȴ�SD��Ӧ��
     r1 = spi->read();
     if((r1&0x1F)!=0x05)
     {
        cs->set();    //���Ӧ��Ϊ��������������ֱ���˳�
        return r1;
     }

     //�ȴ�SD��д�����
     if(_wait()==1)
     {
        cs->set();    //�ȴ�SD��д����ɳ�ʱ��ֱ���˳�����
        return 1;
     }	   
    }while(--count);//��sector���ݴ������
    
    //��������������0xFD
    r1 = spi->write(0xFD);
    if(r1==0x00)
    {
       count =  0xfe;
    }		   
    if(_wait()) //�ȴ�׼����
    {
      cs->set();
      return 1;  
    }
    //д����ɣ�Ƭѡ��1
    cs->set();
    spi->write(0xff);  
		
	spi->release_spi_right();	
	
    return count;   //����countֵ�����д����count=0������count=1
}
/*******************************************************************************
* Function Name  : SD_Read_Bytes
* Description    : ��ָ������,��offset��ʼ����bytes���ֽ�
* Input          : u32 address ������ַ��sectorֵ���������ַ�� 
*                  u8 *buf     ���ݴ洢��ַ����С<=512byte��
*                  u16 offset  �����������ƫ����
                   u16 bytes   Ҫ�������ֽ���
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD::readBytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes)
{
  u8 r1;u16 i=0;  
	
	spi->take_spi_right(&SPIDevSDCard);

  r1=_sendCommand(CMD17,address<<9,1);//���Ͷ���������      
  if(r1!=0x00)return r1;  //Ӧ����ȷ��ֱ�ӷ���
  cs->reset();//ѡ��SD��
  while (spi->read()!= 0xFE)//ֱ����ȡ�������ݵĿ�ʼͷ0XFE���ż���
  {
    i++;
    if(i>3000)
    {
      cs->set();//�ر�SD��
      return 1;//��ȡʧ��
    }
  }; 		 
  for(i=0;i<offset;i++)
    spi->read();//����offsetλ 
  for(;i<offset+bytes;i++)
    *buf++=spi->read();//��ȡ��������	
  for(;i<512;i++) 
    spi->write(0xff); 	 //����ʣ���ֽ�
  spi->write(0xff);//����αCRC��
  spi->write(0xff);  
  cs->set();//�ر�SD��
	
	spi->release_spi_right();	
	

  return 0;
}




