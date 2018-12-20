/*-----------------------------------------------------------------------------------------
------	
------	ecat_def.h
------   
-----------------------------------------------------------------------------------------*/

#ifndef _ECATDEF_H_
#define _ECATDEF_H_

/*-----------------------------------------------------------------------------------------
------	
------	Includes
------ 
-----------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>

/*-----------------------------------------------------------------------------------------
------	
------	Slave Sample Code Configuration Defines
------	
-----------------------------------------------------------------------------------------*/

/* EL9800_HW: Shall be set if the Slave code shall executed on an EL9800 Evaluation Board 
(if the MCI interface provided by EL9800 board should be used MCI_HW shall be set and this define shall be reset).
Should also set if desired host controller is connected via SPI to the ESC.
NOTE: The PDI type needs also to be configured in the "ESC_CONFIG_DATA". */
#define EL9800_HW                                 1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* MCI_HW: Shall be set if the MCI of the ESC is connected.
NOTE: The PDI type needs also to be configured in the "ESC_CONFIG_DATA". */
#define MCI_HW                                    0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* FC1100_HW: Shall be set if the EtherCAT slave controller is located on an FC1100 PCI card.
NOTE: The PDI type needs also to be configured in the "ESC_CONFIG_DATA". */
#define FC1100_HW                                 0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* CONTROLLER_16BIT: Shall be set if the host controller is a 16Bit architecture */
#define CONTROLLER_16BIT                          1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* CONTROLLER_32BIT: Shall be set if the host controller is a 32Bit architecture */
#define CONTROLLER_32BIT                          0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* _PIC18: Microchip PIC18F452 Specific Code 
This processor is mounted on the Beckhoff Slave Evaluation Board (Hardware version up to EL9800_2). */
#define _PIC18                                    0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* _PIC24: Microchip PIC24HJ128GP306 Specific Code 
This processor is mounted on the Beckhoff Slave Evaluation Board (Hardware version up to EL9800_4A). */
#define _PIC24                                    1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* ESC_16BIT_ACCESS: If the microcontroller only supports 16Bit access to the ESC. */
#define ESC_16BIT_ACCESS                          1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* ESC_32BIT_ACCESS: If the microcontroller only supports 32Bit access to the ESC. */
#define ESC_32BIT_ACCESS                          0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* MBX_16BIT_ACCESS: If the microcontroller only supports 16Bit access to local mailbox memory(access to ESC DPRAM is controlled by "ESC_16BIT_ACCESS"). If reset 8Bit access is used. */
#define MBX_16BIT_ACCESS                          0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* BIG_ENDIAN_16BIT: If the microcontroller always make 16 bit access to external memory, operates in BigEndian format
and the switching of the high and low byte is done in hardware. */
#define BIG_ENDIAN_16BIT                          0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* BIG_ENDIAN_FORMAT: If the microcontroller works with BigEndian format, then this switch shall be set. In that case all WORD-
and DWORD-accesses will make a BYTE- or WORD-swapping, the macros SWAPWORD and SWAPDWORD in ecatslv.h might be adapted. 
If this switch is set, then BIG_ENDIAN_16BIT shall be reset. */
#define BIG_ENDIAN_FORMAT                         0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* EXT_DEBUGER_INTERFACE: If this switch is set, the external debugger interface on the EL9800_4A (_PIC24) will be activated.
This define will be ignored if _PIC24 is not set. */
#define EXT_DEBUGER_INTERFACE                     0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* LEDS_SUPPORTED: Map EtherCAT State to indication led  */
#define LEDS_SUPPORTED                            1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* ESC_SUPPORT_ECAT_LED: This switch can be enabled if the connected ESC support Error and Run LED indication. See the datasheet of the ESC for more details. */
#define ESC_SUPPORT_ECAT_LED                      0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* ESC_EEPROM_EMULATION: If this switch is set EEPROM emulation is supported. Not all ESC types support EEPROM emulation. See ESC datasheet for more information. */
#define ESC_EEPROM_EMULATION                      0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* ESC_EEPROM_SIZE: Specify the EEPROM size in Bytes of the connected EEPROM or the emulated EEPROM. */
#define ESC_EEPROM_SIZE                           0x800

/* EEPROM_READ_SIZE: Only required if EEPROM emulation is active. This value defines the number of bytes which will be read per opertion. */
#define EEPROM_READ_SIZE                          0x8

/* EEPROM_WRITE_SIZE: Only required if EEPROM emulation is active. This value defines the number of bytes which will be written per opertion. */
#define EEPROM_WRITE_SIZE                         0x2

/* AL_EVENT_ENABLED: If an interrupt routine shall be called when one of the Events in the AL Event Register (0x220) changes, 
this switch has to be defined to 1 (synchronous modes are supported). 
If the AL Event register shall only be polled, this switch has to be defined to 0 (only free run mode is supported). */
#define AL_EVENT_ENABLED                          1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* DC_SUPPORTED: If distributed clocks should be supported by the slave, then this switch shall be set.
If this switch is set, then also AL_EVENT_ENABLED shall be set.
NOTE: The DC support needs also be set in the "ESC_CONFIG_DATA" settings. */
#define DC_SUPPORTED                              1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* ECAT_TIMER_INT: If this switch is set, then the watchdog time for the EtherCAT watchdog will be checked in a timer interrupt routine. */
#define ECAT_TIMER_INT                            0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* INTERRUPTS_SUPPORTED: If this switch is set the slave stack provides interrupt handling.
NOTE: value will be evaluated automatically!! */
#define INTERRUPTS_SUPPORTED                      1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* TEST_APPLICATION: This application is used to test features of the slave and/or master. */
#define TEST_APPLICATION                          0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* EL9800_APPLICATION: Should be set if the Slave Sample Code runs on an EL9800_x Evaluation Board. */
#define EL9800_APPLICATION                        1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* CiA402_DEVICE: If this switch is set, then the sample implementation of CiA402 device profile will be activated. */
#define CiA402_DEVICE                             0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* SAMPLE_APPLICATION: Select this define if the hardware independent sample application shall be activated. */
#define SAMPLE_APPLICATION                        0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* SAMPLE_APPLICATION_INTERFACE: Select this define to activate the Sample Application Interface. This provides an simple interface to create a static library and used in an external application.
NOTE: The file "EtherCATSampleLibrary.h" includes the library interface (maybe need to be adapted).  */
#define SAMPLE_APPLICATION_INTERFACE              0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* MAILBOX_QUEUE: If this switch is set, the mailbox services will be stored in a queue.
With this switch reset only one mailbox service can be processed in parallel. */
#define MAILBOX_QUEUE                             1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* AOE_SUPPORTED: If the AoE services are supported, then this switch shall be set. */
#define AOE_SUPPORTED                             0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* COE_SUPPORTED: If the CoE services are supported, then his switch shall be set. */
#define COE_SUPPORTED                             1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* COMPLETE_ACCESS_SUPPORTED: If the complete SDO access (accessing all entries of an object with one SDO service, then this
switch shall be set. Furthermore,COE_SUPPORTED shall be set. */
#define COMPLETE_ACCESS_SUPPORTED                 1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* SEGMENTED_SDO_SUPPORTED: If the segmented SDO services should be supported, then this switch shall be set.
Furthermore, COE_SUPPORTED shall be set. */
#define SEGMENTED_SDO_SUPPORTED                   1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* SDO_RES_INTERFACE: If a SDO response cannot be generated immediately (e.g. when access over
a serial interface is needed), this switch should be set. In that case ABORTIDX_WORKING shall be
returned from OBJ_Read or OBJ_Write and the response shall be sent by calling SDOS_SdoRes, when
the response is available. */
#define SDO_RES_INTERFACE                         1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* BACKUP_PARAMETER_SUPPORTED: If this switch is set, then the functions in the application example to load and
store backup parameter will be compiled. Furthermore, COE_SUPPORTED shall be set. */
#define BACKUP_PARAMETER_SUPPORTED                0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* DIAGNOSIS_SUPPORTED: If this define is set the slave stack supports diagnosis messages (Object 0x10F3). 
To support diagnosis messages COE_SUPPORTED shall be enabled and the platform shall support dynamic memory allocation.
NOTE: this feature is implemented according to ETG.1020 (Draft V.0.9.1) */
#define DIAGNOSIS_SUPPORTED                       0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* EMERGENCY_SUPPORTED: If this define is set the slave stack supports emergency messages. COE_SUPPORTED or SOE_SUPPORTED shall be enabled */
#define EMERGENCY_SUPPORTED                       0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* VOE_SUPPORTED: If the VoE services should be supported, then this switch shall be set. This means only the calling of the 
VoE functions in mailbox.c are implemented, but the VoE service functions have to be added. Furthermore, the example code cannot be linked
correctly, because these functions are missing. */
#define VOE_SUPPORTED                             0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* SOE_SUPPORTED: If the SoE services should be supported, then this switch shall be set. This means only the calling of the 
SoE functions in mailbox.c are implemented, but the SoE service functions have to be added. Furthermore, the example code cannot be linked
correctly, because these functions are missing. */
#define SOE_SUPPORTED                             0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* EOE_SUPPORTED: If the EoE services should be supported, then this switch shall be set. */
#define EOE_SUPPORTED                             0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* STATIC_ETHERNET_BUFFER: If this switch is set a static buffer is used to store ethernet frames, otherwise the buffer is allocated on demand */
#define STATIC_ETHERNET_BUFFER                    0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* FOE_SUPPORTED: If the FoE services should be supported, then this switch shall be set.  */
#define FOE_SUPPORTED                             0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* FOE_SAVE_FILES: If set incoming Files are stored in "aFileData" max file size is set by MAX_FILE_SIZE. */
#define FOE_SAVE_FILES                            0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* MAILBOX_SUPPORTED: If this switch is set the slave stack don't handles mailbox protocols */
#define MAILBOX_SUPPORTED                         1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* BOOTSTRAPMODE_SUPPORTED: If the firmware update over FoE services should be supported, then this switch shall be set.
If this switch is set, then also "FOE_SUPPORTED" shall be set.  */
#define BOOTSTRAPMODE_SUPPORTED                   0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* OP_PD_REQUIRED: If this switch is reset the state transition SAFEOP_2_OP will also successful if no process data was received. The watchdog will only be active when first process data was received (bEcatFirstOutputsReceived) */
#define OP_PD_REQUIRED                            1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* PREOPTIMEOUT: Specify timeout value for the state transition from Init to PreOP/Boot.(ESI Value : "PreopTimeout").
NOTE: Within the stack this value - 50ms will be used to react before the master run into the timeout. */
#define PREOPTIMEOUT                              0x7D0

/* SAFEOP2OPTIMEOUT: Specifiy the timeout from SafeOP to OP. (ESI Value : "SafeopOpTimeout")
NOTE: Within the stack this value - 50ms will be used to react before the master run into the timeout. */
#define SAFEOP2OPTIMEOUT                          0x2328

/* EXPLICIT_DEVICE_ID: If this switch is set Explicit device ID requests are handled. For further information about Explicit Device ID see ETG.1020 specification: http://www.ethercat.org/MemberArea/download_protocolenhancements.asp */
#define EXPLICIT_DEVICE_ID                        0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* ESC_SM_WD_SUPPORTED: This switch should be set if the SyncManger watchdog provided by the ESC should be used. If reset the process data watchdog is triggered by a local timer */
#define ESC_SM_WD_SUPPORTED                       1 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* STATIC_OBJECT_DIC: If this switch is set, the object dictionary is "build" static (by default only PIC18 objects are added static) */
#define STATIC_OBJECT_DIC                         0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!

/* ESC_EEPROM_ACCESS_SUPPORT: If this switch is set the slave stack provides functions to access the EEPROM. */
#define ESC_EEPROM_ACCESS_SUPPORT                 0 //This define was already evaluated by Slave Stack Code Tool(V. 1.1.1.0)!


/*-----------------------------------------------------------------------------------------
------	
------	Compiler Defines
------	
-----------------------------------------------------------------------------------------*/

/* FALSE: Will be used for variables from type BOOL */
#define FALSE                                     0

/* TRUE: Will be used for variables from type BOOL  */
#define TRUE                                      1

/* BOOL: Should be adapted to the boolean type of the microcontroller */
#define BOOL                                      unsigned char

/* UINT8: Should be adapted to the unsigned8 type of the microcontroller  */
#define UINT8                                     unsigned char

/* UINT16: Should be adapted to the unsigned16 type of the microcontroller  */
#define UINT16                                    unsigned short

/* UINT32: Should be adapted to the unsigned32 type of the microcontroller  */
#define UINT32                                    unsigned long

/* USHORT: Should be adapted to the unsigned16 type of the microcontroller */
#define USHORT                                    unsigned short

/* INT8: Should be adapted to the integer8 type of the microcontroller */
#define INT8                                      char

/* INT16: Should be adapted to the integer16 type of the microcontroller  */
#define INT16                                     short

/* INT32: Should be adapted to the integer32 type of the microcontroller */
#define INT32                                     long

/* CHAR: Should be adapted to the character type of the microcontroller */
#define CHAR                                      char

/* UCHAR: Should be adapted to the unsigned character type of the microcontroller */
#define UCHAR                                     unsigned char

/* SIZEOF(x): Used to calculate the size in Bytes */
#define SIZEOF(x)                                 sizeof(x)

/* HUGE: Should be adapted to the huge type of the microcontroller, if the microcontroller
does not support a huge type, HUGE shall be defined to nothing */
#define HUGE 

/* HMEMSET: Should be defined to the memset function for huge memory, if the microcontroller
does not support a huge type, HMEMSET shall be defined to a 'normal' memset function */
#define HMEMSET                                   memset

/* HMEMCPY: Should be defined to the memcpy function for huge memory, if the microcontroller
does not support a huge type, HMEMCPY shall be defined to a 'normal' memcpy function */
#define HMEMCPY                                   memcpy

/* HMEMCMP: Should be defined to the memcmp function for huge memory, if the microcontroller
does not support a huge type, HMEMCMP shall be defined to a 'normal' memcmp function */
#define HMEMCMP                                   memcmp

/* ESCMEM: Should be defined to select the memory type of the ESC memory (e.g. near, far or huge), if the microcontroller
does not support different memory types, ESCMEM shall be defined to nothing */
#define ESCMEM 

/* ESCMEMCPY: Should be defined to the memcpy function for ESCMEM memory, if the microcontroller
does not support different memory types, ESCMEMCPY shall be defined to a 'normal' memcpy function */
#define ESCMEMCPY                                 memcpy

/* ESCMEMSET: Should be defined to the memset function for ESCMEM memory, if the microcontroller
does not support different memory types, ESCMEMSET shall be defined to a 'normal' memset function */
#define ESCMEMSET                                 memset

/* ESCMBXMEMCPY: Should be defined to the memcpy function for copying ESCMEM memory to or from MBXMEM memory, if the microcontroller
does not support different memory types, ESCMBXMEMCPY shall be defined to a 'normal' memcpy function */
#define ESCMBXMEMCPY                              memcpy

/* MBXMEM: Should be defined to select the memory type of the memory used for mailbox communication (e.g. near, far or huge), 
if the microcontroller does not support different memory types, MBXMEM shall be defined to nothing */
#define MBXMEM 

/* MBXMEMCPY: Should be defined to the memcpy function for MBXMEM memory, if the microcontroller
does not support different memory types, MBXMEMCPY shall be defined to a 'normal' memcpy function */
#define MBXMEMCPY                                 memcpy

/* MBXMEMCMP: Should be defined to the memcmp function for MBXMEM memory, if the microcontroller
does not support different memory types, MBXMEMCMP shall be defined to a 'normal' memcmp function */
#define MBXMEMCMP                                 memcmp

/* MBXMEMSET: Should be defined to the memcpy function for MBXMEM memory, if the microcontroller
does not support different memory types, MBXMEMSET shall be defined to a 'normal' memset function */
#define MBXMEMSET                                 memset

/* MBXSTRLEN: Should be defined to the strlen function for MBXMEM memory, if the microcontroller
does not support different memory types, MBXSTRLEN shall be defined to a 'normal' strlen function */
#define MBXSTRLEN                                 strlen

/* MBXSTRCPY: Should be defined to the strcpy function for MBXMEM memory, if the microcontroller
does not support different memory types, MBXSTRCPY shall be defined to a 'normal' strcpy function */
#define MBXSTRCPY                                 memcpy

/* OBJCONST: Should be used to define the object dictionary in ROM (f.e. define OBJCONST const) or
in RAM (e.g. define OBJCONST) */
#define OBJCONST                                  const

/* VARCONST: Should be used to define the constant variables in ROM (f.e. define VARCONST const) or
in RAM (e.g. define VARCONST) */
#define VARCONST 

/* VARVOLATILE: Should be used to prevent dummy variables to be deleted due to compiler optimization.  */
#define VARVOLATILE                               volatile

/* OBJMEM: Should be defined to select the memory type of the memory used for the object dictionary (e.g. near, far or huge), 
if the microcontroller does not support different memory types, OBJMEM shall be defined to nothing */
#define OBJMEM 

/* OBJTOMBXMEMCPY: Should be defined to the memcpy function for copying OBJMEM memory to MBXMEM memory, if the microcontroller
does not support different memory types, OBJTOMBXMEMCPY shall be defined to a 'normal' memcpy function */
#define OBJTOMBXMEMCPY                            memcpy

/* OBJTOMBXSTRCPY: Should be defined to the strcpy function for copying OBJMEM memory to MBXMEM memory, if the microcontroller
does not support different memory types, OBJTOMBXSTRCPY shall be defined to a 'normal' memcpy function */
#define OBJTOMBXSTRCPY                            memcpy

/* OBJMEMCPY: Should be defined to the memcpy function for OBJMEM memory, if the microcontroller
does not support different memory types, OBJMEMCPY shall be defined to a 'normal' memcpy function */
#define OBJMEMCPY                                 memcpy

/* OBJSTRLEN: Should be defined to the strlen function for OBJMEM memory, if the microcontroller
does not support different memory types, OBJSTRLEN shall be defined to a 'normal' strlen function */
#define OBJSTRLEN                                 strlen

/* OBJSTRCPY: Should be defined to the strcpy function for OBJMEM memory, if the microcontroller
does not support different memory types, OBJSTRCPY shall be defined to a 'normal' strcpy function */
#define OBJSTRCPY                                 memcpy

/* MAKE_HUGE_PTR: Should be defined to the initialize a pointer variable with an absolute address */
#define MAKE_HUGE_PTR 

/* MAKE_PTR_TO_ESC: Should be defined to the initialize the pointer to the ESC */
#define MAKE_PTR_TO_ESC 

/* EMCYMEMCPY: Should be defined to the memcpy function for EMCYMEM memory, if the microcontroller
does not support different memory types, EMCYMEMCPY shall be defined to a 'normal' memcpy function */
#define EMCYMEMCPY                                memcpy

/* EMCYMEMSET: Should be defined to the memset function for EMCYMEM memory, if the microcontroller
does not support different memory types, EMCYMEMSET shall be defined to a 'normal' memcset function */
#define EMCYMEMSET                                memset

/* EMCYMEM: Should be defined to select the memory type of the memory used for the emergencies (e.g. near, far or huge), 
if the microcontroller does not support different memory types, EMCYMEM shall be defined to nothing */
#define EMCYMEM 

/* MEMCPY: Should be defined to copy data within local memory. */
#define MEMCPY                                    memcpy

/* ALLOCMEM(size): Should be defined to the alloc function to get dynamic memory */
#define ALLOCMEM(size)                            malloc((size))

/* FREEMEM(pointer): Should be defined to the free function to put back dynamic memory */
#define FREEMEM(pointer)                          free((pointer))

/* VARMEMSET: Should be defined to the memset function for VARMEM memory, if the microcontroller
does not support different memory types, EMCYMEMSET shall be defined to a 'normal' memcset function */
#define VARMEMSET                                 memset

/* VARMEM: Should be defined to select the memory type of the memory used for dynamic memory (e.g. near, far or huge), 
if the microcontroller does not support different memory types, VARMEM shall be defined to nothing */
#define VARMEM 

/* MEM_ADDR: Type to access local memory addresses */
#define MEM_ADDR                                  UINT16

/* GET_MEM_SIZE(ByteSize): Round up the byte size to next matching memory boundary depending on "MEM_ADDR" */
#define GET_MEM_SIZE(ByteSize)                    (((ByteSize)+1) >> 1)

/* APPL_AllocMailboxBuffer(size): Should be defined to a function to get a buffer for a mailbox service,
this is only used if the switch MAILBOX_QUEUE is set */
#define APPL_AllocMailboxBuffer(size)             malloc((size))

/* APPL_FreeMailboxBuffer(pointer): Should be defined to a function to put back a buffer for a mailbox service,
this is only used if the switch MAILBOX_QUEUE is set */
#define APPL_FreeMailboxBuffer(pointer)           free((pointer))

/* STRUCT_PACKED_START: Is defined before the typedef struct construct to pack the generic structures if necessary */
#define STRUCT_PACKED_START 

/* STRUCT_PACKED_END: Is defined after the typedef struct {} construct to pack the generic structures if necessary */
#define STRUCT_PACKED_END 

/* MBX_STRUCT_PACKED_START: Is defined before the typedef struct construct to pack the MAILBOX structures if necessary */
#define MBX_STRUCT_PACKED_START 

/* MBX_STRUCT_PACKED_END: Is defined after the typedef struct {} construct to pack the MAILBOX structures if necessary */
#define MBX_STRUCT_PACKED_END 

/* OBJ_STRUCT_PACKED_START: Is defined before the typedef struct construct to pack the OBJECT structures if necessary */
#define OBJ_STRUCT_PACKED_START 

/* OBJ_STRUCT_PACKED_END: Is defined after the typedef struct {} construct to pack the OBJECT structures if necessary */
#define OBJ_STRUCT_PACKED_END 

/* OBJ_DWORD_ALIGN: Shall be set if the object structures are not Byte aligned and the Code is executed on an 32bit platform */
#define OBJ_DWORD_ALIGN                           0

/* OBJ_WORD_ALIGN: Shall be set if the object structures are not Byte aligned and the Code is executed on an 16bit platform */
#define OBJ_WORD_ALIGN                            1


/*-----------------------------------------------------------------------------------------
------	
------	Application Specific Defines
------	
-----------------------------------------------------------------------------------------*/
//============Not used============
/* MAX_DIAG_MSG: Number of diagnosis message ringbuffer */
#define MAX_DIAG_MSG                              0x14

/* MAX_EMERGENCIES: Number of emergencies supported in parallel */
#define MAX_EMERGENCIES                           0x1

/* MAX_FILE_SIZE: Maximum file size */
#define MAX_FILE_SIZE                             0x500
//============Move to sketch============
/* VENDOR_ID: Object 0x1018 SI1 (Vendor ID)	 
An unique EtherCAT Vendor ID is required. Please find all valid Vendor IDs listed at http://www.ethercat.org/en/vendor_id_list.html.
If your company is not listed, please assign an ID for free at http://www.ethercat.org/memberarea/vendor_id.asp */
//#define VENDOR_ID                                 0x04210909

/* PRODUCT_CODE: Object 0x1018 SI2 (EtherCAT product code) */
//#define PRODUCT_CODE                              0x00030004

/* REVISION_NUMBER: Object 0x1018 SI3 (EtherCAT product revision number) */
//#define REVISION_NUMBER                           0x00010000

/* SERIAL_NUMBER: Object 0x1018 SI4 (EtherCAT product serial number) */
//#define SERIAL_NUMBER                             0x00000000

/* DEVICE_PROFILE_TYPE: Slave device type (Object 0x1000)  */		 //Cia DS401
//#define DEVICE_PROFILE_TYPE                       ((0<<24)|(3<<16)|(401))

/* DEVICE_NAME: Name of the slave device (Object 0x1008) */
//#define DEVICE_NAME                               "DIO044 4Ch. Dig. IO "

/* DEVICE_NAME_LEN: Length of 'DEVICE_NAME' without '\0' */
//#define DEVICE_NAME_LEN                           0x14

/* DEVICE_HW_VERSION: Hardware version of the slave device (Object 0x1009) */
//#define DEVICE_HW_VERSION                         "1.00"

/* DEVICE_HW_VERSION_LEN: Length of 'DEVICE_HW_VERSION' without '\0' */
//#define DEVICE_HW_VERSION_LEN                     0x4

/* DEVICE_SW_VERSION: Software version of the slave device (Object 0x100A) */
//#define DEVICE_SW_VERSION                         "1.00"

/* DEVICE_SW_VERSION_LEN: Length of 'DEVICE_SW_VERSION' without '\0' */
//#define DEVICE_SW_VERSION_LEN                     0x4

/* MIN_PD_WRITE_ADDRESS: Minimum address for the process output data (Sync Manager 2)
inside the application memory of the EtherCAT Slave Controller which could be set by the master. The setting have to be within the ranges of the user memory of the ESC (this is not checked by the tool). */
#define MIN_PD_WRITE_ADDRESS                      0x1000

/* MAX_PD_WRITE_ADDRESS: Maximum address for the process output data (Sync Manager 2)
inside the application memory of the EtherCAT Slave Controller which could be set by the master. The setting have to be within the ranges of the user memory of the ESC (this is not checked by the tool). */
#define MAX_PD_WRITE_ADDRESS                      0x3000

/* MIN_PD_READ_ADDRESS: Minimum address for the process input data (Sync Manager 3)
inside the application memory of the EtherCAT Slave Controller which could be set by the master. The setting have to be within the ranges of the user memory of the ESC (this is not checked by the tool). */
#define MIN_PD_READ_ADDRESS                       0x1000

/* MAX_PD_READ_ADDRESS: Maximum address for the process input data (Sync Manager 3)
inside the application memory of the EtherCAT Slave Controller which could be set by the master. The setting have to be within the ranges of the user memory of the ESC (this is not checked by the tool). */
#define MAX_PD_READ_ADDRESS                       0x3000

/* MIN_MBX_SIZE: Minimum mailbox size (Sync Manager 0 and 1) which could be set by the master. The SSC don't support fragmented SDO info object/entry service => at least entry info including 12byte name shall fit in the mailbox buffer */
#define MIN_MBX_SIZE                              0x0022

/* MAX_MBX_SIZE: Maximum mailbox size (Sync Manager 0 and 1) which could be set by the master. */
#define MAX_MBX_SIZE                              0x0100

/* MIN_MBX_WRITE_ADDRESS: Minimum address for the write (receive) mailbox (Sync Manager 0). The setting have to be within the ranges of the user memory of the ESC (this is not checked by the tool). */
#define MIN_MBX_WRITE_ADDRESS                     0x1000

/* MAX_MBX_WRITE_ADDRESS: Maximum address for the write (receive) mailbox (Sync Manager 0). The setting have to be within the ranges of the user memory of the ESC (this is not checked by the tool). */
#define MAX_MBX_WRITE_ADDRESS                     0x3000

/* MAX_PD_INPUT_SIZE: Maximum size of the process input data (Sync Manager 3) for cyclic exchange. */
#define MAX_PD_INPUT_SIZE                         0x0044

/* MIN_MBX_READ_ADDRESS: Minimum address for the read (send) mailbox (Sync Manager 1). */
#define MIN_MBX_READ_ADDRESS                      0x1000

/* MAX_MBX_READ_ADDRESS: Maximum address for the read (send) mailbox (Sync Manager 1). */
#define MAX_MBX_READ_ADDRESS                      0x3000

/* MAX_PD_OUTPUT_SIZE: Maximum size of the process output data (Sync Manager 2) for cyclic exchange. */
#define MAX_PD_OUTPUT_SIZE                        0x044

/* MIN_PD_CYCLE_TIME: Minimum cycle time in ns the slave is supporting 
(entry 0x1C32:05 or entry 0x1C33:05) */
#define MIN_PD_CYCLE_TIME                         0x7A120

/* MAX_PD_CYCLE_TIME: Maximum cycle time in ns the slave is supporting */
#define MAX_PD_CYCLE_TIME                         0xC3500000

/* PD_OUTPUT_DELAY_TIME: Minimum output delay time in ns the slave is supporting (entry 0x1C32:09) */
#define PD_OUTPUT_DELAY_TIME                      0x0

/* PD_OUTPUT_CALC_AND_COPY_TIME: Output calc+copy time in ns the slave is supporting (entry 0x1C32:06) */
#define PD_OUTPUT_CALC_AND_COPY_TIME              0x0

/* PD_INPUT_CALC_AND_COPY_TIME: Input calc+copy time in ns the slave is supporting (entry 0x1C33:06) */
#define PD_INPUT_CALC_AND_COPY_TIME               0x0

/* PD_INPUT_DELAY_TIME: Input delay time in ns the slave is supporting (entry 0x1C33:09) */
#define PD_INPUT_DELAY_TIME                       0x0



/*-----------------------------------------------------------------------------------------
------	
------	Test Configuration Defines
------	
-----------------------------------------------------------------------------------------*/


#endif // _ECATDEF_H_

