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
const char * url = "adafruit.com/blog/";
uint8_t ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT;
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

    while(1)
    {
        test();
    }
}
void test()
{
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  bool authenticated = false;               // Flag to indicate if the sector is authenticated

  // Use the default NDEF keys (these would have have set by mifareclassic_formatndef.pde!)
  uint8_t keya[6] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
  uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };

  Serial.println("Place your NDEF formatted Mifare Classic card on the reader to update the");
  Serial.println("NDEF record and press any key to continue ...");
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
      return;
    }
    
    // We probably have a Mifare Classic card ... 
    Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

    // Check if this is an NDEF card (using first block of sector 1 from mifareclassic_formatndef.pde)
    // Must authenticate on the first key using 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7
    success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 0, keyb);
    if (!success)
    {
      Serial.println("Unable to authenticate block 4 ... is this card NDEF formatted?");
      return;
    }
    
    Serial.println("Authentication succeeded (seems to be an NDEF/NFC Forum tag) ...");

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
      Serial.println("URI is too long ... must be less than 38 characters!");
      return;
    }
    
    Serial.println("Updating sector 1 with URI as NDEF Message");
    
    // URI is within size limits ... write it to the card and report success/failure
    success = nfc.mifareclassic_WriteNDEFURI(1, ndefprefix, url);
    if (success)
    {
      Serial.println("NDEF URI Record written to sector 1");
      Serial.println("");      
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