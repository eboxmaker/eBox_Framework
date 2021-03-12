#include "i2c.h"


void I2c::beginTransmission(uint8_t address)
{
    transmitting = 1;
    txAddress = address;
    txIndex = 0;
    txLength = 0;
}


//
void I2c::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

uint8_t I2c::endTransmission(void)
{
    return endTransmission(true);
}
uint8_t I2c::endTransmission(uint8_t sendStop)
{   
    uint8_t ret = _write(txAddress, txBuffer, txLength, sendStop);
    txIndex = 0;
    txLength = 0;
    transmitting = 0;
    return ret;
}


uint8_t I2c::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
    if (isize > 0) 
    {
        // send internal address; this mode allows sending a repeated start to access
        // some devices' internal registers. This function is executed by the hardware
        // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

        beginTransmission(address);

        // the maximum size of internal address is 3 bytes
        if (isize > 3)
        {
            isize = 3;
        }

        // write internal register address - most significant byte first
        while (isize-- > 0)
            write((uint8_t)(iaddress >> (isize*8)));
        endTransmission(false);
    }

    // clamp to buffer length
    if(quantity > I2C_BUFFER_LENGTH)
    {
        quantity = I2C_BUFFER_LENGTH;
    }
    // perform blocking read into buffer
    size_t read = _read(address, rxBuffer, quantity, sendStop);
    // set rx buffer iterator vars
    rxIndex = 0;
    rxLength = read;

    return read;
}
uint8_t I2c::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}    
uint8_t I2c::requestFrom(uint8_t address, uint8_t quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t I2c::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);

}
uint8_t I2c::requestFrom(int address, int quantity, int sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

size_t I2c::write(uint8_t data)
{       
    if(txLength >= I2C_BUFFER_LENGTH) {
        return 0;
    }
    txBuffer[txIndex] = data;
    ++txIndex;
    txLength = txIndex;
    return 1;
}

size_t I2c::write(const uint8_t *data, size_t size)
{
    for(size_t i = 0; i < size; ++i) {
        if(!write(data[i])) {
            return i;
        }
    }
    return size;
}

int I2c::available(void)
{
    int result = rxLength - rxIndex;
    return result;
}

int I2c::read(void)
{
    int value = -1;
    if(rxIndex < rxLength) {
        value = rxBuffer[rxIndex];
        ++rxIndex;
    }
    return value;
}

int I2c::peek(void)
{
    int value = -1;
    if(rxIndex < rxLength) {
        value = rxBuffer[rxIndex];
    }
    return value;
}

void I2c::flush(void)
{
    rxIndex = 0;
    rxLength = 0;
}







