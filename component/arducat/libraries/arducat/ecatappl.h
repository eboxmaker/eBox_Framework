/**
\ingroup ecatappl
\file ecatappl.h
\brief Definition.
*/
/*-----------------------------------------------------------------------------------------
------
------    Description
------
------    ecatappl.h
------
------    EtherCAT Slave Application
------                                                                                                                                                                 ------
-----------------------------------------------------------------------------------------*/

#ifndef _ECATAPPL_H_
#define _ECATAPPL_H_

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

#include "ecat_def.h"

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/
/*Set to unsigned short to handle bit entries correct*/
#define     BOOLEAN(x)                  unsigned short(x):1
#define     BIT1(x)                     unsigned short(x):1
#define     BIT2(x)                     unsigned short(x):2
#define     BIT3(x)                     unsigned short(x):3
#define     BIT4(x)                     unsigned short(x):4
#define     BIT5(x)                     unsigned short(x):5
#define     BIT6(x)                     unsigned short(x):6
#define     BIT7(x)                     unsigned short(x):7
#define     BIT8(x)                     unsigned short(x):8
#define     ALIGN0(x)
#define     ALIGN1(x)                   unsigned short(x):1;
#define     ALIGN2(x)                   unsigned short(x):2;
#define     ALIGN3(x)                   unsigned short(x):3;
#define     ALIGN4(x)                   unsigned short(x):4;
#define     ALIGN5(x)                   unsigned short(x):5;
#define     ALIGN6(x)                   unsigned short(x):6;
#define     ALIGN7(x)                   unsigned short(x):7;
#define     ALIGN8(x)                   unsigned short(x):8;
#define     ALIGN9(x)                   unsigned short x1:1; unsigned short(x):8;
#define     ALIGN10(x)                  unsigned short x1:2; unsigned short(x):8;
#define     ALIGN11(x)                  unsigned short x1:3; unsigned short(x):8;
#define     ALIGN12(x)                  unsigned short x1:4; unsigned short(x):8;
#define     ALIGN13(x)                  unsigned short x1:5; unsigned short(x):8;
#define     ALIGN14(x)                  unsigned short x1:6; unsigned short(x):8;
#define     ALIGN15(x)                  unsigned short x1:7; unsigned short(x):8;


/*-----------------------------------------------------------------------------------------
------
------    type definitions
------
-----------------------------------------------------------------------------------------*/

#endif //_ECATAPPL_H_

