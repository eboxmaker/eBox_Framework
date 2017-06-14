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
#include "bsp.h"
#include "sx1278.h"
#include "radio.h"
#include "sx1276-lora.h"


#define BUFFER_SIZE                                 9 // Define the payload size here


static uint16_t BufferSize = BUFFER_SIZE;			// RF buffer size
static uint8_t Buffer[BUFFER_SIZE];					// RF buffer

static uint8_t EnableMaster = true; 				// Master/Slave selection

tRadioDriver *Radio = NULL;

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";
/*
 * Manages the master operation
 */
void OnMaster( void )
{
    uint8_t i;
    
    switch( Radio->Process( ) )
    {
    case RF_RX_TIMEOUT:
        // Send the next PING frame
        Buffer[0] = 'P';
        Buffer[1] = 'I';
        Buffer[2] = 'N';
        Buffer[3] = 'G';
        for( i = 4; i < BufferSize; i++ )
        {
            Buffer[i] = i - 4;
        }
        Radio->SetTxPacket( Buffer, BufferSize );
        break;
    case RF_RX_DONE:
        Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
    
        if( BufferSize > 0 )
        {
            if( strncmp( ( const char* )Buffer, ( const char* )PongMsg, 4 ) == 0 )
            {
                // Indicates on a LED that the received frame is a PONG
                //LedToggle( LED_GREEN );
                PB8.toggle();

                // Send the next PING frame            
                Buffer[0] = 'P';
                Buffer[1] = 'I';
                Buffer[2] = 'N';
                Buffer[3] = 'G';
                // We fill the buffer with numbers for the payload 
                for( i = 4; i < BufferSize; i++ )
                {
                    Buffer[i] = i - 4;
                }
                Radio->SetTxPacket( Buffer, BufferSize );
            }
            else if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
            { // A master already exists then become a slave
                EnableMaster = false;
                //LedOff( LED_RED );
                PB9.reset();
            }
            
            uart1.printf("RSSI::%3.2F\r\n",SX1276LoRaGetPacketRssi());
        }            
        break;
    case RF_TX_DONE:
        // Indicates on a LED that we have sent a PING
        //LedToggle( LED_RED );
        //PB9.toggle();
        Radio->StartRx( );
        break;
    default:
        break;
    }
}
/*
 * Manages the slave operation
 */
void OnSlave( void )
{
    uint8_t i;

    switch( Radio->Process( ) )
    {
    case RF_RX_DONE:
        Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
    
        if( BufferSize > 0 )
        {
            if( strncmp( ( const char* )Buffer, ( const char* )PingMsg, 4 ) == 0 )
            {
                // Indicates on a LED that the received frame is a PING
                //LedToggle( LED_GREEN );
                PB8.toggle();

               // Send the reply to the PONG string
                Buffer[0] = 'P';
                Buffer[1] = 'O';
                Buffer[2] = 'N';
                Buffer[3] = 'G';
                // We fill the buffer with numbers for the payload 
                for( i = 4; i < BufferSize; i++ )
                {
                    Buffer[i] = i - 4;
                }

                Radio->SetTxPacket( Buffer, BufferSize );
            }
            uart1.printf("RSSI::%3.2F\r\n",SX1276LoRaGetPacketRssi());
        }
        break;
    case RF_TX_DONE:
        // Indicates on a LED that we have sent a PONG
        //LedToggle( LED_RED );
       // PB9.toggle();
        Radio->StartRx( );
        break;
    default:
        break;
    }
}
Lora lora(&PA4,&PA2,&spi1);

void setup()
{
    int ret;
    ebox_init();
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    uart1.begin(115200);
//    lora.begin(1,SX1278_BW_6,SX1278_CR_4_5,SX1278_SF_12);
//    
//    ex1.begin();
//    ex1.attach(test);
//    ex2.begin();
//    ex2.attach(timeout);
//    
//    #if TXMODE
//        lora.enttry_tx();
//    #else
//        lora.enttry_rx();
//    #endif
    
    Radio = RadioDriverInit( );
    Radio->Init( );
    Radio->StartRx( );
     uart1.printf("SX1278_REG_FRF_MSB:0x%x\n",lora.readRegister(SX1278_REG_FRF_MSB));
     uart1.printf("SX1278_REG_FRF_MID:0x%x\n",lora.readRegister(SX1278_REG_FRF_MID));
     uart1.printf("SX1278_REG_FRF_LSB:0x%x\n",lora.readRegister(SX1278_REG_FRF_LSB));
    
    uart1.printf("SX1278_REG_MODEM_CONFIG_2:0x%x\n",lora.readRegister(SX1278_REG_MODEM_CONFIG_2));
    
    
    
}
uint32_t last;
uint8_t temp;
int main(void)
{
    int temp;
    setup();
    while( 1 )
    {
        if( EnableMaster == true )
        {
            OnMaster( );
        }
        else
        {
            OnSlave( );
        }    
#if( PLATFORM == SX12xxEiger ) && ( USE_UART == 1 )

        UartProcess( );
        
        {
            uint8_t data = 0;
            if( UartGetChar( &data ) == UART_OK )
            {
                UartPutChar( data );
            }
        }
#endif        
    }
#ifdef __GNUC__
    return 0;
#endif


}
