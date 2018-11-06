/*  PCA9685 LED library for Arduino
    Copyright (C) 2012 Kasper Sk?rh?j    <kasperskaarhoj@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "PCA9685.h"


PCA9685::PCA9685(SoftI2c *i2c)
{

    this->i2c = i2c;
}

void PCA9685::begin(uint32_t speed, int i2cAddress)
{
    _i2cAddress = ((PCA9685_I2C_BASE_ADDRESS) | (i2cAddress & B00111111));
    this->speed = speed;
    i2c->begin(speed);
}
bool PCA9685::init()
{

    delay_ms(1);
    writeRegister(PCA9685_MODE1, (uint8_t)0x01);	// reset the device

    delay_ms(1);
    bool isOnline;
    if (readRegister(PCA9685_MODE1) == 0x01)
    {
        isOnline = true;
    }
    else
    {
        isOnline = false;
    }
    writeRegister(PCA9685_MODE1, (uint8_t)B10100000);	// set up for auto increment
    writeRegister(PCA9685_MODE2, (uint8_t)0x10);	// set to output

    return isOnline;
}

void PCA9685::setLEDOn(int ledNumber)
{
    writeLED(ledNumber, 0x1000, 0);
}

void PCA9685::setLEDOff(int ledNumber)
{
    writeLED(ledNumber, 0, 0x1000);
}

void PCA9685::setLEDDimmed(int ledNumber, uint8_t amount)  		// Amount from 0-100 (off-on)
{
    if (amount == 0)
    {
        setLEDOff(ledNumber);
    }
    else if (amount >= 100)
    {
        setLEDOn(ledNumber);
    }
    else
    {
        int randNumber = (int)random(4096);	// Randomize the phaseshift to distribute load. Good idea? Hope so.
        writeLED(ledNumber, randNumber, (int)(amount * 41 + randNumber) & 0xFFF);
    }
}

void PCA9685::writeLED(int ledNumber, uint32_t LED_ON, uint32_t LED_OFF)  	// LED_ON and LED_OFF are 12bit values (0-4095); ledNumber is 0-15
{
    if (ledNumber >= 0 && ledNumber <= 15)
    {
        //		Wire.beginTransmission(_i2cAddress);
        //		Wire.write(PCA9685_LED0 + 4*ledNumber);

        //		Wire.write(lowByte(LED_ON));
        //		Wire.write(highByte(LED_ON));
        //		Wire.write(lowByte(LED_OFF));
        //		Wire.write(highByte(LED_OFF));
        //
        //		Wire.endTransmission();

        i2c->take_i2c_right(speed);
        i2c->write_byte(_i2cAddress, PCA9685_LED0 + 4 * ledNumber, low_byte(LED_ON));
        i2c->write_byte(_i2cAddress, PCA9685_LED0 + 4 * ledNumber, high_byte(LED_ON));
        i2c->write_byte(_i2cAddress, PCA9685_LED0 + 4 * ledNumber, low_byte(LED_OFF));
        i2c->write_byte(_i2cAddress, PCA9685_LED0 + 4 * ledNumber, high_byte(LED_ON));
        i2c->release_i2c_right();

    }
}


//PRIVATE
void PCA9685::writeRegister(int regAddress, uint8_t data)
{
    //	Wire.beginTransmission(_i2cAddress);
    //	Wire.write(regAddress);
    //	Wire.write(data);
    //	Wire.endTransmission();
    i2c->take_i2c_right(speed);
    i2c->write_byte(_i2cAddress, regAddress, data);
    i2c->release_i2c_right();

}

uint32_t PCA9685::readRegister(int regAddress)
{
    //	uint32_t returnword = 0x00;
    //	Wire.beginTransmission(_i2cAddress);
    //	Wire.write(regAddress);
    //	Wire.endTransmission();
    //	Wire.requestFrom((int)_i2cAddress, 1);
    //
    ////    int c=0;
    //	//Wait for our 2 bytes to become available
    //	while (Wire.available()) {
    //        //high uint8_t
    ////        if (c==0)   { returnword = Wire.read() << 8; }
    //        //low uint8_t
    //  //      if (c==1)   {
    //		returnword |= Wire.read();
    //		//}
    //        //c++;
    //    }
    //
    //	return returnword;
    DataU32_t data;
    i2c->take_i2c_right(speed);
    i2c->read_byte(_i2cAddress, regAddress, data.byte, 4);
    i2c->release_i2c_right();
    return data.value;


}

