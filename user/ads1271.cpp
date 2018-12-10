#include "ads1271.h"

void ADS1271::begin()
{
    config.dev_num  = spi->get_new_dev_num();
    config.bit_order = MSB_FIRST;
    config.mode     = SPI_MODE0;
    config.prescaler = SPI_CLOCK_DIV2;

    spi->begin(&config);


}
int32_t ADS1271::read()
{
    uint32_t temp = 0;
    temp |= spi->read() << 16;
    temp |= spi->read() << 8;
    temp |= spi->read();
    return temp;
}
void ADS1271::process()
{

}

