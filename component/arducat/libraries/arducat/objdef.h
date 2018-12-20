/*-----------------------------------------------------------------------------------------
------
------    Description
------
------    objdef.h
------
------    CANopen over EtherCAT object dictionary
------                                                                                                                                                                 ------
-----------------------------------------------------------------------------------------*/

#ifndef _OBJDEF_H_
#define _OBJDEF_H_

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

#include "sdoserv.h"

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Types
*/

#define     DEFTYPE_NULL                0x0000
#define     DEFTYPE_BOOLEAN             0x0001
#define     DEFTYPE_INTEGER8            0x0002
#define     DEFTYPE_INTEGER16           0x0003
#define     DEFTYPE_INTEGER32           0x0004
#define     DEFTYPE_UNSIGNED8           0x0005
#define     DEFTYPE_UNSIGNED16          0x0006
#define     DEFTYPE_UNSIGNED32          0x0007
#define     DEFTYPE_REAL32              0x0008
#define     DEFTYPE_VISIBLESTRING       0x0009
#define     DEFTYPE_OCTETSTRING         0x000A
#define     DEFTYPE_UNICODE_STRING      0x000B
#define     DEFTYPE_TIME_OF_DAY         0x000C
#define     DEFTYPE_TIME_DIFFERENCE     0x000D
#define     DEFTYPE_DOMAIN              0x000F
#define     DEFTYPE_INTEGER24           0x0010
#define     DEFTYPE_REAL64              0x0011
#define     DEFTYPE_INTEGER40           0x0012
#define     DEFTYPE_INTEGER48           0x0013
#define     DEFTYPE_INTEGER56           0x0014
#define     DEFTYPE_INTEGER64           0x0015
#define     DEFTYPE_UNSIGNED24          0x0016
#define     DEFTYPE_UNSIGNED40          0x0018
#define     DEFTYPE_UNSIGNED48          0x0019
#define     DEFTYPE_UNSIGNED56          0x001A
#define     DEFTYPE_UNSIGNED64          0x001B
#define     DEFTYPE_SAFETY              0x001C
#define     DEFTYPE_PDOMAPPING          0x0021
#define     DEFTYPE_IDENTITY            0x0023
#define     DEFTYPE_COMMAND             0x0025
#define     DEFTYPE_PDOCOMPAR           0x0027
#define     DEFTYPE_ENUM                0x0028
#define     DEFTYPE_SMPAR               0x0029
#define     DEFTYPE_RECORD              0x002A
#define     DEFTYPE_BACKUP              0x002B
#define     DEFTYPE_MDP                 0x002C
#define     DEFTYPE_FSOEFRAME           0x002E
#define     DEFTYPE_FSOECOMMPAR         0x002F
#define     DEFTYPE_BIT1                0x0030
#define     DEFTYPE_BIT2                0x0031
#define     DEFTYPE_BIT3                0x0032
#define     DEFTYPE_BIT4                0x0033
#define     DEFTYPE_BIT5                0x0034
#define     DEFTYPE_BIT6                0x0035
#define     DEFTYPE_BIT7                0x0036
#define     DEFTYPE_BIT8                0x0037
#define     DEFTYPE_ERRORHANDLING       0x0038
#define     DEFTYPE_DIAGHISTORY         0x0039
#define     DEFTYPE_SYNCSTATUS          0x003A
#define     DEFTYPE_SYNCSETTINGS        0x003B
#define     DEFTYPE_CYCLICTIMES         0x003C

#define     BYTELEN_UNKNOWN             0x0000
#define     BYTELEN_BOOLEAN             0x0001
#define     BYTELEN_INTEGER8            0x0001
#define     BYTELEN_INTEGER16           0x0002
#define     BYTELEN_INTEGER32           0x0004
#define     BYTELEN_UNSIGNED8           0x0001
#define     BYTELEN_UNSIGNED16          0x0002
#define     BYTELEN_UNSIGNED32          0x0004
#define     BYTELEN_REAL32              0x0004
#define     BYTELEN_VISIBLESTRING       0x0001
#define     BYTELEN_OCTETSTRING         0x0001
#define     BYTELEN_REAL64              0x0008
#define     BYTELEN_UNSIGNED24          0x0003
#define     BYTELEN_UNSIGNED40          0x0005
#define     BYTELEN_UNSIGNED48          0x0006
#define     BYTELEN_UNSIGNED56          0x0007
#define     BYTELEN_UNSIGNED64          0x0008

#define     BITLEN_BOOLEAN              0x0001
#define     BITLEN_INTEGER8             0x0008
#define     BITLEN_INTEGER16            0x0010
#define     BITLEN_INTEGER32            0x0020
#define     BITLEN_UNSIGNED8            0x0008
#define     BITLEN_UNSIGNED16           0x0010
#define     BITLEN_UNSIGNED32           0x0020
#define     BITLEN_REAL32               0x0020
#define     BITLEN_REAL64               0x0040
#define     BITLEN_UNSIGNED24           0x0018
#define     BITLEN_UNSIGNED40           0x0028
#define     BITLEN_UNSIGNED48           0x0030
#define     BITLEN_UNSIGNED56           0x0038
#define     BITLEN_UNSIGNED64           0x0040

#define     SYNCTYPE_FREERUN            0x0000
#define     SYNCTYPE_SYNCHRON           0x0001
#define     SYNCTYPE_DCSYNC0            0x0002
#define     SYNCTYPE_DCSYNC1            0x0003
#define     SYNCTYPE_SM2INT             0x0022
#define     SYNCTYPE_SM3INT             0x0023
#define     SYNCTYPE_FREERUNSUPP        0x0001
#define     SYNCTYPE_SYNCHRONSUPP       0x0002
#define     SYNCTYPE_DCSYNC0SUPP        0x0004
#define     SYNCTYPE_DCSYNC1SUPP        0x0008
#define     SYNCTYPE_TIMESVARIABLE      0x4000
#define     SYNCTYPE_FASTMODE           0x8000


#define     IS_PDO_ASSIGN(x)            ((x >= 0x1C10) && (x <= 0x1C2F))
#define     IS_RX_PDO(x)                (((x) >= 0x1600) && ((x) <= 0x17FF))
#define     IS_TX_PDO(x)                (((x) >= 0x1A00) && ((x) <= 0x1BFF))
/*/////////////////////////////////////////////////////////////////////////////////////////
//
//    Object dictionary entry structure
*/

typedef struct OBJ_ENTRY
{
    struct OBJ_ENTRY                      *pPrev;                    //previous entry(object) in the object dictionary list
    struct OBJ_ENTRY                      *pNext;                    //next entry(object) in the object dictionary list

    UINT16                                Index;
    TSDOINFOOBJDESC                       ObjDesc;
    OBJCONST TSDOINFOENTRYDESC OBJMEM     *pEntryDesc;
    OBJCONST UCHAR OBJMEM                 *pName;
    void MBXMEM                           *pVarPtr;
    UINT8 (* Read)( UINT16 Index, UINT8 Subindex, UINT32 Size, UINT16 MBXMEM * pData, UINT8 bCompleteAccess );
    UINT8 (* Write)( UINT16 Index, UINT8 Subindex, UINT32 Size, UINT16 MBXMEM * pData, UINT8 bCompleteAccess );
    UINT16                                 NonVolatileOffset;
}
TOBJECT;

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// type definitions of objects
//
*/

typedef struct OBJ_STRUCT_PACKED_START
{
    UINT16    subindex0;
    UINT16    u16SyncType;              //SI 1
    UINT32    u32CycleTime;             //SI 2
    UINT32    u32ShiftTime;             //SI 3
    UINT16    u16SyncTypesSupported;    //SI 4
    UINT32    u32MinCycleTime;          //SI 5
    UINT32    u32CalcAndCopyTime;       //SI 6
    UINT32    u32Reserved;              //SI 7
    UINT16    u16GetCycleTime;          //SI 8
    UINT32    u32DelayTime;             //SI 9
    UINT32    u32Sync0CycleTime;        //SI 10
    UINT32    u32SmEventMissedCounter;  //SI 11
    UINT32    u32CycleExceededCounter;  //SI 12
    UINT32    u32ShiftTooShortCounter;  // SI 13
    UINT16    u16SyncError;             // SI 32
}OBJ_STRUCT_PACKED_END
TSYNCMANPAR;

typedef struct OBJ_STRUCT_PACKED_START
{
    UINT16    syncFailedCounter;
}OBJ_STRUCT_PACKED_END
TCYCLEDIAG;

typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT32   u32LocalErrorReaction;
   UINT32   u32SyncErrorCounterLimit;
} OBJ_STRUCT_PACKED_END
TOBJ10F1;

#endif //_OBJDEF_H_


