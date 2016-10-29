/**
  ******************************************************************************
  * @file    si4432.cpp
  * @author  songsj
  * @version V1.0
  * @date    2016/10/8
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of songsj. This specification is 
  * preliminary and is subject to change at any time without notice. songsj assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2016 songsj. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/


/* Includes ------------------------------------------------------------------*/
#include "si4432.h"

uint8_t  ItStatus1, ItStatus2;

Si4432::Si4432(Gpio *irq, Gpio *ce, Gpio *cs, Spi *spi)
{
    this->irq = irq;
    this->ce = ce;
    this->cs = cs;
    this->spi = spi;
}

/****************************************************************************
** �������ƣ�begin
** �������ܣ�Si4432��ʼ��
** ��ڲ�������
** ���ڲ�������
****************************************************************************/
void Si4432::begin(uint8_t dev_num)
{
    spi_dev_si4432.dev_num = dev_num;
    spi_dev_si4432.mode = SPI_MODE0;
    spi_dev_si4432.prescaler = SPI_CLOCK_DIV4;
    spi_dev_si4432.bit_order = SPI_BITODER_MSB;
    spi->begin(&spi_dev_si4432);
    cs->mode(OUTPUT_PP);
    cs->set();
	ce->mode(OUTPUT_PP);
	ce->set();
	irq->mode(INPUT_PU);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  + ������:	void SpiWriteRegister(uint8_t reg, uint8_t value)
  + ����:	uint8_t reg - �Ĵ�����ַ
  			uint8_t value - д��ֵ
  + NOTES:  SPIд��SI4432�Ĵ���
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
uint8_t Si4432::SpiWriteRegister (uint8_t reg, uint8_t value)
{
    spi->take_spi_right(&spi_dev_si4432);
    cs->reset();
    spi->write(reg | 0x80);
    spi->write(value);
    cs->set();
    spi->release_spi_right();
    return 1;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  + ������: uint8_t SpiReadRegister(uint8_t reg)
  + ����:	uint8_t reg - �Ĵ�����ַ
  + ����:   SPI1DAT - ����
  + NOTES:  SPI��ȡSI4432�Ĵ���
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
uint8_t Si4432::SpiReadRegister (uint8_t reg)
{
	uint8_t ret;
    spi->take_spi_right(&spi_dev_si4432);
    cs->reset();
    spi->write(reg);
    ret = spi->read();
    cs->set();
    spi->release_spi_right();
    return ret;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  + ������: void set_config()
  + NOTES:  SI4432����ģ������,���ݲ�ͬ����Ƶ����������Ƶģ�飻
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Si4432::set_config()
{
	ItStatus1 = SpiWriteRegister(0x03,0x00);		// ���ж�
	ItStatus2 = SpiWriteRegister(0x04,0x00);
	
	SpiWriteRegister(0x06, 0x00);  			
	
	SpiWriteRegister(0x07, 1);		// ��readyģʽ

	SpiWriteRegister(0x09, 0x7f);  	// ���� 12P

	SpiWriteRegister(0x0a, 0x05);	
	SpiWriteRegister(0x0b, 0xea); 	// GPIO 0 ���ݿ�������������߿��ؿ��ƣ�
	SpiWriteRegister(0x0c, 0xea); 	// GPIO 1 ���ݿ�������������߿��ؿ��ƣ�
	
	SpiWriteRegister(0x0d, 0xf4);  	// GPIO 2 �����������
	
	SpiWriteRegister(0x70, 0x2c);  
	SpiWriteRegister(0x1d, 0x40);  	// ʹ�� afc
		
	// 1.2K bps setting
	SpiWriteRegister(0x1c, 0x16);	//����������Ϊ1.2K
	SpiWriteRegister(0x20, 0x83); 	 
	SpiWriteRegister(0x21, 0xc0); 
	SpiWriteRegister(0x22, 0x13); 
	SpiWriteRegister(0x23, 0xa9); 
	SpiWriteRegister(0x24, 0x00); 
	SpiWriteRegister(0x25, 0x04);	
	SpiWriteRegister(0x2a, 0x14);
	SpiWriteRegister(0x6e, 0x09);
	SpiWriteRegister(0x6f, 0xd5);	
	//1.2K bps setting end		
	
	SpiWriteRegister(0x30, 0x8c);	// ʹ�� PH+ FIFOģʽ, ʹ�� crc, MSB					
	SpiWriteRegister(0x32, 0xff);	 
	SpiWriteRegister(0x33, 0x42);	// ͷ�� = byte 0,1,2,3, ͬ���� = byte 3, 2
				
	SpiWriteRegister(0x34, 16);  	// 16��nibbleǰ����
	SpiWriteRegister(0x35, 0x20);  	// ��Ч���20λǰ����
	SpiWriteRegister(0x36, 0x2d);  	// ͬ���� =  0x2dd4
	SpiWriteRegister(0x37, 0xd4);
	SpiWriteRegister(0x38, 0x00);
	SpiWriteRegister(0x39, 0x00);
	SpiWriteRegister(0x3a, 's');  	// �����ͷ�� ����swwx"
	SpiWriteRegister(0x3b, 'w');
	SpiWriteRegister(0x3c, 'w');
	SpiWriteRegister(0x3d, 'x');
	SpiWriteRegister(0x3e, 10);  	// ���� = 10�ֽ�
	SpiWriteRegister(0x3f, 's'); 	// ����У��ͷ�� ����swwx"
	SpiWriteRegister(0x40, 'w');
	SpiWriteRegister(0x41, 'w');
	SpiWriteRegister(0x42, 'x');
	SpiWriteRegister(0x43, 0xff);  	// У��4�ֽ�ͷ�� 
	SpiWriteRegister(0x44, 0xff);  	
	SpiWriteRegister(0x45, 0xff); 	 
	SpiWriteRegister(0x46, 0xff);  	
	SpiWriteRegister(0x6d, 0x07);  	// ����������

	SpiWriteRegister(0x79, 0x0);  	// 
	SpiWriteRegister(0x7a, 0x0);  	// ����Ƶ	
	
	SpiWriteRegister(0x71, 0x22); 	// FiFo, FSK , not need clk			
	SpiWriteRegister(0x72, 0x20);  	// Ƶƫ 20KHz	

	SpiWriteRegister(0x73, 0x0);  	
	SpiWriteRegister(0x74, 0x0);  	
		
	SpiWriteRegister(0x75, 0x53);  	// Ƶ��:  433.5 MHz
	SpiWriteRegister(0x76, 0x57);  
	SpiWriteRegister(0x77, 0x80);	
}

//------------------------------------------------------------------------------------------------
// ������: set_rx_mode
// Notes : ����ģ�鹤���ڽ���״̬�£���һ��׼���õ�ʱ�򣬵��øú���������ģ�鹤���ڽ���ģʽ��
//-----------------------------------------------------------------------------------------------
void Si4432::set_rx_mode(void)
{
    //read interrupt status registers to release all pending interrupts
    ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);	    //read the Interrupt Status2 register

    //Set the Rx switcher������GPIO�ڿ���ģ���ڲ������߿��ء�
    RxGPIOSetting();

    /*enable receiver chain*/
    SpiWriteRegister(0x07, 0x05);
	
	//ʹ�ܽ������ݰ��ж�
	SpiWriteRegister(0x05,2);
}

//------------------------------------------------------------------------------------------------
// ������: set_tx_mode
// Notes : ����ģ�鹤���ڽ���״̬�£���һ��׼���õ�ʱ�򣬵��øú���������ģ�鹤���ڷ���״̬ģʽ��
//-----------------------------------------------------------------------------------------------
void Si4432::set_tx_mode(void)
{
    //Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
    ItStatus1 = SpiReadRegister(0x03);			//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);			//read the Interrupt Status2 register

    //Set the Rx switcher������GPIO�ڿ���ģ���ڲ������߿��ء�
    TxGPIOSetting();

    /*enable transmitter*/
    //The radio forms the packet and send it automatically.
    SpiWriteRegister(0x07, 0x09);
}

//------------------------------------------------------------------------------------------------
// ������: set_idle_mode
// Notes : ����ģ�鹤���ڿ���״̬�¡�
//          ��ģ�鲻��Ҫ��������Ҳ����Ҫ�������ݵ�ʱ����Ե��øú����Ա�ʡ��
//-----------------------------------------------------------------------------------------------
void Si4432::set_idle_mode(void)
{
    //The radio will work in idle mode
    SpiWriteRegister(0x07, 0x01);	//write 0x09 to the Operating Function Control 1 register

    IdleGPIOSetting();

    //Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
    ItStatus1 = SpiReadRegister(0x03);			//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);			//read the Interrupt Status2 register
}

//��λģ��
void Si4432::reset(void)
{
	SpiWriteRegister(0x08, 0x02);												//write 0x02 to the Operating Function Control 2 register
	SpiWriteRegister(0x08, 0x00);												//write 0x00 to the Operating Function Control 2 register
}
//------------------------------------------------------------------------------------------------
// ������:write
// ����  : uint8_t len_tx  : �������ݳ���(������64�ֽ�)
//         uint8_t *txdata: ���ݵ�ָ��
// Notes : ���ݷ���
//-----------------------------------------------------------------------------------------------
void Si4432::write(uint8_t *txdata, uint8_t len_tx)
{
    uint8_t i;

    set_idle_mode();
	
    //�������ݰ��ĳ���(�ֽ�)
    SpiWriteRegister(0x3E, len_tx);

    //������д��RF
    for(i = 0; i < len_tx; i++)
    {
        SpiWriteRegister(0x7F, txdata[i]);
    }

	//�ر����������ж�,���򿪷����жϡ�
    SpiWriteRegister(0x05, 0x04);	//write 0x04 to the Interrupt Enable 1 register
    SpiWriteRegister(0x06, 0x00);	//write 0x00 to the Interrupt Enable 2 register

    set_tx_mode();

    //�ȴ���������жϵĵ���������жϲ��÷ǲ�ѯ��ʽ���򱾳���������Խ��������������жϷ�������ж�ȡ״̬�Ĵ���0x03��0x04���Ա��ͷ��ж�
    while (irq->read());

    //read interrupt status registers to release the interrupt flags
    ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register
}

//------------------------------------------------------------------------------------------------
// ������:read(uint8_t payload[])
// Notes : ��������,���ж���ʱ
//-----------------------------------------------------------------------------------------------
uint8_t Si4432::read(uint8_t *rxbuf)
{	
	uint8_t temp8,len_Rx;

    //�ж��Ƿ�����CRCУ������������жϣ���RFSetIdleMode���Ѿ��������ж�״̬�Ĵ�����ֵ
    if( (ItStatus1 & 0x01) == 0x01 )
    {
        //�����CRCУ�������ô ����FIFO ��λ��
        reset();
    }

    //�ж��Ƿ������ݰ��Ѿ�����ȷ���ա�
    if( (ItStatus1 & 0x02) == 0x02 )
    {
        len_Rx = SpiReadRegister(0x4B);		//��ȡ�յ������ݰ�����
        //���ݳ����ж���Ӧ�Ĳ�����
        if(len_Rx <= 64)
        {
            //ֱ�Ӵ�FIFO�ж�ȡ�յ������ݡ��ͻ�ֻҪ����FIFO�����ݾ����յ����ݡ�
            for(temp8 = 0; temp8 < len_Rx; temp8++)
            {
                rxbuf[temp8] = SpiReadRegister(0x7F);						//read the FIFO Access register
            }
        }
    }
	return len_Rx;
}

// �����ź�ǿ��
uint8_t Si4432::read_rssi(void)
{
	return SpiReadRegister(0x26);
}

// ��������ģ��
void Si4432::start(void)
{
	//������Ƶģ�飬ģ���SDN�������ͺ������ʱ����30ms��ʵ���Ͽ���ֱ�Ӱ�SDN���Žӵأ������Ͳ����ڳ����г�ʼ���ˣ�
	ce->reset();
	
    delay_ms(30);
		
    //�ֱ�����GPIO0/1/2�˿�Ϊֱ���������
    SpiWriteRegister(0x0B, 0xCA);
    SpiWriteRegister(0x0C, 0xCA);
    SpiWriteRegister(0x0D, 0xCA);

    //��ȡ�ж�״̬�Ĵ�������жϱ�־�Ա��ͷ�NIRQ����
    //����ж����ˣ���ôNIRQ���Żᱻ���ͣ������ʱ���ж�״̬�Ĵ���0x03��0x04����ȡ����ôNIRQ���Żᱻ�ͷŻָ��ߵ�ƽ
    ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register

    //SW reset�������λ���ģ������оƬ
    SpiWriteRegister(0x07, 0x80);			//write 0x80 to the Operating & Function Control1 register
    
	//�ȴ���λ��ɣ�����λ��ɺ����ж�,���ж�״̬�Ĵ����ͷ��ж�����(NIRQ���Żָ��ߵ�ƽ)
    while (irq->read());
	
    ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register
}

uint8_t Si4432::read_irq(void)
{
	return irq->read();
}

