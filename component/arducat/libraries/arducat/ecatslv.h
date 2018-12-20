/*-----------------------------------------------------------------------------------------
------
------    ecatslv.h
------
-----------------------------------------------------------------------------------------*/

#ifndef _ECATSLV_H_
#define _ECATSLV_H_

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

#include "ecat_def.h"

#include "esc.h"



/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////
//
//    General
//

#ifndef OBJGETNEXTSTR
    /* the old definition was not working with all compilers */
    /* old:    #define    OBJGETNEXTSTR(p)    ( (OBJCONST CHAR OBJMEM * )( ((UINT32) p) + OBJSTRLEN( (OBJCONST CHAR OBJMEM *) p ) + 1 ) ) */
    #define    OBJGETNEXTSTR(p)    ( (OBJCONST CHAR OBJMEM * )(  &((p)[OBJSTRLEN( (OBJCONST CHAR OBJMEM *) (p) ) + 1]) ) )
#endif

#ifndef LO_BYTE
    #define    LO_BYTE                            0
#endif

#ifndef HI_BYTE
    #define    HI_BYTE                            1
#endif

#ifndef LOLO_BYTE
    #define    LOLO_BYTE                          0
#endif

#ifndef LOHI_BYTE
    #define    LOHI_BYTE                          1
#endif

#ifndef HILO_BYTE
    #define    HILO_BYTE                         2
#endif

#ifndef HIHI_BYTE
    #define    HIHI_BYTE                          3
#endif

#ifndef LO_WORD
    #define    LO_WORD                            0
#endif

#ifndef HI_WORD
    #define    HI_WORD                            1
#endif

#ifndef SWAPWORD
    #define    SWAPWORD(x)                        (x)
#endif

#ifndef SWAPDWORD
    #define    SWAPDWORD(x)                     (x)
#endif

#ifndef LOBYTE
    #define    LOBYTE(x)                        ((x)&0xFF)
#endif

#ifndef HIBYTE
    #define    HIBYTE(x)                        (((x)&0xFF00)>>8)
#endif

#ifndef LOLOBYTE
    #define    LOLOBYTE(x)                        ((x)&0xFF)
#endif

#ifndef LOHIBYTE
    #define    LOHIBYTE(x)                        (((x)&0xFF00)>>8)
#endif

#ifndef HILOBYTE
    #define    HILOBYTE(x)                        (((x)&0xFF0000)>>16)
#endif

#ifndef HIHIBYTE
    #define    HIHIBYTE(x)                        (((x)&0xFF000000)>>24)
#endif

#ifndef LOWORD
    #define    LOWORD(x)                        ((x)&0xFFFF)
#endif

#ifndef HIWORD
    #define    HIWORD(x)                        (((x)&0xFFFF0000)>>16)
#endif

#ifndef BIT2BYTE
    #define    BIT2BYTE(x)        (((x)+7)>>3)
#endif

#ifndef BYTE2BIT
    #define    BYTE2BIT(x)        ((x)<<3)
#endif

#ifndef BIT2WORD
    #define    BIT2WORD(x)        (((x)+15)>>4)
#endif

#ifndef BYTE2WORD
    #define    BYTE2WORD(x)    (((x)+1)>>1)
#endif

#ifndef ROUNDUPBYTE2WORD
    #define    ROUNDUPBYTE2WORD(x)    ((((x)+1)>>1)<<1)
#endif

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// State defines
*/

#define     STATE_INIT                        ((UINT8) 0x01)
#define     STATE_PREOP                     ((UINT8) 0x02)
#define     STATE_BOOT                        ((UINT8) 0x03)
#define     STATE_SAFEOP                    ((UINT8) 0x04)
#define     STATE_OP                            ((UINT8) 0x08)

#define        STATE_MASK                        ((UINT8) 0x0F)
#define     STATE_CHANGE                    ((UINT8) 0x10)
#define     STATE_DEVID                     ((UINT8) 0x20)

#define     BOOT_2_INIT                        (((STATE_BOOT) << 4) | (STATE_INIT))

#define     INIT_2_BOOT                        (((STATE_INIT) << 4) | (STATE_BOOT))
#define     PREOP_2_BOOT                    (((STATE_PREOP) << 4) | (STATE_BOOT))
#define     SAFEOP_2_BOOT                    (((STATE_SAFEOP) << 4) | (STATE_BOOT))
#define     OP_2_BOOT                        (((STATE_OP) << 4) | (STATE_BOOT))

#define     INIT_2_INIT                        (((STATE_INIT) << 4) | (STATE_INIT))
#define     INIT_2_PREOP                    (((STATE_INIT) << 4) | (STATE_PREOP))
#define     INIT_2_SAFEOP                    (((STATE_INIT) << 4) | (STATE_SAFEOP))
#define     INIT_2_OP                        (((STATE_INIT) << 4) | (STATE_OP))

#define     PREOP_2_INIT                    (((STATE_PREOP) << 4) | (STATE_INIT))
#define     PREOP_2_PREOP                    (((STATE_PREOP) << 4) | (STATE_PREOP))
#define     PREOP_2_SAFEOP                    (((STATE_PREOP) << 4) | (STATE_SAFEOP))
#define     PREOP_2_OP                        (((STATE_PREOP) << 4) | (STATE_OP))

#define     SAFEOP_2_INIT                    (((STATE_SAFEOP) << 4) | (STATE_INIT))
#define     SAFEOP_2_PREOP                      (((STATE_SAFEOP) << 4) | (STATE_PREOP))
#define     SAFEOP_2_SAFEOP                    (((STATE_SAFEOP) << 4) | (STATE_SAFEOP))
#define     SAFEOP_2_OP                        (((STATE_SAFEOP) << 4) |( STATE_OP))

#define     OP_2_INIT                        (((STATE_OP) << 4) | (STATE_INIT))
#define     OP_2_PREOP                        (((STATE_OP) << 4) | (STATE_PREOP))
#define     OP_2_SAFEOP                        (((STATE_OP) << 4) | (STATE_SAFEOP))
#define     OP_2_OP                            (((STATE_OP) << 4) | (STATE_OP))

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// ESM transition error codes
*/

#define    SYNCMANCHODDADDRESS                                     0x00
#define    SYNCMANCHADDRESS                                         0x01
#define    SYNCMANCHSIZE                                            0x02
#define    SYNCMANCHSETTINGS                                        0x03
#define    ERROR_SYNCMANCH(code, channel)                    ((code)+((channel)<<2))
#define    ERROR_SYNCMANCHODDADDRESS(channel)                ((SYNCMANCHODDADDRESS)+((channel)<<2))
#define    ERROR_SYNCMANCHADDRESS(channel)                    ((SYNCMANCHADDRESS)+((channel)<<2))
#define    ERROR_SYNCMANCHSIZE(channel)                          ((SYNCMANCHSIZE)+((channel)<<2))
#define    ERROR_SYNCMANCHSETTINGS(channel)                    ((SYNCMANCHSETTINGS)+((channel)<<2))
#define    ERROR_SYNCTYPES                                        0x80
#define    ERROR_DCSYNCCONTROL                                    0x81
#define    ERROR_DCSYNC0CYCLETIME                                0x82
#define    ERROR_DCSYNC1CYCLETIME                                0x83
#define    ERROR_DCCYCLEPARAMETER                                0x84
#define    ERROR_DCLATCHCONTROL                                    0x85

#define    ERROR_INVALIDSTATE                                    0xF0
#define    ERROR_NOMEMORY                                            0xF1
#define    ERROR_OBJECTDICTIONARY                                0xF2
#define    ERROR_NOSYNCMANACCESS                                0xF3
#define    ERROR_NOOFRXPDOS                                        0xF4
#define    ERROR_NOOFTXPDOS                                        0xF5
#define    ERROR_STATECHANGE                                        0xF6

#define    NOERROR_NOSTATECHANGE                                0xFE
#define    NOERROR_INWORK                                            0xFF

#define    EMCY_SM_ERRORCODE                                        0xA000
#define    EMCY_SM_DEVICESPECIFIC                                0xFF00

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// AL Status Codes
*/

#define    ALSTATUSCODE_NOERROR                        0x0000
#define    ALSTATUSCODE_UNSPECIFIEDERROR               0x0001
#define    ALSTATUSCODE_NOMEMORY                       0x0002
#define    ALSTATUSCODE_INVALIDALCONTROL               0x0011
#define    ALSTATUSCODE_UNKNOWNALCONTROL               0x0012
#define    ALSTATUSCODE_BOOTNOTSUPP                    0x0013
#define    ALSTATUSCODE_NOVALIDFIRMWARE                0x0014
#define    ALSTATUSCODE_INVALIDMBXCFGINBOOT            0x0015
#define    ALSTATUSCODE_INVALIDMBXCFGINPREOP           0x0016
#define    ALSTATUSCODE_INVALIDSMCFG                   0x0017
#define    ALSTATUSCODE_NOVALIDINPUTS                  0x0018
#define    ALSTATUSCODE_NOVALIDOUTPUTS                 0x0019
#define    ALSTATUSCODE_SYNCERROR                      0x001A
#define    ALSTATUSCODE_SMWATCHDOG                     0x001B
#define    ALSTATUSCODE_SYNCTYPESNOTCOMPATIBLE         0x001C
#define    ALSTATUSCODE_INVALIDSMOUTCFG                0x001D
#define    ALSTATUSCODE_INVALIDSMINCFG                 0x001E
#define    ALSTATUSCODE_INVALIDWDCFG                   0x001F
#define    ALSTATUSCODE_WAITFORCOLDSTART               0x0020
#define    ALSTATUSCODE_WAITFORINIT                    0x0021
#define    ALSTATUSCODE_WAITFORPREOP                   0x0022
#define    ALSTATUSCODE_WAITFORSAFEOP                  0x0023
#define    ALSTATUSCODE_INVALIDINPUTMAPPING            0x0024
#define    ALSTATUSCODE_INVALIDOUTPUTMAPPING           0x0025
#define    ALSTATUSCODE_INCONSISTENTSETTINGS           0x0026
#define    ALSTATUSCODE_FREERUNNOTSUPPORTED            0x0027
#define    ALSTATUSCODE_SYNCHRONNOTSUPPORTED           0x0028
#define    ALSTATUSCODE_FREERUNNEEDS3BUFFERMODE        0x0029
#define    ALSTATUSCODE_BACKGROUNDWATCHDOG             0x002A
#define    ALSTATUSCODE_NOVALIDINPUTSANDOUTPUTS        0x002B
#define    ALSTATUSCODE_FATALSYNCERROR                 0x002C
#define    ALSTATUSCODE_NOSYNCERROR                    0x002D
#define    ALSTATUSCODE_DCINVALIDSYNCCFG               0x0030
#define    ALSTATUSCODE_DCINVALIDLATCHCFG              0x0031
#define    ALSTATUSCODE_DCPLLSYNCERROR                 0x0032
#define    ALSTATUSCODE_DCSYNCIOERROR                  0x0033
#define    ALSTATUSCODE_DCSYNCMISSEDERROR              0x0034
#define    ALSTATUSCODE_DCINVALIDSYNCCYCLETIME         0x0035
#define    ALSTATUSCODE_DCSYNC0CYCLETIME               0x0036
#define    ALSTATUSCODE_DCSYNC1CYCLETIME               0x0037
#define    ALSTATUSCODE_MBX_AOE                        0x0041
#define    ALSTATUSCODE_MBX_EOE                        0x0042
#define    ALSTATUSCODE_MBX_COE                        0x0043
#define    ALSTATUSCODE_MBX_FOE                        0x0044
#define    ALSTATUSCODE_MBX_SOE                        0x0045
#define    ALSTATUSCODE_MBX_VOE                        0x004F
#define    ALSTATUSCODE_EE_NOACCESS                    0x0050
#define    ALSTATUSCODE_EE_ERROR                       0x0051

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// AL event masks
*/

#define     AL_CONTROL_EVENT                    ((UINT16) 0x01)
#define     SYNC0_EVENT                         ((UINT16) 0x04)
#define     SYNC1_EVENT                         ((UINT16) 0x08)
#define     SM_CHANGE_EVENT                     ((UINT16) 0x10)
#define     EEPROM_CMD_PENDING                  ((UINT16) 0x20)

#ifndef MAX_PD_SYNC_MAN_CHANNELS
    #define    MAX_PD_SYNC_MAN_CHANNELS         2
#endif
#define    MAX_NUMBER_OF_SYNCMAN                ((MAX_PD_SYNC_MAN_CHANNELS)+2)

#define    MAILBOX_WRITE                        0
#define    MAILBOX_READ                         1
#define    PROCESS_DATA_OUT                     2
#define    PROCESS_DATA_IN                      3

#define     MAILBOX_WRITE_EVENT                 ((UINT16) 0x0100)
#define     MAILBOX_READ_EVENT                  ((UINT16) 0x0200)
#define     PROCESS_OUTPUT_EVENT                ((UINT16) 0x0400)
#define     PROCESS_INPUT_EVENT                 ((UINT16) 0x0800)

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// Codes for LED
//    bit 7: invert flag
//    bit 6: toggle    (if toggle == 1 and number of flashes == 0 => infinite toggle)
//    bit 5: fast toggle flag (50ms cycle)
//    bit 4-0: number of flashes
*/
#define    LED_OFF                      0x00
#define    LED_FLICKERING               0x60
#define    LED_BLINKING                 0x40
#define    LED_SINGLEFLASH              0x41
#define    LED_DOUBLEFLASH              0x42
#define    LED_INVERT_DOUBLEFLASH       0xC2
#define    LED_ON                       0x01


/*/////////////////////////////////////////////////////////////////////////////////////////
//
// Addresses
*/
#define    MEMORY_START_ADDRESS            0x1000

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// Overwrites
*/

#ifndef    DC_SYNC_ACTIVE
    #define    DC_SYNC_ACTIVE                ESC_DC_SYNC0_ACTIVE_MASK
#endif
#ifndef    DC_EVENT_MASK
    #define    DC_EVENT_MASK                PROCESS_OUTPUT_EVENT
#endif

/*/////////////////////////////////////////////////////////////////////////////////////////
//
// Data Types
*/
#define MAX_SM_EVENT_MISSED             4

#endif //_ECATSLV_H_

/*-----------------------------------------------------------------------------------------
------
------    global variables
------
-----------------------------------------------------------------------------------------*/










