/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-Hal.c
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */
#include "ebox.h"
#include "platform.h"
#include "sx1276-Hal.h"


#if defined( USE_SX1276_RADIO )


#define RST_PIN PA2
#define NSS_PIN PA4

#define DIO0_PIN PA3
#define DIO1_PIN PB2
#define DIO2_PIN PB3
#define DIO3_PIN PB4
#define DIO4_PIN PB5
#define DIO5_PIN PB6

SPI_CONFIG_TYPE sx1276_spi_config;
void SX1276InitIo( void )
{
    
    // Configure NSS as output
    NSS_PIN.mode(OUTPUT_PP);

    // Configure radio DIO as inputs float

    
    // Configure DIO0
    DIO0_PIN.mode(INPUT);

    
    // Configure DIO1
        DIO1_PIN.mode(INPUT);

    // Configure DIO2
        DIO2_PIN.mode(INPUT);

    // REAMARK: DIO3/4/5 configured are connected to IO expander

    // Configure DIO3 as input
        DIO3_PIN.mode(INPUT);

    // Configure DIO4 as input
    
    // Configure DIO5 as input
    
    //init spi
    sx1276_spi_config.dev_num = 1;
    sx1276_spi_config.mode = SPI_MODE0;
    sx1276_spi_config.prescaler = SPI_CLOCK_DIV16;
    sx1276_spi_config.bit_order = SPI_BITODER_MSB;

    spi1.begin(&sx1276_spi_config);
}

void SX1276SetReset( uint8_t state )
{

    if( state == RADIO_RESET_ON )
    {
        // Set RESET pin to 0
        RST_PIN.mode(OUTPUT_PP);
        RST_PIN.reset();
    }
    else
    {
        // Set RESET pin as input pull down
        RST_PIN.mode(INPUT_PD);        
    }
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    NSS_PIN.reset();
    spi1.take_spi_right(&sx1276_spi_config);
    spi1.transfer( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        spi1.transfer( buffer[i] );
    }
    spi1.release_spi_right();
    //NSS = 1;
    NSS_PIN.set();
}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    NSS_PIN.reset();

    spi1.take_spi_right(&sx1276_spi_config);
    spi1.transfer( addr & 0x7F );
    for( i = 0; i < size; i++ )
    {
        buffer[i] = spi1.transfer( 0 );
    }
    spi1.release_spi_right();

    //NSS = 1;
    NSS_PIN.set();
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}

inline uint8_t SX1276ReadDio0( void )
{
    return DIO0_PIN.read();
}

inline uint8_t SX1276ReadDio1( void )
{
    return DIO1_PIN.read();
}

inline uint8_t SX1276ReadDio2( void )
{
    return DIO2_PIN.read();
}

inline uint8_t SX1276ReadDio3( void )
{
    return DIO3_PIN.read();
}

inline uint8_t SX1276ReadDio4( void )
{
    return DIO4_PIN.read();
}

inline uint8_t SX1276ReadDio5( void )
{
    return DIO5_PIN.read();
}

inline void SX1276WriteRxTx( uint8_t txEnable )
{
    if( txEnable != 0 )
    {
//        IoePinOn( FEM_CTX_PIN );
//        IoePinOff( FEM_CPS_PIN );
    }
    else
    {
//        IoePinOff( FEM_CTX_PIN );
//        IoePinOn( FEM_CPS_PIN );
    }
}

#endif // USE_SX1276_RADIO
