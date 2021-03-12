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
#include <PN532_I2C.h>

PN532_I2C nfc(&PB8,&PB9);
/**
    *	1	此例程需要调用eDrive目录下的at24c02驱动
	*	2	此例程演示了at24c02的读写操作
    *   3   通过begin可以设置i2c速率为10k，100k，200k，400k
    *   4   通过config可以自由设置i2c速率，注意f0需要借助官方工具生成timing传入
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"AT24C02 example"
#define EXAMPLE_DATE	"2018-08-11"

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
        else
            break;
        delay_ms(1000);
    }

    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    bool err = nfc.setPassiveActivationRetries(0xFF);
    if(!err)
        Serial.println("err1\n");
    // configure board to read RFID tags
    err = nfc.SAMConfig();
    if(!err)
        Serial.println("err2\n");

    Serial.println("Waiting for an ISO14443A card");
}



int main(void)
{
    setup();

    while(1)
    {
        boolean success;
        uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
        uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

        // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
        // 'uid' will be populated with the UID, and uidLength will indicate
        // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
        success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

        if (success) {
        Serial.println("Found a card!");
        Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
        Serial.print("UID Value: ");
        for (uint8_t i=0; i < uidLength; i++) 
        {
        Serial.print(" 0x");Serial.print(uid[i], HEX); 
        }
        Serial.println("");
        // Wait 1 second before continuing
        delay(1000);
        }
        else
        {
        // PN532 probably timed out waiting for a card
        Serial.println("Timed out waiting for a card");
        }
    }
}
