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

// For a http://www. url:
const char * url = "adafruit.com";
uint8_t ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT;

// for an email address
//const char * url = "mail@example.com";
//uint8_t ndefprefix = NDEF_URIPREFIX_MAILTO;

// for a phone number
//const char * url = "+1 212 555 1212";
//uint8_t ndefprefix = NDEF_URIPREFIX_TEL;





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

void test();


int main(void)
{
    setup();
    test();

    while(1)
    {
        
    }
}
void test()
{
        uint8_t success;                          // Flag to check if there was an error with the PN532
        uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
        uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
        bool authenticated = false;               // Flag to indicate if the sector is authenticated

        // Use the default key
        uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

        Serial.println("");
        Serial.println("PLEASE NOTE: Formatting your card for NDEF records will change the");
        Serial.println("authentication keys.  To reformat your NDEF tag as a clean Mifare");
        Serial.println("Classic tag, use the mifareclassic_ndeftoclassic example!");
        Serial.println("");
        Serial.println("Place your Mifare Classic card on the reader to format with NDEF");
        Serial.println("and press any key to continue ...");
        // Wait for user input before proceeding
        while (!Serial.available());
        // a key was pressed1
        while (Serial.available()) Serial.read();

        // Wait for an ISO14443A type card (Mifare, etc.).  When one is found
        // 'uid' will be populated with the UID, and uidLength will indicate
        // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
        success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

        if (success)
        {
        // Display some basic information about the card
        Serial.println("Found an ISO14443A card");
        Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
        Serial.print("  UID Value: ");
        nfc.PrintHex(uid, uidLength);
        Serial.println("");

        // Make sure this is a Mifare Classic card
        if (uidLength != 4)
        {
        Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
        Serial.flush();
        return;
        }

        // We probably have a Mifare Classic card ...
        Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

        // Try to format the card for NDEF data
        success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 0, 0, keya);
        if (!success)
        {
        Serial.println("Unable to authenticate block 0 to enable card formatting!");
        return;
        }
        success = nfc.mifareclassic_FormatNDEF();
        if (!success)
        {
        Serial.println("Unable to format the card for NDEF");
        return;
        }

        Serial.println("Card has been formatted for NDEF data using MAD1");

        // Try to authenticate block 4 (first block of sector 1) using our key
        success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 0, keya);

        // Make sure the authentification process didn't fail
        if (!success)
        {
        Serial.println("Authentication failed.");
        return;
        }

        // Try to write a URL
        Serial.println("Writing URI to sector 1 as an NDEF Message");

        // Authenticated seems to have worked
        // Try to write an NDEF record to sector 1
        // Use 0x01 for the URI Identifier Code to prepend "http://www."
        // to the url (and save some space).  For information on URI ID Codes
        // see http://www.ladyada.net/wiki/private/articlestaging/nfc/ndef
        if (strlen(url) > 38)
        {
        // The length is also checked in the WriteNDEFURI function, but lets
        // warn users here just in case they change the value and it's bigger
        // than it should be
        Serial.println("URI is too long ... must be less than 38 characters long");
        return;
        }

        // URI is within size limits ... write it to the card and report success/failure
        success = nfc.mifareclassic_WriteNDEFURI(1, ndefprefix, url);
        if (success)
        {
        Serial.println("NDEF URI Record written to sector 1");
        }
        else
        {
        Serial.println("NDEF Record creation failed! :(");
        }
        }

        // Wait a bit before trying again
        Serial.println("\n\nDone!");
        delay(1000);
        Serial.flush();
        while(Serial.available()) Serial.read();

}
