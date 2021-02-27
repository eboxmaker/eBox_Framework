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

#include "TwoWire.h"
TwoWire Wire(&PB10, &PB11);
/**
    *	1	此例程为IIC扫描程序
	*	2	此例程演示了扫描总线上所有地址有响应的设备
    *   3   通过begin可以设置i2c速率为10k，100k，200k，400k，默认100k
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"IIC Scan example"
#define EXAMPLE_DATE	"2021-02-27"

uint8_t data;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    Wire.begin();

}

void scan();



int main(void)
{
    setup();

    while(1)
    {
        scan();
    }
}

void scan()
{
      int nDevices = 0;

      Serial.println("Scanning...");

      for (byte address = 0; address < 254; ) {
        address += 2;
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




