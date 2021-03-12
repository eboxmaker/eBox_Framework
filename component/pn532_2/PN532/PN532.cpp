/**************************************************************************/
/*! 
    @file     PN532.cpp
    @author   Adafruit Industries
	@license  BSD (see license.txt)
	
	I2C Driver for NXP's PN532 NFC/13.56MHz RFID Transceiver

	This is a library for the Adafruit PN532 NFC/RFID shields
	This library works with the Adafruit NFC breakout 
	----> https://www.adafruit.com/products/364
	
	Check out the links above for our tutorials and wiring diagrams 
	These chips use I2C to communicate
	
	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	@section  HISTORY

    v1.4 - Added setPassiveActivationRetries()
	
    v1.3 - Modified to work with I2C
	
    v1.2 - Added writeGPIO()
         - Added readGPIO()

    v1.1 - Changed readPassiveTargetID() to handle multiple UID sizes
         - Added the following helper functions for text display
             static void PrintHex(const byte * data, const uint32_t numBytes)
             static void PrintHexChar(const byte * pbtData, const uint32_t numBytes)
         - Added the following Mifare Classic functions:
             bool mifareclassic_IsFirstBlock (uint32_t uiBlock)
             bool mifareclassic_IsTrailerBlock (uint32_t uiBlock)
             uint8_t mifareclassic_AuthenticateBlock (uint8_t * uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t * keyData)
             uint8_t mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t * data)
             uint8_t mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t * data)
         - Added the following Mifare Ultalight functions:
             uint8_t mifareultralight_ReadPage (uint8_t page, uint8_t * buffer)	
*/
/**************************************************************************/

#include "PN532.h"

#include "debug.h"

#define HAL(func)   (_interface->func)

byte pn532response_firmwarevers[] = {0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03};

// Uncomment these lines to enable debug output for PN532(I2C) and/or MIFARE related code
#define PN532DEBUG
// #define MIFAREDEBUG

#define PN532_PACKBUFFSIZ 128
byte pn532_packetbuffer[PN532_PACKBUFFSIZ];

    
PN532::PN532(PN532Interface &interface) {
  _interface = &interface;
}



/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
void PN532::begin() {
    HAL(begin)();
    HAL(wakeup)();
}
 
/**************************************************************************/
/*! 
    @brief  Prints a hexadecimal value in plain characters

    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void PN532::PrintHex(const byte * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++) 
  {
    uart1.print("0x");
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      uart1.print("0");
    uart1.print(data[szPos]&0xff, HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      uart1.print(" ");
    }
  }
  uart1.println("");
}

/**************************************************************************/
/*! 
    @brief  Prints a hexadecimal value in plain characters, along with
            the char equivalents in the following format

            00 00 00 00 00 00  ......

    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void PN532::PrintHexChar(const byte * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++) 
  {
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      uart1.print("0");
    uart1.print(data[szPos], HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      uart1.print(" ");
    }
  }
  uart1.print("  ");
  for (szPos=0; szPos < numBytes; szPos++) 
  {
    if (data[szPos] <= 0x1F)
      uart1.print(".");
    else
      uart1.print((char)data[szPos]);
  }
  uart1.println("");
}
 
/**************************************************************************/
/*! 
    @brief  Checks the firmware version of the PN5xx chip

    @returns  The chip's firmware version and ID
*/
/**************************************************************************/
uint32_t PN532::getFirmwareVersion(void) {
  uint32_t response;

  pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

  if (HAL(writeCommand)(pn532_packetbuffer, 1))
    return 0;
	
  // read data packet
  int16_t status = HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));
  if (0 > status) {
    DMSG("GetFirmwareVersion: error code - ");
    DMSG(status);
    DMSG('\n');
    return 0;
  }
  
  response = pn532_packetbuffer[0];
  response <<= 8;
  response |= pn532_packetbuffer[1];
  response <<= 8;
  response |= pn532_packetbuffer[2];
  response <<= 8;
  response |= pn532_packetbuffer[3];

  return response;
}


/**************************************************************************/
/*! 
    Writes an 8-bit value that sets the state of the PN532's GPIO pins
    
    @warning This function is provided exclusively for board testing and
             is dangerous since it will throw an error if any pin other
             than the ones marked "Can be used as GPIO" are modified!  All
             pins that can not be used as GPIO should ALWAYS be left high
             (value = 1) or the system will become unstable and a HW reset
             will be required to recover the PN532.
    
             pinState[0]  = P30     Can be used as GPIO
             pinState[1]  = P31     Can be used as GPIO
             pinState[2]  = P32     *** RESERVED (Must be 1!) ***
             pinState[3]  = P33     Can be used as GPIO
             pinState[4]  = P34     *** RESERVED (Must be 1!) ***
             pinState[5]  = P35     Can be used as GPIO
    
    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
boolean PN532::writeGPIO(uint8_t pinstate) {
  uint8_t errorbit;

  // Make sure pinstate does not try to toggle P32 or P34
  pinstate |= (1 << PN532_GPIO_P32) | (1 << PN532_GPIO_P34);
  
  // Fill command buffer
  pn532_packetbuffer[0] = PN532_COMMAND_WRITEGPIO;
  pn532_packetbuffer[1] = PN532_GPIO_VALIDATIONBIT | pinstate;  // P3 Pins
  pn532_packetbuffer[2] = 0x00;    // P7 GPIO Pins (not used ... taken by I2C)

  DMSG("Writing P3 GPIO: ");
  DMSG_HEX(pn532_packetbuffer[1]);

  // Send the WRITEGPIO command (0x0E)  
  if (HAL(writeCommand)(pn532_packetbuffer, 3))
    return 0;
  
  return (0 < HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer)));
}

/**************************************************************************/
/*! 
    Reads the state of the PN532's GPIO pins
    
    @returns An 8-bit value containing the pin state where:
    
             pinState[0]  = P30     
             pinState[1]  = P31     
             pinState[2]  = P32     
             pinState[3]  = P33     
             pinState[4]  = P34     
             pinState[5]  = P35     
*/
/**************************************************************************/
uint8_t PN532::readGPIO(void) {
  pn532_packetbuffer[0] = PN532_COMMAND_READGPIO;

  // Send the READGPIO command (0x0C)  
  if (HAL(writeCommand)(pn532_packetbuffer, 1))
    return 0x0;
  
  HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

  /* READGPIO response without prefix and suffix should be in the following format:
  
    byte            Description
    -------------   ------------------------------------------
    b0              P3 GPIO Pins
    b1              P7 GPIO Pins (not used ... taken by I2C)
    b2              Interface Mode Pins (not used ... bus select pins) 
  */
  

  DMSG("P3 GPIO: "); DMSG_HEX(pn532_packetbuffer[7]);
  DMSG("P7 GPIO: "); DMSG_HEX(pn532_packetbuffer[8]);
  DMSG("I0I1 GPIO: "); DMSG_HEX(pn532_packetbuffer[9]);
    
  #ifdef DEBUG
    // Note: You can use the IO GPIO value to detect the serial bus being used
    switch(pn532_packetbuffer[3])
    {
      case 0x00:    // Using UART
        uart1.println("Using UART (IO = 0x00)");
        break;
      case 0x01:    // Using I2C 
        uart1.println("Using I2C (IO = 0x01)");
        break;
      case 0x02:    // Using I2C
        uart1.println("Using I2C (IO = 0x02)");
        break;
    }
  #endif

  return pn532_packetbuffer[0];
}

/**************************************************************************/
/*! 
    @brief  Configures the SAM (Secure Access Module)
*/
/**************************************************************************/
boolean PN532::SAMConfig(void) {
  pn532_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
  pn532_packetbuffer[1] = 0x01; // normal mode;
  pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
  pn532_packetbuffer[3] = 0x01; // use IRQ pin!
  
  if (HAL(writeCommand)(pn532_packetbuffer, 4))
     return false;

  return (0 < HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer)));
}

/**************************************************************************/
/*! 
    Sets the MxRtyPassiveActivation byte of the RFConfiguration register
    
    @param  maxRetries    0xFF to wait forever, 0x00..0xFE to timeout
                          after mxRetries
    
    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
boolean PN532::setPassiveActivationRetries(uint8_t maxRetries) {
  pn532_packetbuffer[0] = PN532_COMMAND_RFCONFIGURATION;
  pn532_packetbuffer[1] = 5;    // Config item 5 (MaxRetries)
  pn532_packetbuffer[2] = 0xFF; // MxRtyATR (default = 0xFF)
  pn532_packetbuffer[3] = 0x01; // MxRtyPSL (default = 0x01)
  pn532_packetbuffer[4] = maxRetries;

  DMSG("Setting MxRtyPassiveActivation to ");
  DMSG(maxRetries);
  DMSG('\n');
  
  if (HAL(writeCommand)(pn532_packetbuffer, 5))
    return 0x0;  // no ACK
  
  return 1;
}

/***** ISO14443A Commands ******/

/**************************************************************************/
/*! 
    Waits for an ISO14443A target to enter the field
    
    @param  cardBaudRate  Baud rate of the card
    @param  uid           Pointer to the array that will be populated
                          with the card's UID (up to 7 bytes)
    @param  uidLength     Pointer to the variable that will hold the
                          length of the card's UID.
    
    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
boolean PN532::readPassiveTargetID(uint8_t cardbaudrate, uint8_t * uid, uint8_t * uidLength) {
  pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
  pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this to 2 later)
  pn532_packetbuffer[2] = cardbaudrate;
  
  if (HAL(writeCommand)(pn532_packetbuffer, 3))
  {
	DMSG("No card(s) read");
    return 0x0;  // no cards read
  }
  uart1.flush();
  // Wait for a card to enter the field
  uint8_t status = PN532_I2C_BUSY;
 
  // read data packet
  HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));
  
  // check some basic stuff
  /* ISO14443A card response should be in the following format:
  
    byte            Description
    -------------   ------------------------------------------
    b0..6           Frame header and preamble
    b7              Tags Found
    b8              Tag Number (only one used in this example)
    b9..10          SENS_RES
    b11             SEL_RES
    b12             NFCID Length
    b13..NFCIDLen   NFCID                                      */
  
  DMSG("Found "); DMSG(pn532_packetbuffer[0]); DMSG(" tags\n");

  if (pn532_packetbuffer[0] != 1) 
    return 0;
    
  uint16_t sens_res = pn532_packetbuffer[2];
  sens_res <<= 8;
  sens_res |= pn532_packetbuffer[3];

  DMSG("ATQA: 0x");  DMSG_HEX(sens_res); 
  DMSG("SAK: 0x");  DMSG_HEX(pn532_packetbuffer[4]); 
  
  /* Card appears to be Mifare Classic */
  *uidLength = pn532_packetbuffer[5];

  DMSG("UID:"); 
  for (uint8_t i=0; i < pn532_packetbuffer[5]; i++) 
  {
    uid[i] = pn532_packetbuffer[6+i];
    DMSG(uid[i]); 
  }
  DMSG('\n');

  return 1;
}


/***** Mifare Classic Functions ******/

/**************************************************************************/
/*! 
      Indicates whether the specified block number is the first block
      in the sector (block 0 relative to the current sector)
*/
/**************************************************************************/
bool PN532::mifareclassic_IsFirstBlock (uint32_t uiBlock)
{
  // Test if we are in the small or big sectors
  if (uiBlock < 128)
    return ((uiBlock) % 4 == 0);
  else
    return ((uiBlock) % 16 == 0);
}

/**************************************************************************/
/*! 
      Indicates whether the specified block number is the sector trailer
*/
/**************************************************************************/
bool PN532::mifareclassic_IsTrailerBlock (uint32_t uiBlock)
{
  // Test if we are in the small or big sectors
  if (uiBlock < 128)
    return ((uiBlock + 1) % 4 == 0);
  else
    return ((uiBlock + 1) % 16 == 0);
}

/**************************************************************************/
/*! 
    Tries to authenticate a block of memory on a MIFARE card using the
    INDATAEXCHANGE command.  See section 7.3.8 of the PN532 User Manual
    for more information on sending MIFARE and other commands.

    @param  uid           Pointer to a byte array containing the card UID
    @param  uidLen        The length (in bytes) of the card's UID (Should
                          be 4 for MIFARE Classic)
    @param  blockNumber   The block number to authenticate.  (0..63 for
                          1KB cards, and 0..255 for 4KB cards).
    @param  keyNumber     Which key type to use during authentication
                          (0 = MIFARE_CMD_AUTH_A, 1 = MIFARE_CMD_AUTH_B)
    @param  keyData       Pointer to a byte array containing the 6 byte
                          key value
    
    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t PN532::mifareclassic_AuthenticateBlock (uint8_t * uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t * keyData)
{
  uint8_t len;
  uint8_t i;
  
  // Hang on to the key and uid data
  memcpy (_key, keyData, 6); 
  memcpy (_uid, uid, uidLen); 
  _uidLen = uidLen;  

  #ifdef MIFAREDEBUG
  uart1.print("Trying to authenticate card ");
  PN532::PrintHex(_uid, _uidLen);
  uart1.print("Using authentication KEY ");uart1.print(keyNumber ? 'B' : 'A');uart1.print(": ");
  PN532::PrintHex(_key, 6);
  #endif
  
  // Prepare the authentication command //
  pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;   /* Data Exchange Header */
  pn532_packetbuffer[1] = 1;                              /* Max card numbers */
  pn532_packetbuffer[2] = (keyNumber) ? MIFARE_CMD_AUTH_B : MIFARE_CMD_AUTH_A;
  pn532_packetbuffer[3] = blockNumber;                    /* Block Number (1K = 0..63, 4K = 0..255 */
  memcpy (pn532_packetbuffer+4, _key, 6);
  for (i = 0; i < _uidLen; i++)
  {
    pn532_packetbuffer[10+i] = _uid[i];                /* 4 byte card ID */
  }

  if (HAL(writeCommand)(pn532_packetbuffer, 10+_uidLen))
    return 0;

  // Read the response packet
  HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));
  
  // Check if the response is valid and we are authenticated???
  // for an auth success it should be bytes 5-7: 0xD5 0x41 0x00
  // Mifare auth error is technically byte 7: 0x14 but anything other and 0x00 is not good
  if (pn532_packetbuffer[0] != 0x00)
  {
    DMSG("Authentification failed\n");
    return 0;
  }  
  
  return 1;
}

/**************************************************************************/
/*! 
    Tries to read an entire 16-byte data block at the specified block
    address.

    @param  blockNumber   The block number to authenticate.  (0..63 for
                          1KB cards, and 0..255 for 4KB cards).
    @param  data          Pointer to the byte array that will hold the
                          retrieved data (if any)
    
    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t PN532::mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t * data)
{
  #ifdef MIFAREDEBUG
  uart1.print("Trying to read 16 bytes from block ");uart1.println(blockNumber);
  #endif
  
  /* Prepare the command */
  pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
  pn532_packetbuffer[1] = 1;                      /* Card number */
  pn532_packetbuffer[2] = MIFARE_CMD_READ;        /* Mifare Read command = 0x30 */
  pn532_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */

  /* Send the command */
  if (HAL(writeCommand)(pn532_packetbuffer, 4))
  {
    #ifdef MIFAREDEBUG
    uart1.println("Failed to receive ACK for read command");
    #endif
    return 0;
  }

  /* Read the response packet */
  HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

  /* If byte 8 isn't 0x00 we probably have an error */
  if (pn532_packetbuffer[0] != 0x00)
  {
    return 0;
  }
    
  /* Copy the 16 data bytes to the output buffer        */
  /* Block content starts at byte 9 of a valid response */
  memcpy (data, pn532_packetbuffer+1, 16);

  return 1;  
}

/**************************************************************************/
/*! 
    Tries to write an entire 16-byte data block at the specified block
    address.

    @param  blockNumber   The block number to authenticate.  (0..63 for
                          1KB cards, and 0..255 for 4KB cards).
    @param  data          The byte array that contains the data to write.
    
    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t PN532::mifareclassic_WriteDataBlock (uint8_t blockNumber, uint8_t * data)
{
  DMSG("Trying to write 16 bytes to block ");
  DMSG(blockNumber);
  DMSG('\n');
  
  /* Prepare the first command */
  pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
  pn532_packetbuffer[1] = 1;                      /* Card number */
  pn532_packetbuffer[2] = MIFARE_CMD_WRITE;       /* Mifare Write command = 0xA0 */
  pn532_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */
  memcpy (pn532_packetbuffer+4, data, 16);          /* Data Payload */

  /* Send the command */
  if (HAL(writeCommand)(pn532_packetbuffer, 20))
  {
    DMSG("Failed to receive ACK for write command");
    return 0;
  }
  
  /* Read the response packet */
  return (0 < HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer)));
}

/**************************************************************************/
/*! 
    Formats a Mifare Classic card to store NDEF Records 
    
    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t PN532::mifareclassic_FormatNDEF (void)
{
  uint8_t sectorbuffer1[16] = {0x14, 0x01, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1};
  uint8_t sectorbuffer2[16] = {0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1};
  uint8_t sectorbuffer3[16] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x78, 0x77, 0x88, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  // Note 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 must be used for key A
  // for the MAD sector in NDEF records (sector 0)
  
  // Write block 1 and 2 to the card
  if (!(mifareclassic_WriteDataBlock (1, sectorbuffer1)))
    return 0;
  if (!(mifareclassic_WriteDataBlock (2, sectorbuffer2)))
    return 0;
  // Write key A and access rights card
  if (!(mifareclassic_WriteDataBlock (3, sectorbuffer3)))
    return 0;

  // Seems that everything was OK (?!)
  return 1;
}

/**************************************************************************/
/*! 
    Writes an NDEF URI Record to the specified sector (1..15)
    
    Note that this function assumes that the Mifare Classic card is
    already formatted to work as an "NFC Forum Tag" and uses a MAD1
    file system.  You can use the NXP TagWriter app on Android to
    properly format cards for this.

    @param  sectorNumber  The sector that the URI record should be written
                          to (can be 1..15 for a 1K card)
    @param  uriIdentifier The uri identifier code (0 = none, 0x01 = 
                          "http://www.", etc.)
    @param  url           The uri text to write (max 38 characters).
    
    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t PN532::mifareclassic_WriteNDEFURI (uint8_t sectorNumber, uint8_t uriIdentifier, const char * url)
{
  // Figure out how long the string is
  uint8_t len = strlen(url);
  
  // Make sure we're within a 1K limit for the sector number
  if ((sectorNumber < 1) || (sectorNumber > 15))
    return 0;
  
  // Make sure the URI payload is between 1 and 38 chars
  if ((len < 1) || (len > 38))
    return 0;
    
  // Note 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 must be used for key A
  // in NDEF records
	
  // Setup the sector buffer (w/pre-formatted TLV wrapper and NDEF message)
  uint8_t sectorbuffer1[16] = {0x00, 0x00, 0x03, len+5, 0xD1, 0x01, len+1, 0x55, uriIdentifier, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t sectorbuffer2[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t sectorbuffer3[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t sectorbuffer4[16] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 0x07, 0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  if (len <= 6)
  {
    // Unlikely we'll get a url this short, but why not ...
    memcpy (sectorbuffer1+9, url, len);
    sectorbuffer1[len+9] = 0xFE;
  }
  else if (len == 7)
  {
    // 0xFE needs to be wrapped around to next block
    memcpy (sectorbuffer1+9, url, len);
    sectorbuffer2[0] = 0xFE;
  }
  else if ((len > 7) || (len <= 22))
  {
    // Url fits in two blocks
    memcpy (sectorbuffer1+9, url, 7);
    memcpy (sectorbuffer2, url+7, len-7);
    sectorbuffer2[len-7] = 0xFE;
  }
  else if (len == 23)
  {
    // 0xFE needs to be wrapped around to final block
    memcpy (sectorbuffer1+9, url, 7);
    memcpy (sectorbuffer2, url+7, len-7);
    sectorbuffer3[0] = 0xFE;
  }
  else
  {
    // Url fits in three blocks
    memcpy (sectorbuffer1+9, url, 7);
    memcpy (sectorbuffer2, url+7, 16);
    memcpy (sectorbuffer3, url+23, len-24);
    sectorbuffer3[len-22] = 0xFE;
  }
  
  // Now write all three blocks back to the card
  if (!(mifareclassic_WriteDataBlock (sectorNumber*4, sectorbuffer1)))
    return 0;
  if (!(mifareclassic_WriteDataBlock ((sectorNumber*4)+1, sectorbuffer2)))
    return 0;
  if (!(mifareclassic_WriteDataBlock ((sectorNumber*4)+2, sectorbuffer3)))
    return 0;
  if (!(mifareclassic_WriteDataBlock ((sectorNumber*4)+3, sectorbuffer4)))
    return 0;

  // Seems that everything was OK (?!)
  return 1;
}

/***** Mifare Ultralight Functions ******/

/**************************************************************************/
/*! 
    Tries to read an entire 4-byte page at the specified address.

    @param  page        The page number (0..63 in most cases)
    @param  buffer      Pointer to the byte array that will hold the
                        retrieved data (if any)
*/
/**************************************************************************/
uint8_t PN532::mifareultralight_ReadPage (uint8_t page, uint8_t * buffer)
{
  if (page >= 64)
  {
    DMSG("Page value out of range\n");
    return 0;
  }

  DMSG("Reading page ");
  DMSG(page);
  DMSG('\n');

  /* Prepare the command */
  pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
  pn532_packetbuffer[1] = 1;                   /* Card number */
  pn532_packetbuffer[2] = MIFARE_CMD_READ;     /* Mifare Read command = 0x30 */
  pn532_packetbuffer[3] = page;                /* Page Number (0..63 in most cases) */

  /* Send the command */
  if (HAL(writeCommand)(pn532_packetbuffer, 4))
  {
    DMSG("Failed to receive ACK for write command\n");
    return 0;
  }
  
  /* Read the response packet */
  HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer));

  /* If byte 8 isn't 0x00 we probably have an error */
  if (pn532_packetbuffer[0] == 0x00)
  {
    /* Copy the 4 data bytes to the output buffer         */
    /* Block content starts at byte 9 of a valid response */
    /* Note that the command actually reads 16 byte or 4  */
    /* pages at a time ... we simply discard the last 12  */
    /* bytes                                              */
    memcpy (buffer, pn532_packetbuffer+1, 4);
  }
  else
  {
    return 0;
  }

  // Return OK signal
  return 1;
}

/**************************************************************************/
/*! 
    @brief  Exchanges an APDU with the currently inlisted peer

    @param  send            Pointer to data to send
    @param  sendLength      Length of the data to send
    @param  response        Pointer to response data
    @param  responseLength  Pointer to the response data length
*/
/**************************************************************************/
boolean PN532::inDataExchange(uint8_t * send, uint8_t sendLength, uint8_t * response, uint8_t * responseLength) {
  if (sendLength > PN532_PACKBUFFSIZ -2) {
    #ifdef PN532DEBUG
      uart1.println("APDU length too long for packet buffer");
    #endif
    return false;
  }
  uint8_t i;
  
  pn532_packetbuffer[0] = 0x40; // PN532_COMMAND_INDATAEXCHANGE;
  pn532_packetbuffer[1] = inListedTag;
  for (i=0; i<sendLength; ++i) {
    pn532_packetbuffer[i+2] = send[i];
  }
  
  if (HAL(writeCommand)(pn532_packetbuffer,sendLength+2)) {
    DMSG("Could not send ADPU\n");
    return false;
  }


  int16_t status = HAL(readResponse)(pn532_packetbuffer,sizeof(pn532_packetbuffer), 1000);
  if (status < 0) {
    return false;
  }
  
  uint8_t length = status;

  if ((pn532_packetbuffer[0] & 0x3f)!=0) {
    DMSG("Status code indicates an error\n");
    return false;
  }
  
  length -= 1;
  
  if (length > *responseLength) {
    length = *responseLength; // silent truncation...
  }
  
  for (i=0; i<length; ++i) {
    response[i] = pn532_packetbuffer[1+i];
  }
  *responseLength = length;
  
  return true;
}

/**************************************************************************/
/*! 
    @brief  'InLists' a passive target. PN532 acting as reader/initiator,
            peer acting as card/responder.
*/
/**************************************************************************/
boolean PN532::inListPassiveTarget() {
  pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
  pn532_packetbuffer[1] = 1;
  pn532_packetbuffer[2] = 0;
  
  DMSG("About to inList passive target");

  if (HAL(writeCommand)(pn532_packetbuffer, 3)) {
    DMSG("Could not send inlist message\n");
    return false;
  }

  int16_t status = HAL(readResponse)(pn532_packetbuffer,sizeof(pn532_packetbuffer), 30000);
  if (status < 0) {
    return false;
  }

  if (pn532_packetbuffer[0] != 1) {
    return false;
  }
  
  inListedTag = pn532_packetbuffer[1];
  DMSG("Tag number: ");
  DMSG(inListedTag);
  DMSG('\n');
  
  return true;
}

/**
 * Peer to Peer
 */
int8_t PN532::tgInitAsTarget()
{
    static const uint8_t command[] = {
            PN532_COMMAND_TGINITASTARGET,
            0,
            0x00, 0x00,         //SENS_RES
            0x00, 0x00, 0x00,   //NFCID1
            0x40,               //SEL_RES

            0x01, 0xFE, 0x0F, 0xBB, 0xBA, 0xA6, 0xC9, 0x89, // POL_RES
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xFF, 0xFF,

            0x01, 0xFE, 0x0F, 0xBB, 0xBA, 0xA6, 0xC9, 0x89, 0x00, 0x00, //NFCID3t: Change this to desired value

            0x06, 0x46,  0x66, 0x6D, 0x01, 0x01, 0x10, 0x00// LLCP magic number and version parameter
            };
    
    int8_t status = HAL(writeCommand)(command, sizeof(command));
    if (status < 0) {
        return status;
    }
    
    return HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer), 0);
}

int16_t PN532::tgGetData(uint8_t *buf, uint16_t len)
{
    pn532_packetbuffer[0] = PN532_COMMAND_TGGETDATA;
    
    if (HAL(writeCommand)(pn532_packetbuffer, 1)) {
        DMSG("TgGetData: no ACK\n");
        return -1;
    }
    
    int16_t status = HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer), 3000);
    if (0 > status) {
        DMSG("TgGetData: failed to read response, error code - ");
        DMSG(status);
        DMSG('\n');
        return status;
    }
    
    uint16_t length = status;
    if (length > len) {
        return -4;
    }
    
    if (pn532_packetbuffer[0] != 0) {
        DMSG("status is not ok\n");
        return -5;
    }
    
    memcpy(buf, pn532_packetbuffer + 1, length - 1);

    
    return length - 1;
}

boolean PN532::tgSetData(const uint8_t *buf, uint16_t len)
{
    pn532_packetbuffer[0] = PN532_COMMAND_TGSETDATA;
    if (len > (sizeof(pn532_packetbuffer) + 1)) {
        return false;
    }
    
    memcpy(pn532_packetbuffer + 1, buf, len);
    
    if (HAL(writeCommand)(pn532_packetbuffer, len + 1)) {
        return false;
    }
    
    if (0 > HAL(readResponse)(pn532_packetbuffer, sizeof(pn532_packetbuffer), 3000)) {
        return false;
    }
    
    if (0 != pn532_packetbuffer[0]) {
        return false;
    }
    
    return true;
}


    
    