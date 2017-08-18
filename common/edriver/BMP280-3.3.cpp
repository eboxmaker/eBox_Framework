#include "BMP280-3.3.h"
/***************************************************************************
  This is a library for the BMP280 pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout 
  ----> http://www.adafruit.com/products/2651
 
  These sensors use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include "BMP280-3.3.h"

static bmp280_calib_data bmp280_calib; 

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/




bool Adafruit_BMP280::begin(uint8_t dev_num) 
{
  //_i2caddr = a;

//    if (_cs == -1)
//    {
//        // i2c
////        Wire.begin();
//    } 
//    else 
//    {
        cs->mode(OUTPUT_PP);
        cs->set();
        config.dev_num = dev_num;
        config.bit_order = SPI_BITODER_MSB;
        config.mode = SPI_MODE3;
        config.prescaler = SPI_CLOCK_DIV2;
//      }
    spi->begin(&config);

  if (read8(BMP280_REGISTER_CHIPID) != 0x58)
    return false;

  readCoefficients();
  write8(BMP280_REGISTER_CONTROL, 0x3F);
  return true;
}

//uint8_t Adafruit_BMP280::spixfer(uint8_t x) {
////  if (_sck == -1) 
////    return SPI.transfer(x);
////  
////  // software spi
////  //Serial.println("Software SPI");
////  uint8_t reply = 0;
////  for (int i=7; i>=0; i--) {
////    reply <<= 1;
////    digitalWrite(_sck, LOW);
////    digitalWrite(_mosi, x & (1<<i));
////    digitalWrite(_sck, HIGH);
////    if (digitalRead(_miso)) 
////      reply |= 1;
////  }
//   uint8_t reply = 0;
//    
// return reply;
//}

/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C/SPI
*/
/**************************************************************************/
void Adafruit_BMP280::write8(uint8_t reg, uint8_t value)
{
//  if (_cs == -1) {
//    Wire.beginTransmission((uint8_t)_i2caddr);
//    Wire.write((uint8_t)reg);
//    Wire.write((uint8_t)value);
//    Wire.endTransmission();
//  } else {
//    if (_sck == -1)
//      SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
//    digitalWrite(_cs, LOW);
//    spixfer(reg & ~0x80); // write, bit 7 low
//    spixfer(value);
//    digitalWrite(_cs, HIGH);
//    if (_sck == -1)
//      SPI.endTransaction();              // release the SPI bus
//  }
    spi->take_spi_right(&config);
    cs->reset();
    spi->transfer(reg& ~0x80);// write, bit 7 low
    spi->transfer(value);
    cs->set();
    spi->release_spi_right();
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t Adafruit_BMP280::read8(uint8_t reg)
{
  uint8_t value;
  
//  if (_cs == -1) {
//    Wire.beginTransmission((uint8_t)_i2caddr);
//    Wire.write((uint8_t)reg);
//    Wire.endTransmission();
//    Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)1);
//    value = Wire.read();
//    Wire.endTransmission();
//  } else {
//    if (_sck == -1)
//      SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
//    digitalWrite(_cs, LOW);
//    spixfer(reg | 0x80); // read, bit 7 high
//    value = spixfer(0);
//    digitalWrite(_cs, HIGH);
//    if (_sck == -1)
//      SPI.endTransaction();              // release the SPI bus
//  }
    spi->take_spi_right(&config);
    cs->reset();
    spi->transfer(reg | 0x80);// write, bit 7 low
    value = spi->transfer(0);
    cs->set();
    spi->release_spi_right();
    return value;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/
uint16_t Adafruit_BMP280::read16(uint8_t reg)
{
  uint16_t value;

//    if (_cs == -1) {
//    Wire.beginTransmission((uint8_t)_i2caddr);
//    Wire.write((uint8_t)reg);
//    Wire.endTransmission();
//    Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)2);
//    value = (Wire.read() << 8) | Wire.read();
//    Wire.endTransmission();
//    } else {
//    if (_sck == -1)
//      SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
//    digitalWrite(_cs, LOW);
//    spixfer(reg | 0x80); // read, bit 7 high
//    value = (spixfer(0) << 8) | spixfer(0);
//    digitalWrite(_cs, HIGH);
//    if (_sck == -1)
//      SPI.endTransaction();              // release the SPI bus
//    }
    spi->take_spi_right(&config);
    cs->reset();
    spi->transfer(reg | 0x80);// write, bit 7 low
    value = (spi->transfer(0) << 8);
    value |= spi->transfer(0);
    cs->set();
    spi->release_spi_right();

  return value;
}

uint16_t Adafruit_BMP280::read16_LE(uint8_t reg) {
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
  
}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/
int16_t Adafruit_BMP280::readS16(uint8_t reg)
{
  return (int16_t)read16(reg);

}

int16_t Adafruit_BMP280::readS16_LE(uint8_t reg)
{
  return (int16_t)read16_LE(reg);

}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void Adafruit_BMP280::readCoefficients(void)
{
    bmp280_calib.dig_T1 = read16_LE(BMP280_REGISTER_DIG_T1);
    bmp280_calib.dig_T2 = readS16_LE(BMP280_REGISTER_DIG_T2);
    bmp280_calib.dig_T3 = readS16_LE(BMP280_REGISTER_DIG_T3);

    bmp280_calib.dig_P1 = read16_LE(BMP280_REGISTER_DIG_P1);
    bmp280_calib.dig_P2 = readS16_LE(BMP280_REGISTER_DIG_P2);
    bmp280_calib.dig_P3 = readS16_LE(BMP280_REGISTER_DIG_P3);
    bmp280_calib.dig_P4 = readS16_LE(BMP280_REGISTER_DIG_P4);
    bmp280_calib.dig_P5 = readS16_LE(BMP280_REGISTER_DIG_P5);
    bmp280_calib.dig_P6 = readS16_LE(BMP280_REGISTER_DIG_P6);
    bmp280_calib.dig_P7 = readS16_LE(BMP280_REGISTER_DIG_P7);
    bmp280_calib.dig_P8 = readS16_LE(BMP280_REGISTER_DIG_P8);
    bmp280_calib.dig_P9 = readS16_LE(BMP280_REGISTER_DIG_P9);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BMP280::readTemperature(void)
{
  int32_t var1, var2;
  
  int32_t adc_T = read16(BMP280_REGISTER_TEMPDATA);
  adc_T <<= 8;
  adc_T |= read8(BMP280_REGISTER_TEMPDATA+2);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)bmp280_calib.dig_T1 <<1))) * 
	   ((int32_t)bmp280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)bmp280_calib.dig_T1)) * 
	     ((adc_T>>4) - ((int32_t)bmp280_calib.dig_T1))) >> 12) * 
	   ((int32_t)bmp280_calib.dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T/100;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BMP280::readPressure(void) {
  int64_t var1, var2, p;
  
  int32_t adc_P = read16(BMP280_REGISTER_PRESSUREDATA);
  adc_P <<= 8;
  adc_P |= read8(BMP280_REGISTER_PRESSUREDATA+2);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)bmp280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)bmp280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)bmp280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)bmp280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)bmp280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bmp280_calib.dig_P1)>>33;
  
  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)bmp280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)bmp280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280_calib.dig_P7)<<4);
  return (float)p/256;
}
