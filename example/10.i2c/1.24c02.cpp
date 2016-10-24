/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */


#include "ebox.h"
#include "at24c02.h"
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

At24c02 ee(&i2c2);
uint8_t data;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    ee.begin(400000);
}

int16_t x, i;
uint8_t wbuf[512];
uint8_t rbuf[512];
#define MAX_LEN 10
int ret = 0;
int main(void)
{
    setup();


    while(1)
    {

        uart1.printf("=================wbuf================\r\n");
        for(uint16_t i = 0; i < MAX_LEN; i++)
        {
            wbuf[i] = random() % 256;
        }
        for(uint16_t i = 0; i < MAX_LEN; i++)
        {
                uart1.printf(" %02x ", wbuf[i ]);
                //ee.byteWrite(i*16+j,buf[i*16+j]);
        }
        uart1.printf("\r\n ");
        ee.write_byte(256, wbuf, MAX_LEN);

        uart1.printf("==================rbuf==============\r\n");

        data = ee.read_byte(256, rbuf, MAX_LEN);
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
            ee.begin(4000);
        }
        else
            uart1.printf("eeprom check ......[OK]");
            
        uart1.printf("\r\n================================\r\n");
        delay_ms(1000);
    }


}




