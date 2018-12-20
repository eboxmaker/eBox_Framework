/*-----------------------------------------------------------------------------------------
------
------    ecatcoe.h
------                                                                                                                                                                 ------
-----------------------------------------------------------------------------------------*/

#ifndef _ECATCOE_H_

#define _ECATCOE_H_

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

#include "mailbox.h"

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// Error Codes
*/

#define     ERROR_COEINVALIDSERVICE     0x01
#define     ERROR_COENOTSUPPORTED       0x02

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// COE services
*/

#define     COESERVICE_EMERGENCY        0x01
#define     COESERVICE_SDOREQUEST       0x02
#define     COESERVICE_SDORESPONSE      0x03
#define     COESERVICE_TXPDO            0x04
#define     COESERVICE_RXPDO            0x05
#define     COESERVICE_TXPDOREMREQ      0x06
#define     COESERVICE_RXPDOREMREQ      0x07
#define     COESERVICE_SDOINFO          0x08

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// COE Structures
*/

typedef UINT16 TCOEHEADER;
#define     COEHEADER_COESERVICESHIFT   12
#define     COEHEADER_COESERVICEMASK    0xF000

#define     COE_HEADER_SIZE             2

typedef struct MBX_STRUCT_PACKED_START
{
  TMBXHEADER        MbxHeader;
  TCOEHEADER        CoeHeader;
  UINT16            Data[((MAX_MBX_DATA_SIZE)-(COE_HEADER_SIZE)) >> 1];
}MBX_STRUCT_PACKED_END
TCOEMBX;



#endif //_ECATCOE_H_


