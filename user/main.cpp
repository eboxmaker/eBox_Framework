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
#include "at24x.h"
#include "TwoWire.h"
#include <PN532_I2C.h>
#include <PN532.h>

TwoWire Wire(&PB10, &PB11);

PN532_I2C pn532i2c(&Wire);
PN532 nfc(pn532i2c);

/**
    *	1	��������Ҫ����eDriveĿ¼�µ�at24c02����
	*	2	��������ʾ��at24c02�Ķ�д����
    *   3   ͨ��begin��������i2c����Ϊ10k��100k��200k��400k
    *   4   ͨ��config������������i2c���ʣ�ע��f0��Ҫ�����ٷ���������timing����
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"AT24C02 example"
#define EXAMPLE_DATE	"2018-08-11"

uint8_t data;
int16_t x, i;
uint8_t wbuf[512];
uint8_t rbuf[512];
#define MAX_LEN 10
int ret = 0;
void test();

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    uint32_t versiondata;
    nfc.begin();
    while(1){
        versiondata = nfc.getFirmwareVersion();
        if (! versiondata) {
        Serial.print("Didn't find PN53x board");
        //while (1); // halt
        }
        delay_ms(1000);
    }

    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    nfc.setPassiveActivationRetries(0xFF);

    // configure board to read RFID tags
    nfc.SAMConfig();

    Serial.println("Waiting for an ISO14443A card");
}



int main(void)
{
    setup();

    while(1)
    {
        
    }
}
