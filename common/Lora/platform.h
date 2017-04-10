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
 * \file       platform.h
 * \brief        
 *
 * \version    1.0
 * \date       Nov 21 2012
 * \author     Miguel Luis
 */
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifndef __GNUC__
#define inline
#endif

/*!
 * Platform definition
 */
#define Bleeper                                     3
#define SX1243ska                                   2
#define SX12xxEiger                                 1
#define SX12000DVK                                  0

/*!
 * Platform choice. Please uncoment the PLATFORM define and choose your platform
 * or add/change the PLATFORM definition on the compiler Defines option
 */
#define PLATFORM                                    SX12xxEiger

#if( PLATFORM == SX12xxEiger )
/*!
 * Radio choice. Please uncomment the wanted radio and comment the others
 * or add/change wanted radio definition on the compiler Defines option
 */
//#define USE_SX1232_RADIO
//#define USE_SX1272_RADIO
#define USE_SX1276_RADIO
//#define USE_SX1243_RADIO

/*!
 * Module choice. There are three existing module with the SX1276.
 * Please set the connected module to the value 1 and set the others to 0
 */
#ifdef USE_SX1276_RADIO
#define MODULE_SX1276RF1IAS                         0
#define MODULE_SX1276RF1JAS                         0
#define MODULE_SX1276RF1KAS                         1
#endif

    #define USE_UART                                1

#elif( PLATFORM == SX12000DVK )
/*!
 * Radio choice. Please uncomment the wanted radio and comment the others
 * or add/change wanted radio definition on the compiler Defines option
 */
//#define USE_SX1232_RADIO
//#define USE_SX1272_RADIO
#define USE_SX1276_RADIO
//#define USE_SX1243_RADIO


#elif( PLATFORM == SX1243ska )

#elif( PLATFORM == Bleeper )
    #define USE_SX1272_RADIO
    
    #include "bleeper/bleeper.h"
    #define USE_UART                                0

#else
    #error "Missing define: Platform (ie. SX12xxEiger)"
#endif

#endif // __PLATFORM_H__
