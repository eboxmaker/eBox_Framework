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
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t data[16];                         // Array to store block data during reads

  // Keyb on NDEF and Mifare Classic should be the same
  uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ...
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

      // Now we try to go through all 16 sectors (each having 4 blocks)
      // authenticating each sector, and then dumping the blocks
      for (currentblock = 0; currentblock < 64; currentblock++)
      {
        // Check if this is a new block so that we can reauthenticate
        if (nfc.mifareclassic_IsFirstBlock(currentblock)) authenticated = false;

        // If the sector hasn't been authenticated, do so first
        if (!authenticated)
        {
          // Starting of a new sector ... try to to authenticate
          Serial.print("------------------------Sector ");Serial.print(currentblock/4, DEC);Serial.println("-------------------------");
          if (currentblock == 0)
          {
              // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
              // or 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 for NDEF formatted cards using key a,
              // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, keyuniversal);
          }
          else
          {
              // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
              // or 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 for NDEF formatted cards using key a,
              // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, keyuniversal);
          }
          if (success)
          {
            authenticated = true;
          }
          else
          {
            Serial.println("Authentication error");
          }
        }
        // If we're still not authenticated just skip the block
        if (!authenticated)
        {
          Serial.print("Block ");Serial.print(currentblock, DEC);Serial.println(" unable to authenticate");
        }
        else
        {
          // Authenticated ... we should be able to read the block now
          // Dump the data into the 'data' array
          success = nfc.mifareclassic_ReadDataBlock(currentblock, data);
          if (success)
          {
            // Read successful
            Serial.print("Block ");Serial.print(currentblock, DEC);
            if (currentblock < 10)
            {
              Serial.print("  ");
            }
            else
            {
              Serial.print(" ");
            }
            // Dump the raw data
            nfc.PrintHexChar(data, 16);
          }
          else
          {
            // Oops ... something happened
            Serial.print("Block ");Serial.print(currentblock, DEC);
            Serial.println(" unable to read this block");
          }
        }
      }
    }
    else
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
    }
  }
  // Wait a bit before trying again
  Serial.println("\n\nSend a character to run the mem dumper again!");
  Serial.flush();
  while (!Serial.available());
  while (Serial.available()) {
  Serial.read();
  }
  Serial.flush();
  }
}
