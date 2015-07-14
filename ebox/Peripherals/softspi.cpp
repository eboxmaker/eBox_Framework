#include "softspi.h"
uint8_t SOFTSPI::currentDevNum = 0;

SOFTSPI::SOFTSPI(GPIO* sckPin,GPIO* misoPin,GPIO* mosiPin)
{
	_mosiPin = mosiPin;
	_misoPin = misoPin;
	_sckPin =  sckPin;
	
//	_bitOrder = MSBFIRST;
//	_mode = SPI_MODE0;
//	_spidelay = 0;

}
void SOFTSPI::begin(SPICONFIG* spiConfig)
{

	_sckPin->mode(OUTPUT_PP);
	_misoPin->mode(INPUT);
	_mosiPin->mode(OUTPUT_PP);
	
	config(spiConfig);
	switch(_mode)
	{
		case SPI_MODE0:
				_sckPin->reset();
				break;
		case SPI_MODE1:
				_sckPin->reset();		
				break;
		case SPI_MODE2:
				_sckPin->set();		
				break;
		case SPI_MODE3:
				_sckPin->set();		
				break;			
	}
}
void SOFTSPI::config(SPICONFIG* spiConfig)
{
	currentDevNum = spiConfig->devNum;
	_mode = spiConfig->mode;
	_bitOrder = spiConfig->bitOrder;
	switch(spiConfig->prescaler)
	{
		case SPI_BaudRatePrescaler_2:
			_spidelay = 0;
			break;
		case SPI_BaudRatePrescaler_4:
			_spidelay = 1;
			break;
		case SPI_BaudRatePrescaler_8:
			_spidelay = 2;
			break;
		case SPI_BaudRatePrescaler_16:
			_spidelay = 4;
			break;
		case SPI_BaudRatePrescaler_32:
			_spidelay = 8;
			break;
		case SPI_BaudRatePrescaler_64:
			_spidelay = 16;
			break;
		case SPI_BaudRatePrescaler_128:
			_spidelay = 32;
			break;
		case SPI_BaudRatePrescaler_256:
			_spidelay = 64;
			break;
		default:
			_spidelay = spiConfig->prescaler;
			break;
	}
}
uint8_t SOFTSPI::readConfig(void)
{
	return currentDevNum; 
}
uint8_t SOFTSPI::transfer(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;
	switch(_mode)
	{
		case SPI_MODE0:
			//ʱ�ӿ��������0��
			//��һ���������أ���ȡ���ݣ�
			//�ڶ������½��أ�������ݣ�
				for (i = 0; i < 8; i++)  {
					if (_bitOrder == SPI_BITODER_LSB)
					{
						RcvData |= _misoPin->read()<<i;
						_mosiPin->write(!!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (_misoPin->read()<<(7-i));
						_mosiPin->write(!!(data & (1 << (7 - i))));
					}
					delay_us(_spidelay);
					_sckPin->set();
					delay_us(_spidelay);
					_sckPin->reset();
				}
				break;
		case SPI_MODE1:
			//ʱ�ӿ��������0��
			//��һ���������أ�������ݣ�
			//�ڶ������½��أ���ȡ���ݣ�
				for (i = 0; i < 8; i++)  {
					///////////////////���������//////////
					if (_bitOrder == SPI_BITODER_LSB)
					{
						_mosiPin->write(!!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (_misoPin->read()<<(7-i));
						_mosiPin->write(!!(data & (1 << (7 - i))));
					}
					delay_us(_spidelay);
					_sckPin->set();
					/////////////////�½��ز���////////////
					delay_us(_spidelay);
					_sckPin->reset();
					if (_bitOrder == LSBFIRST)
					{
						_mosiPin->write(!!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (_misoPin->read()<<(7-i));
						_mosiPin->write(!!(data & (1 << (7 - i))));
					}	
				}
				break;

		case SPI_MODE2:
			//ʱ�ӿ��������1��
			//��һ�����½��أ���ȡ���ݣ�
			//�ڶ����������أ�������ݣ�
				for (i = 0; i < 8; i++)  {
					_sckPin->reset();
					delay_us(_spidelay);
					if (_bitOrder == SPI_BITODER_LSB)
					{
						RcvData |= _misoPin->read()<<i;
						_mosiPin->write(!!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (_misoPin->read()<<(7-i));
						_mosiPin->write(!!(data & (1 << (7 - i))));
					}
					delay_us(_spidelay);
					_sckPin->set();
				}
				break;
				
		case SPI_MODE3:
			//ʱ�ӿ��������1��
			//��һ�����½��أ�������ݣ�
			//�ڶ����������أ���ȡ���ݣ�
				for (i = 0; i < 8; i++)  {
					///////////////////�½��������
					_sckPin->reset();
					delay_us(_spidelay);
					if (_bitOrder == SPI_BITODER_LSB)
					{
						_mosiPin->write(!!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (_misoPin->read()<<(7-i));
						_mosiPin->write(!!(data & (1 << (7 - i))));
					}
					/////////////////�����ز���////////////
					_sckPin->set();
					delay_us(_spidelay);
					if (_bitOrder == LSBFIRST)
					{
						_mosiPin->write(!!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (_misoPin->read()<<(7-i));
						_mosiPin->write(!!(data & (1 << (7 - i))));
					}	
				}
				break;
		}
				return RcvData;
}








