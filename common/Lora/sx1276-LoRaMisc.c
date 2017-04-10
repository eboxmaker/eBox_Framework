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
 * \file       sx1276-LoRaMisc.c
 * \brief      SX1276 RF chip high level functions driver
 *
 * \remark     Optional support functions.
 *             These functions are defined only to easy the change of the
 *             parameters.
 *             For a final firmware the radio parameters will be known so
 *             there is no need to support all possible parameters.
 *             Removing these functions will greatly reduce the final firmware
 *             size.
 *
 * \version    2.0.0 
 * \date       May 6 2013
 * \author     Gregory Cristian
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */
#include "platform.h"

#if defined( USE_SX1276_RADIO )

#include "sx1276-Hal.h"
#include "sx1276.h"

#include "sx1276-LoRa.h"
#include "sx1276-LoRaMisc.h"

/*!
 * SX1276 definitions
 */
#define XTAL_FREQ                                   32000000
#define FREQ_STEP                                   61.03515625

extern tLoRaSettings LoRaSettings;

void SX1276LoRaSetRFFrequency( uint32_t freq )
{
    LoRaSettings.RFFrequency = freq;

    freq = ( uint32_t )( ( double )freq / ( double )FREQ_STEP );
    SX1276LR->RegFrfMsb = ( uint8_t )( ( freq >> 16 ) & 0xFF );
    SX1276LR->RegFrfMid = ( uint8_t )( ( freq >> 8 ) & 0xFF );
    SX1276LR->RegFrfLsb = ( uint8_t )( freq & 0xFF );
    SX1276WriteBuffer( REG_LR_FRFMSB, &SX1276LR->RegFrfMsb, 3 );
}

uint32_t SX1276LoRaGetRFFrequency( void )
{
    SX1276ReadBuffer( REG_LR_FRFMSB, &SX1276LR->RegFrfMsb, 3 );
    LoRaSettings.RFFrequency = ( ( uint32_t )SX1276LR->RegFrfMsb << 16 ) | ( ( uint32_t )SX1276LR->RegFrfMid << 8 ) | ( ( uint32_t )SX1276LR->RegFrfLsb );
    LoRaSettings.RFFrequency = ( uint32_t )( ( double )LoRaSettings.RFFrequency * ( double )FREQ_STEP );

    return LoRaSettings.RFFrequency;
}

void SX1276LoRaSetRFPower( int8_t power )
{
    SX1276Read( REG_LR_PACONFIG, &SX1276LR->RegPaConfig );
    SX1276Read( REG_LR_PADAC, &SX1276LR->RegPaDac );
    
    if( ( SX1276LR->RegPaConfig & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
    {
        if( ( SX1276LR->RegPaDac & 0x87 ) == 0x87 )
        {
            if( power < 5 )
            {
                power = 5;
            }
            if( power > 20 )
            {
                power = 20;
            }
            SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
            SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
        }
        else
        {
            if( power < 2 )
            {
                power = 2;
            }
            if( power > 17 )
            {
                power = 17;
            }
            SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
            SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
        }
    }
    else
    {
        if( power < -1 )
        {
            power = -1;
        }
        if( power > 14 )
        {
            power = 14;
        }
        SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
        SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power + 1 ) & 0x0F );
    }
    SX1276Write( REG_LR_PACONFIG, SX1276LR->RegPaConfig );
    LoRaSettings.Power = power;
}

int8_t SX1276LoRaGetRFPower( void )
{
    SX1276Read( REG_LR_PACONFIG, &SX1276LR->RegPaConfig );
    SX1276Read( REG_LR_PADAC, &SX1276LR->RegPaDac );

    if( ( SX1276LR->RegPaConfig & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
    {
        if( ( SX1276LR->RegPaDac & 0x07 ) == 0x07 )
        {
            LoRaSettings.Power = 5 + ( SX1276LR->RegPaConfig & ~RFLR_PACONFIG_OUTPUTPOWER_MASK );
        }
        else
        {
            LoRaSettings.Power = 2 + ( SX1276LR->RegPaConfig & ~RFLR_PACONFIG_OUTPUTPOWER_MASK );
        }
    }
    else
    {
        LoRaSettings.Power = -1 + ( SX1276LR->RegPaConfig & ~RFLR_PACONFIG_OUTPUTPOWER_MASK );
    }
    return LoRaSettings.Power;
}

void SX1276LoRaSetSignalBandwidth( uint8_t bw )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    SX1276LR->RegModemConfig1 = ( SX1276LR->RegModemConfig1 & RFLR_MODEMCONFIG1_BW_MASK ) | ( bw << 4 );
    SX1276Write( REG_LR_MODEMCONFIG1, SX1276LR->RegModemConfig1 );
    LoRaSettings.SignalBw = bw;
}

uint8_t SX1276LoRaGetSignalBandwidth( void )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    LoRaSettings.SignalBw = ( SX1276LR->RegModemConfig1 & ~RFLR_MODEMCONFIG1_BW_MASK ) >> 4;
    return LoRaSettings.SignalBw;
}

void SX1276LoRaSetSpreadingFactor( uint8_t factor )
{

    if( factor > 12 )
    {
        factor = 12;
    }
    else if( factor < 6 )
    {
        factor = 6;
    }

    if( factor == 6 )
    {
        SX1276LoRaSetNbTrigPeaks( 5 );
    }
    else
    {
        SX1276LoRaSetNbTrigPeaks( 3 );
    }

    SX1276Read( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2 );    
    SX1276LR->RegModemConfig2 = ( SX1276LR->RegModemConfig2 & RFLR_MODEMCONFIG2_SF_MASK ) | ( factor << 4 );
    SX1276Write( REG_LR_MODEMCONFIG2, SX1276LR->RegModemConfig2 );    
    LoRaSettings.SpreadingFactor = factor;
}

uint8_t SX1276LoRaGetSpreadingFactor( void )
{
    SX1276Read( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2 );   
    LoRaSettings.SpreadingFactor = ( SX1276LR->RegModemConfig2 & ~RFLR_MODEMCONFIG2_SF_MASK ) >> 4;
    return LoRaSettings.SpreadingFactor;
}

void SX1276LoRaSetErrorCoding( uint8_t value )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    SX1276LR->RegModemConfig1 = ( SX1276LR->RegModemConfig1 & RFLR_MODEMCONFIG1_CODINGRATE_MASK ) | ( value << 1 );
    SX1276Write( REG_LR_MODEMCONFIG1, SX1276LR->RegModemConfig1 );
    LoRaSettings.ErrorCoding = value;
}

uint8_t SX1276LoRaGetErrorCoding( void )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    LoRaSettings.ErrorCoding = ( SX1276LR->RegModemConfig1 & ~RFLR_MODEMCONFIG1_CODINGRATE_MASK ) >> 1;
    return LoRaSettings.ErrorCoding;
}

void SX1276LoRaSetPacketCrcOn( bool enable )
{
    SX1276Read( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2 );
    SX1276LR->RegModemConfig2 = ( SX1276LR->RegModemConfig2 & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) | ( enable << 2 );
    SX1276Write( REG_LR_MODEMCONFIG2, SX1276LR->RegModemConfig2 );
    LoRaSettings.CrcOn = enable;
}

void SX1276LoRaSetPreambleLength( uint16_t value )
{
    SX1276ReadBuffer( REG_LR_PREAMBLEMSB, &SX1276LR->RegPreambleMsb, 2 );

    SX1276LR->RegPreambleMsb = ( value >> 8 ) & 0x00FF;
    SX1276LR->RegPreambleLsb = value & 0xFF;
    SX1276WriteBuffer( REG_LR_PREAMBLEMSB, &SX1276LR->RegPreambleMsb, 2 );
}

uint16_t SX1276LoRaGetPreambleLength( void )
{
    SX1276ReadBuffer( REG_LR_PREAMBLEMSB, &SX1276LR->RegPreambleMsb, 2 );
    return ( ( SX1276LR->RegPreambleMsb & 0x00FF ) << 8 ) | SX1276LR->RegPreambleLsb;
}

bool SX1276LoRaGetPacketCrcOn( void )
{
    SX1276Read( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2 );
    LoRaSettings.CrcOn = ( SX1276LR->RegModemConfig2 & RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON ) >> 1;
    return LoRaSettings.CrcOn;
}

void SX1276LoRaSetImplicitHeaderOn( bool enable )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    SX1276LR->RegModemConfig1 = ( SX1276LR->RegModemConfig1 & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) | ( enable );
    SX1276Write( REG_LR_MODEMCONFIG1, SX1276LR->RegModemConfig1 );
    LoRaSettings.ImplicitHeaderOn = enable;
}

bool SX1276LoRaGetImplicitHeaderOn( void )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    LoRaSettings.ImplicitHeaderOn = ( SX1276LR->RegModemConfig1 & RFLR_MODEMCONFIG1_IMPLICITHEADER_ON );
    return LoRaSettings.ImplicitHeaderOn;
}

void SX1276LoRaSetRxSingleOn( bool enable )
{
    LoRaSettings.RxSingleOn = enable;
}

bool SX1276LoRaGetRxSingleOn( void )
{
    return LoRaSettings.RxSingleOn;
}

void SX1276LoRaSetFreqHopOn( bool enable )
{
    LoRaSettings.FreqHopOn = enable;
}

bool SX1276LoRaGetFreqHopOn( void )
{
    return LoRaSettings.FreqHopOn;
}

void SX1276LoRaSetHopPeriod( uint8_t value )
{
    SX1276LR->RegHopPeriod = value;
    SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );
    LoRaSettings.HopPeriod = value;
}

uint8_t SX1276LoRaGetHopPeriod( void )
{
    SX1276Read( REG_LR_HOPPERIOD, &SX1276LR->RegHopPeriod );
    LoRaSettings.HopPeriod = SX1276LR->RegHopPeriod;
    return LoRaSettings.HopPeriod;
}

void SX1276LoRaSetTxPacketTimeout( uint32_t value )
{
    LoRaSettings.TxPacketTimeout = value;
}

uint32_t SX1276LoRaGetTxPacketTimeout( void )
{
    return LoRaSettings.TxPacketTimeout;
}

void SX1276LoRaSetRxPacketTimeout( uint32_t value )
{
    LoRaSettings.RxPacketTimeout = value;
}

uint32_t SX1276LoRaGetRxPacketTimeout( void )
{
    return LoRaSettings.RxPacketTimeout;
}

void SX1276LoRaSetPayloadLength( uint8_t value )
{
    SX1276LR->RegPayloadLength = value;
    SX1276Write( REG_LR_PAYLOADLENGTH, SX1276LR->RegPayloadLength );
    LoRaSettings.PayloadLength = value;
}

uint8_t SX1276LoRaGetPayloadLength( void )
{
    SX1276Read( REG_LR_PAYLOADLENGTH, &SX1276LR->RegPayloadLength );
    LoRaSettings.PayloadLength = SX1276LR->RegPayloadLength;
    return LoRaSettings.PayloadLength;
}

void SX1276LoRaSetPa20dBm( bool enale )
{
    SX1276Read( REG_LR_PADAC, &SX1276LR->RegPaDac );
    SX1276Read( REG_LR_PACONFIG, &SX1276LR->RegPaConfig );

    if( ( SX1276LR->RegPaConfig & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
    {    
        if( enale == true )
        {
            SX1276LR->RegPaDac = 0x87;
        }
    }
    else
    {
        SX1276LR->RegPaDac = 0x84;
    }
    SX1276Write( REG_LR_PADAC, SX1276LR->RegPaDac );
}

bool SX1276LoRaGetPa20dBm( void )
{
    SX1276Read( REG_LR_PADAC, &SX1276LR->RegPaDac );
    
    return ( ( SX1276LR->RegPaDac & 0x07 ) == 0x07 ) ? true : false;
}

void SX1276LoRaSetPAOutput( uint8_t outputPin )
{
    SX1276Read( REG_LR_PACONFIG, &SX1276LR->RegPaConfig );
    SX1276LR->RegPaConfig = (SX1276LR->RegPaConfig & RFLR_PACONFIG_PASELECT_MASK ) | outputPin;
    SX1276Write( REG_LR_PACONFIG, SX1276LR->RegPaConfig );
}

uint8_t SX1276LoRaGetPAOutput( void )
{
    SX1276Read( REG_LR_PACONFIG, &SX1276LR->RegPaConfig );
    return SX1276LR->RegPaConfig & ~RFLR_PACONFIG_PASELECT_MASK;
}

void SX1276LoRaSetPaRamp( uint8_t value )
{
    SX1276Read( REG_LR_PARAMP, &SX1276LR->RegPaRamp );
    SX1276LR->RegPaRamp = ( SX1276LR->RegPaRamp & RFLR_PARAMP_MASK ) | ( value & ~RFLR_PARAMP_MASK );
    SX1276Write( REG_LR_PARAMP, SX1276LR->RegPaRamp );
}

uint8_t SX1276LoRaGetPaRamp( void )
{
    SX1276Read( REG_LR_PARAMP, &SX1276LR->RegPaRamp );
    return SX1276LR->RegPaRamp & ~RFLR_PARAMP_MASK;
}

void SX1276LoRaSetSymbTimeout( uint16_t value )
{
    SX1276ReadBuffer( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2, 2 );

    SX1276LR->RegModemConfig2 = ( SX1276LR->RegModemConfig2 & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) | ( ( value >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK );
    SX1276LR->RegSymbTimeoutLsb = value & 0xFF;
    SX1276WriteBuffer( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2, 2 );
}

uint16_t SX1276LoRaGetSymbTimeout( void )
{
    SX1276ReadBuffer( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2, 2 );
    return ( ( SX1276LR->RegModemConfig2 & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) << 8 ) | SX1276LR->RegSymbTimeoutLsb;
}

void SX1276LoRaSetLowDatarateOptimize( bool enable )
{
    SX1276Read( REG_LR_MODEMCONFIG3, &SX1276LR->RegModemConfig3 );
    SX1276LR->RegModemConfig3 = ( SX1276LR->RegModemConfig3 & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK ) | ( enable << 3 );
    SX1276Write( REG_LR_MODEMCONFIG3, SX1276LR->RegModemConfig3 );
}

bool SX1276LoRaGetLowDatarateOptimize( void )
{
    SX1276Read( REG_LR_MODEMCONFIG3, &SX1276LR->RegModemConfig3 );
    return ( ( SX1276LR->RegModemConfig3 & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON ) >> 3 );
}

void SX1276LoRaSetNbTrigPeaks( uint8_t value )
{
    SX1276Read( 0x31, &SX1276LR->RegDetectOptimize );
    SX1276LR->RegDetectOptimize = ( SX1276LR->RegDetectOptimize & 0xF8 ) | value;
    SX1276Write( 0x31, SX1276LR->RegDetectOptimize );
}

uint8_t SX1276LoRaGetNbTrigPeaks( void )
{
    SX1276Read( 0x31, &SX1276LR->RegDetectOptimize );
    return ( SX1276LR->RegDetectOptimize & 0x07 );
}

#endif // USE_SX1276_RADIO
