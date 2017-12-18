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
 * \file       radio.c
 * \brief      Generic radio driver ( radio abstraction )
 *
 * \version    2.0.0 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Gregory Cristian on Apr 25 2013
 */
#include "platform.h"

#include "radio.h"

#if defined( USE_SX1232_RADIO )
    #include "sx1232.h"
#elif defined( USE_SX1272_RADIO )
    #include "sx1272.h"
#elif defined( USE_SX1276_RADIO )
    #include "sx1276.h"
#else
    #error "Missing define: USE_XXXXXX_RADIO (ie. USE_SX1272_RADIO)"
#endif    

tRadioDriver RadioDriver;

tRadioDriver* RadioDriverInit( void )
{
#if defined( USE_SX1232_RADIO )
    RadioDriver.Init = SX1232Init;
    RadioDriver.Reset = SX1232Reset;
    RadioDriver.StartRx = SX1232StartRx;
    RadioDriver.GetRxPacket = SX1232GetRxPacket;
    RadioDriver.SetTxPacket = SX1232SetTxPacket;
    RadioDriver.Process = SX1232Process;
#elif defined( USE_SX1272_RADIO )
    RadioDriver.Init = SX1272Init;
    RadioDriver.Reset = SX1272Reset;
    RadioDriver.StartRx = SX1272StartRx;
    RadioDriver.GetRxPacket = SX1272GetRxPacket;
    RadioDriver.SetTxPacket = SX1272SetTxPacket;
    RadioDriver.Process = SX1272Process;
#elif defined( USE_SX1276_RADIO )
    RadioDriver.Init = SX1276Init;
    RadioDriver.Reset = SX1276Reset;
    RadioDriver.StartRx = SX1276StartRx;
    RadioDriver.GetRxPacket = SX1276GetRxPacket;
    RadioDriver.SetTxPacket = SX1276SetTxPacket;
    RadioDriver.Process = SX1276Process;
#else
    #error "Missing define: USE_XXXXXX_RADIO (ie. USE_SX1272_RADIO)"
#endif    

    return &RadioDriver;
}
