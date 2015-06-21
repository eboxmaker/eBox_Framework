#include "softspi.h"
SOFTSPI::SOFTSPI(uint8_t sckPin,uint8_t misoPin,uint8_t mosiPin)
{
	_mosiPin = mosiPin;
	_misoPin = misoPin;
	_sckPin = sckPin;
	
	_bitOrder = MSBFIRST;
	_mode = SPI_MODE0;
	_spidelay = 0;

}

void SOFTSPI::begin()
{
	setMode(_mode);
	setBitOrder(_bitOrder);
	setClockDivider(_spidelay);

	pinMode(_mosiPin,1);
	pinMode(_misoPin,1);
	pinMode(_sckPin,1);

	switch(_mode)
	{
		case SPI_MODE0:
			 _cpol = CPOL_Low;
			 _cpha = CPHA_1Edge;
				digitalWrite(_sckPin,0);
				break;
		case SPI_MODE1:
			 _cpol = CPOL_Low;
			 _cpha = CPHA_2Edge;
				digitalWrite(_sckPin,0);		
				break;
		case SPI_MODE2:
			 _cpol = CPOL_High;
			 _cpha = CPHA_1Edge;
				digitalWrite(_sckPin,1);		
				break;
		case SPI_MODE3:
			 _cpol = CPOL_High;
				digitalWrite(_sckPin,1);		
			 _cpha = CPHA_2Edge;
				break;			
	}
}
void SOFTSPI::setBitOrder(uint8_t bitOrder)
{
	_bitOrder = bitOrder;
}
void SOFTSPI::setClockDivider(uint8_t count)
{
	_spidelay = count;
}
void SOFTSPI::setMode(uint8_t spimode)
{
	_mode = spimode;

}
uint8_t SOFTSPI::transfer(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;
	switch(_mode)
	{
		case SPI_MODE0:
			//时钟空闲输出：0；
			//第一个是上升沿：读取数据；
			//第二个是下降沿：输出数据；
				for (i = 0; i < 8; i++)  {
					digitalWrite(_sckPin,1);
					if (_bitOrder == LSBFIRST)
					{
						RcvData |= digitalRead(_misoPin)<<i;
						digitalWrite(_mosiPin, !!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (digitalRead(_misoPin)<<(7-i));
						digitalWrite(_mosiPin, !!(data & (1 << (7 - i))));
					}
					delay_us(_spidelay);
					digitalWrite(_sckPin,0);
					delay_us(_spidelay);
				}
				break;
		case SPI_MODE1:
			//时钟空闲输出：0；
			//第一个是上升沿：输出数据；
			//第二个是下降沿：读取数据；
				for (i = 0; i < 8; i++)  {
					///////////////////上升沿输出//////////
					digitalWrite(_sckPin,1);
					if (_bitOrder == LSBFIRST)
					{
						digitalWrite(_mosiPin, !!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (digitalRead(_misoPin)<<(7-i));
						digitalWrite(_mosiPin, !!(data & (1 << (7 - i))));
					}
					delay_us(_spidelay);
					/////////////////下降沿采样////////////
					digitalWrite(_sckPin,0);
					if (_bitOrder == LSBFIRST)
					{
						digitalWrite(_mosiPin, !!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (digitalRead(_misoPin)<<(7-i));
						digitalWrite(_mosiPin, !!(data & (1 << (7 - i))));
					}	
					delay_us(_spidelay);
				}
				break;

		case SPI_MODE2:
			//时钟空闲输出：1；
			//第一个是下降沿：读取数据；
			//第二个是上升沿：输出数据；
				for (i = 0; i < 8; i++)  {
					digitalWrite(_sckPin,0);
					if (_bitOrder == LSBFIRST)
					{
						RcvData |= digitalRead(_misoPin)<<i;
						digitalWrite(_mosiPin, !!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (digitalRead(_misoPin)<<(7-i));
						digitalWrite(_mosiPin, !!(data & (1 << (7 - i))));
					}
					delay_us(_spidelay);
					digitalWrite(_sckPin,1);
					delay_us(_spidelay);
				}
				break;
				
		case SPI_MODE3:
			//时钟空闲输出：1；
			//第一个是下降沿：输出数据；
			//第二个是上升沿：读取数据；
				for (i = 0; i < 8; i++)  {
					///////////////////下降沿沿输出
					digitalWrite(_sckPin,0);
					if (_bitOrder == LSBFIRST)
					{
						digitalWrite(_mosiPin, !!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (digitalRead(_misoPin)<<(7-i));
						digitalWrite(_mosiPin, !!(data & (1 << (7 - i))));
					}
					delay_us(_spidelay);
					/////////////////上升沿采样////////////
					digitalWrite(_sckPin,1);
					if (_bitOrder == LSBFIRST)
					{
						digitalWrite(_mosiPin, !!(data & (1 << i)));
					}
					else
					{				
						RcvData |= (digitalRead(_misoPin)<<(7-i));
						digitalWrite(_mosiPin, !!(data & (1 << (7 - i))));
					}	
					delay_us(_spidelay);
				}
				break;
		}
				return RcvData;
}








