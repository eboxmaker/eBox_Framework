/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/**
The application code (editable area) is at the end of this file.
This area is the Ethercat slave stack code. DO NOT EDIT!!!
**/
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#include <ecatappl.h>
#include <ecatcoe.h>
#include <ecatslv.h>
#include <ecat_def.h>
#include <esc.h>
#include <ethercat.h>
#include <mailbox.h>
#include <objdef.h>
#include <sdoserv.h>
#include <SPI.h>
#define DEBUG_OVER_SERIAL
const UINT32 VENDOR_ID = 0x04210909;
UINT32 PRODUCT_CODE = 0x00000001;
UINT32 REVISION_NUMBER = 0x00010000;
UINT32 SERIAL_NUMBER = 0x00010000;
UINT32 DEVICE_PROFILE_TYPE = 5001.0;
char DEVICE_NAME[] = "ComboDevice";
UINT8 DEVICE_NAME_LEN = 11;
char DEVICE_HW_VERSION[] = "1.00";
UINT8 DEVICE_HW_VERSION_LEN = 4;
char DEVICE_SW_VERSION[] = "1.00";
UINT8 DEVICE_SW_VERSION_LEN = 4;
//extern TSYNCMANPAR MBXMEM sSyncManOutPar;
//extern TSYNCMANPAR MBXMEM sSyncManInPar;

/*-----------------------------------------------------------------------------------------
------
------    Generic EtherCAT device object
------
-----------------------------------------------------------------------------------------*/

/******************************************************************************
** Object 0x1000
******************************************************************************/
UINT32 u32Devicetype = (DEVICE_PROFILE_TYPE);
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x1000 = {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x1000[] = "Device type";

/******************************************************************************
** Object 0x1001
******************************************************************************/
UINT16 u16ErrorRegister = 0x0;
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x1001 = {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x1001[] = "Error register";

/******************************************************************************
** Object 0x1008
******************************************************************************/
CHAR *acDevicename = DEVICE_NAME;
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x1008 = {DEFTYPE_VISIBLESTRING, BYTE2BIT(DEVICE_NAME_LEN), ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x1008[] = "Device name";

/******************************************************************************
** Object 0x1009
******************************************************************************/
CHAR *acHardwareversion = DEVICE_HW_VERSION;
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x1009 = {DEFTYPE_VISIBLESTRING,BYTE2BIT(DEVICE_HW_VERSION_LEN), ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x1009[] = "Hardware version";

/******************************************************************************
** Object 0x100A
******************************************************************************/
CHAR *acSoftwareversion = DEVICE_SW_VERSION;
OBJCONST TSDOINFOENTRYDESC    OBJMEM sEntryDesc0x100A = {DEFTYPE_VISIBLESTRING,BYTE2BIT(DEVICE_SW_VERSION_LEN), ACCESS_READ};
OBJCONST UCHAR OBJMEM aName0x100A[] = "Software version";


/******************************************************************************
** Object 0x1018
******************************************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT32   u32VendorID;
   UINT32   u32Productcode;
   UINT32   u32Revision;
   UINT32   u32Serialnumber;
} OBJ_STRUCT_PACKED_END
TOBJ1018;

TOBJ1018 sIdentity = {4, (VENDOR_ID), (PRODUCT_CODE), (REVISION_NUMBER), (SERIAL_NUMBER)};
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1018[5] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: Vendor ID */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 002: Product code */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 003: Revision */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}}; /* SubIndex 004: Serial number */
OBJCONST UCHAR OBJMEM aName0x1018[] = "Identity\000Vendor ID\000Product code\000Revision\000Serial number\000\377";


/******************************************************************************
** Object 0x10F1    Error Settings
******************************************************************************/
TOBJ10F1 sErrorSettings
= {2, 0x00, MAX_SM_EVENT_MISSED}
;
//object declaration and initialization in objdef.h
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x10F1[3] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 001: Local Error Reaction */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}}; /* SubIndex 002: Sync Error Counter Limit*/
OBJCONST UCHAR OBJMEM aName0x10F1[] = "Error Settings\000Local Error Reaction\000Sync Error Counter Limit\000\377";

/******************************************************************************
** Object 0x1C00
******************************************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
   UINT16   u16SubIndex0;
   UINT8   aEntries[4];
} OBJ_STRUCT_PACKED_END
TOBJ1C00;

TOBJ1C00 sSyncmanagertype = {0x04, {0x01, 0x02, 0x03, 0x04}};
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C00[2] = {
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ},
   {DEFTYPE_UNSIGNED8, 0x08, ACCESS_READ}};
OBJCONST UCHAR OBJMEM aName0x1C00[] = "Sync manager type";



/******************************************************************************
** Object 0x1C32
******************************************************************************/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C32[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}, /* SubIndex 001: Sync mode */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 002: Cycle time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 003: Shift time */
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}, /* SubIndex 004: Sync modes supported */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 005: Minimum cycle time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 006: Calc and copy time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE}, /* Subindex 007 reserved for future use*/
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}, /* SubIndex 008: Get Cycle Time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 009: Delay time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE}, /* SubIndex 010: Sync0 time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 011: SM event missed counter */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 012: Cycle exceeded counter */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 013: Shift too short counter */
   {0x0000, 0, 0}, /* Subindex 014 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 015 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 016 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 017 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 018 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 019 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 020 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 021 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 022 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 023 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 024 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 025 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 026 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 027 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 028 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 029 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 030 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 031 doesn't exist */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ}}; /* SubIndex 032: Sync error */
/*ECATCHANGE_START(V5.01) COE1*/
OBJCONST UCHAR OBJMEM aName0x1C32[] = "SM output parameter\000Sync mode\000Cycle time\000Shift time\000Sync modes supported\000Minimum cycle time\000Calc and copy time\000Reserved\000Get Cycle Time\000Delay time\000Sync0 time\000SM event missed counter\000Cycle exceeded counter\000Shift too short counter\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000Sync error\000\377";
/*ECATCHANGE_START(V5.01) COE1*/
/******************************************************************************
** Object 0x1C33
******************************************************************************/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asEntryDesc0x1C33[] = {
   {DEFTYPE_UNSIGNED8, 0x8, ACCESS_READ }, /* Subindex 000 */
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}, /* SubIndex 001: Sync mode */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 002: Cycle time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 003: Shift time */
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ}, /* SubIndex 004: Sync modes supported */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 005: Minimum cycle time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 006: Calc and copy time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE}, /* Subindex 007 reserved for future use*/
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READWRITE}, /* SubIndex 008: Get Cycle Time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 009: Delay time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READWRITE}, /* SubIndex 010: Sync0 time */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 011: SM event missed counter */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 012: Cycle exceeded counter */
   {DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ}, /* SubIndex 013: Shift too short counter */
   {0x0000, 0, 0}, /* Subindex 014 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 015 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 016 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 017 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 018 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 019 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 020 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 021 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 022 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 023 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 024 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 025 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 026 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 027 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 028 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 029 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 030 doesn't exist */
   {0x0000, 0, 0}, /* Subindex 031 doesn't exist */
   {DEFTYPE_BOOLEAN, 0x01, ACCESS_READ}}; /* SubIndex 032: Sync error */
/*ECATCHANGE_START(V5.01) COE1*/
OBJCONST UCHAR OBJMEM aName0x1C33[] = "SM input parameter\000Sync mode\000Cycle time\000Shift time\000Sync modes supported\000Minimum cycle time\000Calc and copy time\000Reserved\000Get Cycle Time\000Delay time\000Sync0 time\000Cycle exceeded counter\000SM event missed counter\000Shift too short counter\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000Sync error\000\377";
/*ECATCHANGE_START(V5.01) COE1*/
/******************************************************************************
** Object Dictionary
******************************************************************************/
//TOBJECT    OBJMEM * ObjDicList = NULL;

TOBJECT    OBJMEM GenObjDic[] = {
    /* Object 0x1000 */
   {NULL,NULL,  0x1000, {DEFTYPE_UNSIGNED32, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x1000, aName0x1000, &u32Devicetype, NULL, NULL, 0x0000 },
   /* Object 0x1001 */
   {NULL,NULL,  0x1001, {DEFTYPE_UNSIGNED8, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x1001, aName0x1001, &u16ErrorRegister, NULL, NULL, 0x0000 },
/* Object 0x1008 */
   {NULL,NULL,  0x1008, {DEFTYPE_VISIBLESTRING, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x1008, aName0x1008, acDevicename, NULL, NULL, 0x0000 },
   /* Object 0x1009 */
   {NULL,NULL,  0x1009, {DEFTYPE_VISIBLESTRING, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x1009, aName0x1009, acHardwareversion, NULL, NULL, 0x0000 },
   /* Object 0x100A */
   {NULL,NULL,  0x100A, {DEFTYPE_VISIBLESTRING, 0 | (OBJCODE_VAR << 8)}, &sEntryDesc0x100A, aName0x100A, acSoftwareversion, NULL, NULL, 0x0000 },
   /* Object 0x1018 */
   {NULL,NULL,  0x1018, {DEFTYPE_IDENTITY, 4 | (OBJCODE_REC << 8)}, asEntryDesc0x1018, aName0x1018, &sIdentity, NULL, NULL, 0x0000 },
    /* Object 0x10F1 */
   {NULL,NULL,  0x10F1, {DEFTYPE_RECORD, 2 | (OBJCODE_REC << 8)}, asEntryDesc0x10F1, aName0x10F1, &sErrorSettings, NULL, NULL, 0x0000 },
   /* Object 0x1C00 */
   {NULL,NULL, 0x1C00, {DEFTYPE_UNSIGNED8, 4 | (OBJCODE_ARR << 8)}, asEntryDesc0x1C00, aName0x1C00, &sSyncmanagertype, NULL, NULL, 0x0000 },
   /* Object 0x1C32 */
   {NULL,NULL, 0x1C32, {DEFTYPE_SMPAR, 32 | (OBJCODE_REC << 8)}, asEntryDesc0x1C32, aName0x1C32, &ethercat.sSyncManOutPar, NULL, NULL, 0x0000 },
   /* Object 0x1C33 */
   {NULL,NULL, 0x1C33, {DEFTYPE_SMPAR, 32 | (OBJCODE_REC << 8)}, asEntryDesc0x1C33, aName0x1C33, &ethercat.sSyncManInPar, NULL, NULL, 0x0000 },
  /*end of entries*/
   {NULL,NULL,    0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};




/*PDO assign entry description*/
OBJCONST TSDOINFOENTRYDESC    OBJMEM asPDOAssignEntryDesc[] = {
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ|ACCESS_WRITE},
   {DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ|ACCESS_WRITE}};



/****************************************************
** Object0x1600
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16   u16SubIndex0;
	UINT32   aEntries[13];
} OBJ_STRUCT_PACKED_END
TOBJ0x1600;
TOBJ0x1600 Obj0x1600 __attribute__ ((aligned (2)))
={13, {0x70000101,0x70000201,0x70000301,0x70000401,0x70000501,0x70000601,0x70000701,0x70000801,0x70010108,0x70010208,0x70010308,0x70010408,0x00000008,}};
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1600[] = {
	{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ },
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
};
OBJCONST UCHAR OBJMEM aName0x1600[] = "Output Mapping 00\000\377";
/****************************************************
** Object0x1C12
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16   u16SubIndex0;
	UINT16   aEntries[1];
} OBJ_STRUCT_PACKED_END
TOBJ0x1C12;
TOBJ0x1C12 sRxPDOassign __attribute__ ((aligned (2))) 
={1, {0x1600}};
OBJCONST UCHAR OBJMEM aName0x1C12[] = "RXPDO assign";
/****************************************************
** Object0x1A00
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16   u16SubIndex0;
	UINT32   aEntries[5];
} OBJ_STRUCT_PACKED_END
TOBJ0x1A00;
TOBJ0x1A00 Obj0x1A00 __attribute__ ((aligned (2)))
={5, {0x60000101,0x60000201,0x60000301,0x60000401,0x0000000c,}};
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1A00[] = {
	{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ },
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
};
OBJCONST UCHAR OBJMEM aName0x1A00[] = "Input Mapping 00\000\377";
/****************************************************
** Object0x1A01
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16   u16SubIndex0;
	UINT32   aEntries[5];
} OBJ_STRUCT_PACKED_END
TOBJ0x1A01;
TOBJ0x1A01 Obj0x1A01 __attribute__ ((aligned (2)))
={5, {0x60100108,0x00000008,0x60100220,0x60100320,0x60100420,}};
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x1A01[] = {
	{DEFTYPE_UNSIGNED16, 0x10, ACCESS_READ },
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
	{DEFTYPE_UNSIGNED32, 0x20, ACCESS_READ},
};
OBJCONST UCHAR OBJMEM aName0x1A01[] = "Input Mapping 01\000\377";
/****************************************************
** Object0x1C13
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16   u16SubIndex0;
	UINT16   aEntries[2];
} OBJ_STRUCT_PACKED_END
TOBJ0x1C13;
TOBJ0x1C13 sTxPDOassign __attribute__ ((aligned (2))) 
={2, {0x1A00,0x1A01}};
OBJCONST UCHAR OBJMEM aName0x1C13[] = "TXPDO assign";
/****************************************************
** Object0x6000
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	INT16 SubIndex0 __attribute__ ((aligned (2)));
	UINT16 SubIndex1:1 __attribute__ ((aligned (2)));
	UINT16 SubIndex2:1;
	UINT16 SubIndex3:1;
	UINT16 SubIndex4:1;
} OBJ_STRUCT_PACKED_END
TOBJ0x6000;
TOBJ0x6000 Obj0x6000
={4,0,0,0,0,};
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x6000[5] = {
	{DEFTYPE_INTEGER16, 16, ACCESS_READ|ACCESS_WRITE|OBJACCESS_NOPDOMAPPING},
	{DEFTYPE_BOOLEAN, 1, ACCESS_READ|ACCESS_WRITE|OBJACCESS_TXPDOMAPPING},
	{DEFTYPE_BOOLEAN, 1, ACCESS_READ|ACCESS_WRITE|OBJACCESS_TXPDOMAPPING},
	{DEFTYPE_BOOLEAN, 1, ACCESS_READ|ACCESS_WRITE|OBJACCESS_TXPDOMAPPING},
	{DEFTYPE_BOOLEAN, 1, ACCESS_READ|ACCESS_WRITE|OBJACCESS_TXPDOMAPPING},
};
OBJCONST UCHAR OBJMEM aName0x6000[] = "Digital Inputs\000Digital Input 1\000Digital Input 2\000Digital Input 3\000Digital Input 4\000\377";
/****************************************************
** Object0x6010
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	INT16 SubIndex0 __attribute__ ((aligned (2)));
	UINT16 SubIndex1:8 __attribute__ ((aligned (2)));
	INT32 SubIndex2 __attribute__ ((aligned (2)));
	INT32 SubIndex3 __attribute__ ((aligned (2)));
	INT32 SubIndex4 __attribute__ ((aligned (2)));
} OBJ_STRUCT_PACKED_END
TOBJ0x6010;
TOBJ0x6010 Obj0x6010
={4,0,0,0,0,};
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x6010[5] = {
	{DEFTYPE_INTEGER16, 16, ACCESS_READ|ACCESS_WRITE|OBJACCESS_NOPDOMAPPING},
	{DEFTYPE_UNSIGNED8, 8, ACCESS_READ|ACCESS_WRITE|OBJACCESS_TXPDOMAPPING},
	{DEFTYPE_INTEGER32, 32, ACCESS_READ|ACCESS_WRITE|OBJACCESS_TXPDOMAPPING},
	{DEFTYPE_INTEGER32, 32, ACCESS_READ|ACCESS_WRITE|OBJACCESS_TXPDOMAPPING},
	{DEFTYPE_INTEGER32, 32, ACCESS_READ|ACCESS_WRITE|OBJACCESS_TXPDOMAPPING},
};
OBJCONST UCHAR OBJMEM aName0x6010[] = "Analog Inputs\000Analog Input 1\000Analog Input 2\000Analog Input 3\000Analog Input 4\000\377";
/****************************************************
** Object0x7000
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	UINT16 SubIndex0 __attribute__ ((aligned (2)));
	UINT16 SubIndex1:1 __attribute__ ((aligned (2)));
	UINT16 SubIndex2:1;
	UINT16 SubIndex3:1;
	UINT16 SubIndex4:1;
	UINT16 SubIndex5:1;
	UINT16 SubIndex6:1;
	UINT16 SubIndex7:1;
	UINT16 SubIndex8:1;
} OBJ_STRUCT_PACKED_END
TOBJ0x7000;
TOBJ0x7000 Obj0x7000
={8,0,0,0,0,0,0,0,0,};
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x7000[2] = {
	{DEFTYPE_UNSIGNED8, 8, ACCESS_READ|OBJACCESS_NOPDOMAPPING},
	{DEFTYPE_BOOLEAN, 1, ACCESS_READ|ACCESS_WRITE|OBJACCESS_RXPDOMAPPING},
};
OBJCONST UCHAR OBJMEM aName0x7000[] = "Digital Outputs";
/****************************************************
** Object0x7001
****************************************************/
typedef struct OBJ_STRUCT_PACKED_START {
	INT16 SubIndex0 __attribute__ ((aligned (2)));
	UINT16 SubIndex1:8 __attribute__ ((aligned (2)));
	UINT16 SubIndex2:8;
	UINT16 SubIndex3:8;
	UINT16 SubIndex4:8;
} OBJ_STRUCT_PACKED_END
TOBJ0x7001;
TOBJ0x7001 Obj0x7001
={4,0,0,0,0,};
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x7001[5] = {
	{DEFTYPE_INTEGER16, 16, ACCESS_READ|ACCESS_WRITE|OBJACCESS_NOPDOMAPPING},
	{DEFTYPE_UNSIGNED8, 8, ACCESS_READ|ACCESS_WRITE|OBJACCESS_RXPDOMAPPING},
	{DEFTYPE_UNSIGNED8, 8, ACCESS_READ|ACCESS_WRITE|OBJACCESS_RXPDOMAPPING},
	{DEFTYPE_UNSIGNED8, 8, ACCESS_READ|ACCESS_WRITE|OBJACCESS_RXPDOMAPPING},
	{DEFTYPE_UNSIGNED8, 8, ACCESS_READ|ACCESS_WRITE|OBJACCESS_RXPDOMAPPING},
};
OBJCONST UCHAR OBJMEM aName0x7001[] = "Analog Outputs\000Analog Output 1\000Analog Output 2\000Analog Output 3\000Analog Output 4\000\377";
/****************************************************
** Object0x8000
****************************************************/
INT16 Obj0x8000 __attribute__ ((aligned (2)))= 0;
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x8000 = {DEFTYPE_INTEGER8, 8, ACCESS_READ|OBJACCESS_NOPDOMAPPING};
OBJCONST UCHAR OBJMEM aName0x8000[] = "Config1";
/****************************************************
** Object0x8001
****************************************************/
UINT32 Obj0x8001 __attribute__ ((aligned (2)))= 12;
OBJCONST TSDOINFOENTRYDESC OBJMEM asEntryDesc0x8001 = {DEFTYPE_UNSIGNED32, 32, ACCESS_READ|ACCESS_WRITE|OBJACCESS_NOPDOMAPPING};
OBJCONST UCHAR OBJMEM aName0x8001[] = "Config2";

/****************************************************
** ApplicationObjDic
****************************************************/
TOBJECT    OBJMEM ApplicationObjDic[] = {
	{NULL,NULL,0x1A00,{DEFTYPE_PDOMAPPING, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x1A00, aName0x1A00, &Obj0x1A00, NULL, NULL, 0x0000 },
	{NULL,NULL,0x1A01,{DEFTYPE_PDOMAPPING, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x1A01, aName0x1A01, &Obj0x1A01, NULL, NULL, 0x0000 },
	{NULL,NULL,0x1C13,{DEFTYPE_UNSIGNED16, 2 | (OBJCODE_ARR << 8)}, asPDOAssignEntryDesc, aName0x1C13, &sTxPDOassign, NULL, NULL, 0x0000 },
	{NULL,NULL,0x1600,{DEFTYPE_PDOMAPPING, 1 | (OBJCODE_REC << 8)}, asEntryDesc0x1600, aName0x1600, &Obj0x1600, NULL, NULL, 0x0000 },
	{NULL,NULL,0x1C12,{DEFTYPE_UNSIGNED16, 1 | (OBJCODE_ARR << 8)}, asPDOAssignEntryDesc, aName0x1C12, &sRxPDOassign, NULL, NULL, 0x0000 },
	{NULL, NULL, 0x6000, {DEFTYPE_RECORD, 4|(OBJCODE_REC<<8)}, asEntryDesc0x6000, aName0x6000, &Obj0x6000, NULL , NULL , 0x0000 },
	{NULL, NULL, 0x6010, {DEFTYPE_RECORD, 4|(OBJCODE_REC<<8)}, asEntryDesc0x6010, aName0x6010, &Obj0x6010, NULL , NULL , 0x0000 },
	{NULL, NULL, 0x7000, {DEFTYPE_RECORD, 8|(OBJCODE_ARR<<8)}, asEntryDesc0x7000, aName0x7000, &Obj0x7000, NULL , NULL , 0x0000 },
	{NULL, NULL, 0x7001, {DEFTYPE_RECORD, 4|(OBJCODE_REC<<8)}, asEntryDesc0x7001, aName0x7001, &Obj0x7001, NULL , NULL , 0x0000 },
	{NULL, NULL, 0x8000, {DEFTYPE_INTEGER8, 0|(OBJCODE_VAR<<8)}, &asEntryDesc0x8000, aName0x8000, &Obj0x8000, NULL , NULL , 0x0000 },
	{NULL, NULL, 0x8001, {DEFTYPE_UNSIGNED32, 0|(OBJCODE_VAR<<8)}, &asEntryDesc0x8001, aName0x8001, &Obj0x8001, NULL , NULL , 0x0000 },
	{NULL,NULL, 0xFFFF, {0, 0}, NULL, NULL, NULL, NULL}};
	
	
	
	
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void    Ethercat::APPL_AckErrorInd(UINT16 stateTrans)
{ 
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
           all general settings were checked to start the mailbox handler. This function
           informs the application about the state transition, the application can refuse
           the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StartMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
             all general settings were checked to start the input handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case the application need to be complete 
            the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StartInputHandler(UINT16 *pIntMask)
{

    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StopInputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StartOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::APPL_StopOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 Ethercat::APPL_GenerateMapping(UINT16* pInputSize,UINT16* pOutputSize)
{
    UINT16 result = ALSTATUSCODE_NOERROR;
    UINT16 PDOAssignEntryCnt = 0;
    OBJCONST TOBJECT OBJMEM * pPDO = NULL;
    UINT16 PDOSubindex0 = 0;
    UINT32 *pPDOEntry = NULL;
    UINT16 PDOEntryCnt = 0;
    UINT16 InputSize = 0;
    UINT16 OutputSize = 0;

    /*Scan object 0x1C12 RXPDO assign*/
    for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
    {
        pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
        if(pPDO != NULL)
        {
            PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
            for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
            {
                pPDOEntry = (UINT32 *)((UINT8 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3));    //goto PDO entry
                // we increment the expected output size depending on the mapped Entry
                OutputSize += (UINT16) ((*pPDOEntry) & 0xFF);
            }
        }
        else
        {
            //assigned PDO was not found in object dictionary. return invalid mapping//
            OutputSize = 0;
            result = ALSTATUSCODE_INVALIDOUTPUTMAPPING;
            break;
        }
    }
    OutputSize = (OutputSize + 7) >> 3;

    if(result == 0)
    {
        /*Scan Object 0x1C13 TXPDO assign*/
        for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
        {
            pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
            if(pPDO != NULL)
            {
                PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
                for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
                {
					pPDOEntry = (UINT32 *)((UINT8 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3));    //goto PDO entry
                    // we increment the expected output size depending on the mapped Entry
                    InputSize += (UINT16) ((*pPDOEntry) & 0xFF);
                }
            }
            else
            {
                //assigned PDO was not found in object dictionary. return invalid mapping
                InputSize = 0;
                result = ALSTATUSCODE_INVALIDINPUTMAPPING;
                break;
            }
        }
    }
    InputSize = (InputSize + 7) >> 3;

    *pInputSize = InputSize;
    *pOutputSize = OutputSize;
  #ifdef DEBUG_OVER_SERIAL
    Serial.println("Ethercat::APPL_GenerateMapping");
    Serial.print("InputSize=");
    Serial.println(InputSize);
    Serial.print("OutputSize=");
    Serial.println(OutputSize);
  #endif
    return result;

}


/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data
\brief      This function will copies the inputs from the local memory to the ESC memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::APPL_InputMapping(UINT16* pData)
{
    UINT16 j = 0;
	UINT16 u16Temp;

	for (j = 0; j < sTxPDOassign.u16SubIndex0; j++)
	{
		switch (sTxPDOassign.aEntries[j])
		{
			case 0x1A00:
				*(pData+0) &= ~0x1;
				u16Temp = Obj0x6000.SubIndex1;
				*(pData+0) |= (u16Temp<<(0))&0x1;
				*(pData+0) &= ~0x2;
				u16Temp = Obj0x6000.SubIndex2;
				*(pData+0) |= (u16Temp<<(1))&0x2;
				*(pData+0) &= ~0x4;
				u16Temp = Obj0x6000.SubIndex3;
				*(pData+0) |= (u16Temp<<(2))&0x4;
				*(pData+0) &= ~0x8;
				u16Temp = Obj0x6000.SubIndex4;
				*(pData+0) |= (u16Temp<<(3))&0x8;
				pData += 1;
				break;
			case 0x1A01:
				*(pData+0) &= ~0xff;
				u16Temp = Obj0x6010.SubIndex1;
				*(pData+0) |= (u16Temp<<(0))&0xff;
				*(pData+1) = *((UINT16*)&(Obj0x6010.SubIndex2)+0);
				*(pData+2) = *((UINT16*)&(Obj0x6010.SubIndex2)+1);
				*(pData+3) = *((UINT16*)&(Obj0x6010.SubIndex3)+0);
				*(pData+4) = *((UINT16*)&(Obj0x6010.SubIndex3)+1);
				*(pData+5) = *((UINT16*)&(Obj0x6010.SubIndex4)+0);
				*(pData+6) = *((UINT16*)&(Obj0x6010.SubIndex4)+1);
				pData += 7;
				break;

			default:
				break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
            to the hardware
s*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::APPL_OutputMapping(UINT16* pData)
{
  
    UINT16 j = 0;
	UINT16 u16Temp;
   for (j = 0; j < sRxPDOassign.u16SubIndex0; j++)
   {
      switch (sRxPDOassign.aEntries[j])
      {
			case 0x1600:
				u16Temp = *(pData+0);
				Obj0x7000.SubIndex1 = (u16Temp&0x1)>>(0);
				u16Temp = *(pData+0);
				Obj0x7000.SubIndex2 = (u16Temp&0x2)>>(1);
				u16Temp = *(pData+0);
				Obj0x7000.SubIndex3 = (u16Temp&0x4)>>(2);
				u16Temp = *(pData+0);
				Obj0x7000.SubIndex4 = (u16Temp&0x8)>>(3);
				u16Temp = *(pData+0);
				Obj0x7000.SubIndex5 = (u16Temp&0x10)>>(4);
				u16Temp = *(pData+0);
				Obj0x7000.SubIndex6 = (u16Temp&0x20)>>(5);
				u16Temp = *(pData+0);
				Obj0x7000.SubIndex7 = (u16Temp&0x40)>>(6);
				u16Temp = *(pData+0);
				Obj0x7000.SubIndex8 = (u16Temp&0x80)>>(7);
				u16Temp = *(pData+0);
				Obj0x7001.SubIndex1 = (u16Temp&0xff00)>>(8);
				u16Temp = *(pData+1);
				Obj0x7001.SubIndex2 = (u16Temp&0xff)>>(0);
				u16Temp = *(pData+1);
				Obj0x7001.SubIndex3 = (u16Temp&0xff00)>>(8);
				u16Temp = *(pData+2);
				Obj0x7001.SubIndex4 = (u16Temp&0xff)>>(0);
				pData += 3;
				break;

		default:
			break;
      }
   }
}



/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/**
Above is the Ethercat slave stack code. DO NOT EDIT!!!
Application code goes below (editable)
**/
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the synchronisation ISR 
            or from the mainloop if no synchronisation is supported
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::APPL_Application(void)
{
	//TODO::Add application code here
        analogWrite(13,Obj0x7001.SubIndex1);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    Arduino setup function
*////////////////////////////////////////////////////////////////////////////////////////

void setup() {
	Serial.begin(115200);
	ethercat.HW_Init();
	Serial.println("EtherCAT hardware init finished.");
        Serial.flush();
	ethercat.MainInit();
	Serial.println("EtherCAT slave setup finished.");
        Serial.flush();
        //TODO::Add application code here
        pinMode(13,OUTPUT);

  }

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    Arduino loop function
*////////////////////////////////////////////////////////////////////////////////////////

// the loop function runs over and over again forever
void loop() {
	ethercat.MainLoop();
	//TODO::Add application code here
	delayMicroseconds(100);            //Do not add too long delay, or CoE of EtherCAT may overtime. 
}

