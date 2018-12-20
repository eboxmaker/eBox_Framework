/*-----------------------------------------------------------------------------------------
------
------    Description
------
------    esc.h
------
------    EtherCAT Slave Controller
------                                                                                                                                                                 ------
-----------------------------------------------------------------------------------------*/

#ifndef _ESC_H_
#define _ESC_H_

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

///////////////////////////////////////////////////////////////
//
//    Standard Data Types
//

#define SIZEOF_SM_REGISTER                      8       //Each SyncManger has 8Byte Configuration/Status Registers
#define MAX_NO_OF_SYNC_MAN                      16      //Maximum Number of SyncManager channels supported by an ESC

#ifndef    BL_PAGE_SIZE
    #define    BL_PAGE_SIZE        512
#endif




///////////////////////////////////////////////////////////////
//
//    ESC Offsets (Detailed Information about the registers are located in the ESC Datasheets e.g. ET1100 Datasheet http://www.beckhoff.com/english.asp?download/ethercat_development_products.htm?id=71003127100387)
//
#define ESC_INFO_OFFSET                         0x0000                              //ESC information registers startoffset

#define ESC_COMM_INFO_OFFSET                    0x0004                              //Communication information registers startoffset

#define ESC_SM_CHANNELS_MASK                    0xFF00                              //Mask based on ESC offset "ESC_COMM_INFO_OFFSET"
#define ESC_SM_CHANNELS_SHIFT                   8                                   //Bit shift based on ESC offset "ESC_COMM_INFO_OFFSET"

#define ESC_FEATURE_ADDRESS_OFFSET              0x0008
/* ECATCHANGE_START(V5.01) ESC3*/
#define ESC_SLAVE_ADDRESS_OFFSET                0x0010                              //Register Description: Address used for node addressing (FPxx commands)
/* ECATCHANGE_START(V5.01) ESC3*/


#define ESC_AL_CONTROL_OFFSET                   0x0120                              //Register Description: Initiate State Transition of the Device State Machine
#define ESC_AL_STATUS_OFFSET                    0x0130                              //Register Description: Actual State of the Device State Machine
#define ESC_AL_STATUS_CODE_OFFSET               0x0134                              //Register Description: AL Status Code

#define ESC_RUN_LED_OVERRIDE                    0x0138                              //Register Description: Set Ecat Run indication via ESC. (not all ESC types support this feature)
#define ESC_ERROR_LED_OVERRIDE                  0x0139                              //Register Description: Set Ecat Error indication via ESC. (not all ESC types support this feature)

#define ESC_PDI_CONTROL_OFFSET                  0x0140                              //Register Description: Specifies the process data interface

#define ESC_AL_EVENTMASK_OFFSET                 0x0204                              //Register Description: AL Event masking of the AL Event Request register Events for mapping to PDI IRQ signal
#define ESC_AL_EVENT_OFFSET                     0x0220                              //Register Description: "Mirror" register for ESC events

#define ESC_WD_DIVIDER_OFFSET                   0x0400                              //Register Description: Number of 25 MHz tics (minus 2) that represents the basic watchdog increment. (Default value is 100us = 2498)

#define ESC_PD_WD_TIME                          0x0420                              //Register Description: Number of basic watchdog increments (Default value with Watchdog divider 100us means 100ms Watchdog)
#define ESC_PD_WD_STATE                         0x0440                              //Register Description: Watchdog Status of Process Data (triggered by SyncManagers)
#define ESC_PD_WD_TRIGGER_MASK                  0x0001                              //Trigger state of the process data watchdog

#define ESC_EEPROM_CONFIG_OFFSET                0x0500                              //Register Description: EEPROM Access Configuration
/*EEPROM config and access state bit mask (based on "ESC_EEPROM_CONFIG_OFFSET") - START*/
#define ESC_EEPROM_ASSIGN_TO_PDI_MASK           0x0001                              //Description (0x500.0): PDI has EEPROM control
#define ESC_EEPROM_LOCKED_BY_PDI_MASK           0x0100                              //Description (0x500.8): PDI locked EEPROM access
/*EEPROM config and access state bit mask (based on "ESC_EEPROM_CONFIG_OFFSET") - END*/

#define ESC_EEPROM_CONTROL_OFFSET               0x0502
/* EEPROM command and status bit masks (based on "ESC_EEPROM_CONTROL_OFFSET") - START*/
#define ESC_EEPROM_SUPPORTED_READBYTES_MASK     0x0040                              //Description (0x502.6): Supported number of EEPROM read bytes: 0-> 4 Bytes; 1 -> 8 Bytes
#define ESC_EEPROM_CMD_MASK                     0x0700                              //Description (0x502.8:10): Command bit mask
#define ESC_EEPROM_CMD_READ_MASK                0x0100                              //Description (0x502.8): Currently executed read command
#define ESC_EEPROM_CMD_WRITE_MASK               0x0200                              //Description (0x502.9): Initialize Write Command
#define ESC_EEPROM_CMD_RELOAD_MASK              0x0400                              //Description (0x502.10): Trigger EEPROM reload
#define ESC_EEPROM_ERROR_MASK                   0x7800                              //Description : Mask all EEPROM error bits; Checksum error (0x0502.11); EEPROM not loaded (0x0502.12); Missing EEPROM Acknowledge (0x0502.13); Write Error (0x0502.14)
#define ESC_EEPROM_ERROR_CRC                    0x0800                              //Description (0x502.11): EEPROM CRC Error
#define ESC_EEPROM_ERROR_CMD_ACK                0x2000                              //Description (0x502.13): EEPROM Busy
#define ESC_EEPROM_BUSY_MASK                    0x8000                              //Description (0x502.15): EEPROM Busy
/* EEPROM command and status bit masks (based on "ESC_EEPROM_CONTROL_OFFSET") - END*/
#define ESC_EEPROM_ADDRESS_OFFSET               0x0504
#define ESC_EEPROM_DATA_OFFSET                  0x0508

#define ESC_SYNCMAN_REG_OFFSET                  0x0800                              //Register Description: Start address of the SyncManager Configuration/Staus registers
#define ESC_SYNCMAN_CONTROL_OFFSET              0x0804                              //Register Description: SyncManager Setting Register


#define ESC_SYNCMAN_ACTIVE_OFFSET               0x0806                              //Register Description: SyncManager Activation Register


#define    ESC_SYSTEMTIME_OFFSET                0x0910                              //Register Description: Local copy of the System Time

#define    ESC_DC_UNIT_CONTROL_OFFSET           0x0980                              //Register Description: Control registers for Cycle and Sync Unit (can be predefined with the "AssignActivate" Element in the device description, for further information see ETG.2000)
#define ESC_DC_SYNC_UNIT_ACTIVE_MASK            0x0100                              //Description (0x980.8): Sync Out Unit is activated
#define ESC_DC_SYNC0_ACTIVE_MASK                0x0200                              //Description (0x980.9): Sync0 generation is activated
#define ESC_DC_SYNC1_ACTIVE_MASK                0x0400                              //Description (0x980.10): Sync1 generation is activated

#define ESC_DC_SYNC0_CYCLETIME_OFFSET           0x09A0                              //Register Description: 32Bit Time between two consecutive SYNC0 pulses in ns
#define ESC_DC_SYNC1_CYCLETIME_OFFSET           0x09A4                              //Register Description: 32Bit Time between two consecutive SYNC1 pulses in ns



/*---------------------------------------------------------------------------------
------                                                                        ------
------    Typdefinitionen                                                        ------
------                                                                        ------
---------------------------------------------------------------------------------*/

/****************************************************************
**
** Sync Manager
*/

typedef struct STRUCT_PACKED_START
{

  UINT16                        PhysicalStartAddress;
  UINT16                        Length;

    UINT16                        Settings[2];

/*Defines to access :
* - SM Control register 0x0804
* - SM Status register 0x0805
*
* all defines are based on 0x0804
*/

/*SyncManger control (0x0804) access*/
#define SM_SETTING_CONTROL_OFFSET           0                       //Offset to value of register 0x0804
#define SM_SETTING_MODE_MASK                0x0002
#define SM_SETTING_MODE_THREE_BUFFER_VALUE  0x0000
#define SM_SETTING_MODE_ONE_BUFFER_VALUE    0x0002
#define SM_SETTING_DIRECTION_MASK           0x000C
#define SM_SETTING_DIRECTION_READ_VALUE     0x0000
#define SM_SETTING_DIRECTION_WRITE_VALUE    0x0004
#define SM_SETTING_WATCHDOG_VALUE           0x0040

/*SyncManger status (0x0805) access*/
#define SM_STATUS_MBX_BUFFER_FULL           0x0800                 //Indicates in one buffer mode if buffer was completely written

/*Defines to access :
* - SM Active register 0x0806
* - SM PDI Control register 0x0807
*
* all defines are based on 0x0806
*/

/*SyncManger active (0x0806) access*/
#define SM_SETTING_ACTIVATE_OFFSET          1                       //Offset to value of register 0x0806
#define SM_SETTING_ENABLE_VALUE             0x0001
#define SM_SETTING_REPAET_REQ_MASK          0x0002
#define SM_SETTING_REPEAT_REQ_SHIFT         0

/*SyncManger PDI Control (0x0807) access*/
#define SM_SETTING_PDI_DISABLE              0x0100                  //Bit0 of register 0x0807 (if 1 SM is disabled from PDI)
#define SM_SETTING_REPEAT_ACK               0x0200                  //Bit1 of register 0x0807
}STRUCT_PACKED_END
TSYNCMAN;


#define SYNCMAN_REG_SIZE        8   //"TSYNCMAN" structure size in bytes


#endif //_ESC_H_


