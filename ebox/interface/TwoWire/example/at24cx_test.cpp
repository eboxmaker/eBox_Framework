/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"

#include "at24c02.h"
#include "at24x.h"

At24x ee(&Wire);
/**
    *	1	此例程需要调用eDrive目录下的at24c02驱动
	*	2	此例程演示了at24c02的读写操作
    *   3   通过begin可以设置i2c速率为10k，100k，200k，400k
    *   4   通过config可以自由设置i2c速率，注意f0需要借助官方工具生成timing传入
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"AT24C02 example"
#define EXAMPLE_DATE	"2018-08-11"

uint8_t data;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
   // Wire.begin();
    ee.begin();

}
int16_t x, i;
uint8_t wbuf[512];
uint8_t rbuf[512];
#define MAX_LEN 10
int ret = 0;
void scan1();
void scan2();
void test();


int main(void)
{
    setup();

    while(1)
    {
        test();
        
    }
}

void test()
{
 ret = 0;

        uart1.printf("=================wbuf================\r\n");
        for(uint16_t i = 0; i < MAX_LEN; i++)
        {
            wbuf[i] = random()%255;
            rbuf[i] = 0;
        }
        for(uint16_t i = 0; i < MAX_LEN; i++)
        {
            uart1.printf(" %02x ", wbuf[i ]);
            //ee.byteWrite(i*16+j,buf[i*16+j]);
        }
        uart1.printf("\r\n ");
        
        ee.write_byte(0, wbuf, MAX_LEN);

        uart1.printf("==================rbuf==============\r\n");

        data = ee.read_byte(0                                                                                                                                                                           , rbuf, MAX_LEN);
        for(uint16_t i = 0; i < MAX_LEN; i++)
        {
            uart1.printf(" %02x ", rbuf[i]);
        }
        uart1.printf("\r\n ");
        for(int i = 0; i < MAX_LEN; i++)
        {
            if(wbuf[i] != rbuf[i])
            {
                ret = 1;
                break;
            }
        }
        if(ret == 1)
        {
            uart1.printf("eeprom check ......[err]");
            ee.begin();
        }
        else
            uart1.printf("eeprom check ......[OK]");

        uart1.printf("\r\n================================\r\n");
        delay_ms(1000);

}
void scan1()
{
      for (byte address = 0x48; address < 0x4A; ++address) {

          Wire.beginTransmission(address);
        byte error = Wire.endTransmission();

        if (error != I2C_ERROR_OK) {
            Serial.printf("I2C device not found at address 0x%02x(ERR:%d)\n",address,error);
        }
        else
        {
              Serial.printf("I2C ok 0x%02x\n",address);
        }
    }
        delay_ms(1000);
}
void scan2()
{
      int nDevices = 0;

      Serial.println("Scanning...");

      for (byte address = 10; address < 255; ++address) {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();

        if (error == 0) {
          Serial.print("I2C device found at address 0x");
          if (address < 16) {
            Serial.print("0");
          }
          Serial.print(address, HEX);
          Serial.println("  !");

          ++nDevices;
        } else if (error == 4) {
          Serial.print("Unknown error at address 0x");
          if (address < 16) {
            Serial.print("0");
          }
          Serial.println(address, HEX);
        }
        uart1.flush();

      }
      if (nDevices == 0) {
        Serial.println("No I2C devices found\n");
      } else {
          Serial.printf("done:%d\n",nDevices);
      }
      delay_ms(1000); // Wait 5 seconds for next scan
}




