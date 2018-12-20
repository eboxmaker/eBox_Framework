/**
\defgroup coeappl coeappl.c: CoE (CAN application layer over EtherCAT) - application interface
\brief This file contains an example for CoE services and the CoE object dictionary\n
\brief Changes to version V5.0:
\brief V5.01 APPL3: Include library demo application
\brief V5.01 COE1: Remove alignment entry (SI33) from objects 0x1C32/0x1C33
\brief V5.01 EL9800 1: Read/Write functions for setting object 0x8020 moved to el9800appl.c
\brief V5.01 EL9800 2: Add TxPdo Parameter object 0x1802
\brief V5.01 SDO6: Update SDO response interface handling. (used if the object access function returns "ABORTIDX_WORKING" and SDO_RES_INTERFACE is active)
\brief Changes to version V4.40:
\brief V5.0 TEST1: Add test application. See Application Note ET9300 for more details.
\brief V5.0 COE1: Add reserved Subindex7 to object 0x1C32 and 0x1C33.
\brief V5.0 COE4: Create object dictionary on device startup.
\brief V5.0 COE6: Update entry descriptions for object with the code ARRAY.
\brief V5.0 ECAT2: Reference to application specific object dictionary is set to "ApplicationObjDic".
\brief Changes to version V4.30:\n
\brief V4.40 COE 6: change dynamic object dictionary handling
\brief V4.40 CiA402 2: enable freerun Sync Mode for CiA402 Device (The motion controller application will only be triggered if DC Synchronisation is disabled)
\brief V4.40 COE5: move identification and synchronisation object values to ecat_def.h
\brief V4.40 DIAG1: Add diagnosis object 0x10F3
\brief V4.40 OBJ3: Update the global object dictionary pointer if a new list head is added
\brief V4.40 COE4: Prevent access NULL pointer
\brief V4.40 SYNC1: Initialize 0x1C3x objects
\brief V4.40 HW1: Add (adapt) objects for FC1100 support
\brief Changes to version V4.20:\n
\brief V4.30 OBJ 3: Handle object dictionary in double link list
\brief V4.30 OBJ 2: General EtherCAT device objects are moved from chnappl.h to coeappl.c
\brief V4.30 SYNC: set 0x1C32:05; 0x1C33:05 (minCyleTime) to MIN_PD_CYCLE_TIME
                    (should be set dynamic if required)
\brief V4.30 PDO: rename PDO specific function calls
                    (PDO specific functions are moved to ecatappl.c)
\brief V4.20 PIC24: Add EL9800_4 (PIC24) required source code
\brief V4.11 ECAT 1: C166: MinDelayTime renamed in CalcAndCopyTime\n
\brief Changes to version V4.07:\n
\brief V4.10 ECAT 1: To save and restore arrays in the object directory.
\brief                    The code to get the size of an array entry was changed.\n
\brief V4.10 COE 1:  The SM-Parameter were extended\n
\brief Changes to version V4.06:\n
\brief V4.07 COEAPPL 1: The PDO numbers were adapted according top the modular device profile,
\brief                        the PDO numbers increment now with every module (in chnappl.h)
\brief V4.07 COEAPPL 2: The example is working for the NIOS with the evaluation board DBC2C20 V1.2
\brief                        which is available by Altera\n
\brief V4.07 ECAT 1: The sources for SPI and MCI were merged (in ecat_def.h
\brief                    set the switch MCI_HW to 1 when using the MCI,
\brief                    set the switch SPI_HW to 1 when using the SPI\n
\brief Changes to version V4.03:\n
\brief V4.04 SDO 1: The SDO interface was changed in that way that a SDO response
\brief              could be sent by the application to a later time. In that case
\brief                  the functions OBJ_Read and OBJ_Write shall return the value
\brief                  ABORTIDX_WORKING. To send the SDO response the new function SDOS_SdoRes
\brief                  has to be called by the application. While waiting for the call
\brief                  of SDOS_SdoRes the SDO interface will answer to another SDO request
\brief                  with the error MBXERR_SERVICEINWORK in the mailbox protocol.
\brief                  In this example the reading and writing of object 0x8020 is delayed,
\brief                  the function SDOS_SdoRes will be called from COE_Main.
\brief Changes to version V4.00:\n
\brief V4.01 COEAPPL 1: The object dictionary and the mapping functions are now defined
\brief                  in chnappl.h (chnappl.inc made problems with some developing tools)\n
\brief Changes to version V3.20:\n
\brief V4.00 COEAPPL 1: The object dictionary and the mapping functions are now defined
\brief                  in chnappl.c and were adapted to the modular device profile\n
\brief V4.00 COEAPPL 2: The handling of backup parameters was included according to
\brief                  the EtherCAT Guidelines and Protocol Enhancements Specification\n
\brief V4.00 COEAPPL 3: The handling of the TxPDO Toggle was included according to
\brief                  the EtherCAT Guidelines and Protocol Enhancements Specification\n
\brief V4.00 COEAPPL 4: The handling of the TxPDO State was included according to
\brief                  the EtherCAT Guidelines and Protocol Enhancements Specification\n
\brief V4.00 ECAT 1: The handling of the Sync Manager Parameter was included according to
\brief               the EtherCAT Guidelines and Protocol Enhancements Specification\n


\version 5.01
*/

//---------------------------------------------------------------------------------------
/**
\ingroup coeappl
\file coeappl.c
\brief Implementation.
*/
//---------------------------------------------------------------------------------------

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

//#include "ecat_def.h"

//#include "objdef.h"

#include "ethercat.h"


/*-----------------------------------------------------------------------------------------
------
------    Backup Parameter
------
-----------------------------------------------------------------------------------------*/
/*
extern const UINT32 VENDOR_ID;
extern const UINT32 PRODUCT_CODE;
extern const UINT32 REVISION_NUMBER;
extern const UINT32 DEVICE_PROFILE_TYPE;
extern const char DEVICE_NAME[]; 
extern const UINT8 DEVICE_NAME_LEN;
extern const char DEVICE_HW_VERSION[];
extern const UINT8 DEVICE_HW_VERSION_LEN;
extern const char  DEVICE_SW_VERSION[];
extern const UINT8 DEVICE_SW_VERSION_LEN;
*/
extern TOBJECT    OBJMEM GenObjDic[];
extern TOBJECT    OBJMEM ApplicationObjDic[];


/*-----------------------------------------------------------------------------------------
------
------    Functions
------
-----------------------------------------------------------------------------------------*/
/**
\addtogroup coeappl
@{
*/


/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    returns the pointer to the object dictionary
*////////////////////////////////////////////////////////////////////////////////////////

OBJCONST TOBJECT OBJMEM * Ethercat::COE_GetObjectDictionary(void)
{
    return (OBJCONST TOBJECT OBJMEM *) ObjDicList;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief     This function initialize the several objects
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::COE_ObjInit(void)
{

    /* initialize the Sync Manager Output parameter object 0x1C32 */

    sSyncManOutPar.subindex0         = 32;
    /*
    subindex 1 contains the actual synchronization mode, it could be written
      from the master to switch between ECAT FreeRun and ECAT Synchron Mode
        if the slave supports both modes,
        in DC mode (selected by the DC registers) this value will be overwritten
        with SYNCTYPE_DCSYNC0 or SYNCTYPE_DCSYNC1 */
     /*default mode is ECAT Synchron Mode */
    sSyncManOutPar.u16SyncType     = SYNCTYPE_SYNCHRON;
    /* subindex 2 contains the cycle time of the application,
       in ECAT FreeRun mode it could be used for a timer interrupt to run the application,
        in ECAT Synchron mode it could be written from the master with its local cycle time
        that the slave can check if this cycle time is supported,
        in DC Mode this value will be overwritten with the DC cycle time register */
    sSyncManOutPar.u32CycleTime     = 0;
    /* only for DC Mode important: the subindex 3 contains the time shift between the
       SYNC0 (SYNC1) signal and when the outputs are put to the hardware to allow the
        master a very exactly calculation of delay times
        (in this example we will make an online measurement in the ESC Interrupt Routine) */
    sSyncManOutPar.u32ShiftTime     = 0;
    /* the subindex 4 contains the supported synchronization types */
    sSyncManOutPar.u16SyncTypesSupported    = SYNCTYPE_TIMESVARIABLE        /* the execution times depend on the connected modules */
                                                        | SYNCTYPE_FREERUNSUPP            /* ECAT FreeRun Mode is supported */
                                                        | SYNCTYPE_SYNCHRONSUPP            /* ECAT Synchron Mode is supported */
                                                        | SYNCTYPE_DCSYNC0SUPP            /* DC Sync0 Mode is supported */
    ;
    /* subindex 5 contains the minimum cycle time the slave is able to support,
       will be calculated dynamically because it depends on the connected modules
        (in this example we will make an online measurement in the ESC Interrupt Routine).
        For the sample application this value is set to MIN_PD_CYCLE_TIME */
    sSyncManOutPar.u32MinCycleTime = MIN_PD_CYCLE_TIME;
    /* only for DC Mode important: subindex 6 contains the minimum delay time the slave
       needs after receiving the SM2-event before the SYNC0(SYNC1) can be received without delays
       will be calculated dynamically because it depends on the connected modules
        (in this example we will make an online measurement in the ESC Interrupt Routine) */
    sSyncManOutPar.u32CalcAndCopyTime = (PD_OUTPUT_CALC_AND_COPY_TIME);

    /*subindex 8: trigger cycle time measurement*/
    sSyncManOutPar.u16GetCycleTime = 0;

    /*subindex 9: time from start driving outputs until outputs are valid*/
    sSyncManOutPar.u32DelayTime = (PD_OUTPUT_DELAY_TIME);

    /*subindex 32: indicates if a synchronisation error has occurred*/
    sSyncManOutPar.u16SyncError = 0;

    /* initialize the Sync Manager Input parameter object 0x1C33 */
    sSyncManInPar.subindex0         = 32;
    /* default mode is ECAT Synchron Mode, if output size > 0 the inputs are updated with the SM2-event */
    sSyncManInPar.u16SyncType         = SYNCTYPE_SM2INT;
    /* subindex 2: same as 0x1C32:02 */
    sSyncManInPar.u32CycleTime     = sSyncManOutPar.u32CycleTime;
    /* only for DC Mode important: subindex 3 contains the time shift between the
       SYNC0 (SYNC1) signal and when the inputs are got to the hardware to allow the
        master a very exactly calculation of delay times,
       will be calculated dynamically because it depends on the connected modules
        (in this example we will make an online measurement in the ESC Interrupt Routine) */
    sSyncManInPar.u32ShiftTime     = 0;
    /* subindex 4: same as 0x1C32:04 */
    sSyncManInPar.u16SyncTypesSupported    = sSyncManOutPar.u16SyncTypesSupported;
    /* subindex 5: same as 0x1C32:05 */
    sSyncManInPar.u32MinCycleTime = sSyncManOutPar.u32MinCycleTime;
    /* subindex 6: delay read inputs, calculation and copy to SM buffer*/
    sSyncManInPar.u32CalcAndCopyTime = (PD_INPUT_CALC_AND_COPY_TIME);
    /*subindex 8: trigger cycle time measurement*/
    sSyncManInPar.u16GetCycleTime = 0;
    /*subindex 9: delay to prepare input latch*/
    sSyncManInPar.u32DelayTime = (PD_INPUT_DELAY_TIME);

    /*subindex 32: incremented if a synchronisation error has occurred*/
    sSyncManInPar.u16SyncError = 0;

    {
    UINT16 result = COE_ObjDictionaryInit();
    if(result != 0)
    {
        /*clear already linked objects*/
        COE_ClearObjDictionary();
    }
    }

/* ECATCHANGE_START(V5.01) SDO6*/
    u8PendingSdo = 0;
    bStoreCompleteAccess = FALSE;
    u16StoreIndex   =   0;
    u8StoreSubindex = 0;
    u32StoreDataSize = 0;
    pStoreData = NULL;
    pSdoPendFunc    = NULL;
/* ECATCHANGE_END(V5.01) SDO6*/

    pSdoSegData = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    0               object successful added to object dictionary
            ALSTATUSCODE_XX add object failed

 \brief    This function adds an object to the object dictionary
 *////////////////////////////////////////////////////////////////////////////////////////
UINT16 Ethercat::COE_AddObjectToDic(TOBJECT OBJMEM * pNewObjEntry)
{
    if(pNewObjEntry != NULL)
    {
        if(ObjDicList == NULL)
        {
            /* Object dictionary is empty */
            ObjDicList = pNewObjEntry;
            ObjDicList->pNext = NULL;
            ObjDicList->pPrev = NULL;
            return 0;
        }
        else if(ObjDicList->Index > pNewObjEntry->Index)
        {
            /*insert new object dictionary head*/
            pNewObjEntry->pPrev = NULL;
            pNewObjEntry->pNext = ObjDicList;
            ObjDicList->pPrev = pNewObjEntry;
            ObjDicList = pNewObjEntry;
            return 0;
        }
        else
        {
            TOBJECT    OBJMEM * pDicEntry = ObjDicList;
            while(pDicEntry != NULL)
            {
                if(pDicEntry->Index == pNewObjEntry->Index)
                {
                    /*object already exists in object dictionary*/
                    return ALSTATUSCODE_UNSPECIFIEDERROR;
                }
                else if(pDicEntry->Index > pNewObjEntry->Index)
                {
                    pNewObjEntry->pPrev = pDicEntry->pPrev;
                    pNewObjEntry->pNext = pDicEntry;

                    if(pDicEntry->pPrev != NULL)
                        pDicEntry->pPrev->pNext = pNewObjEntry;

                    pDicEntry->pPrev = pNewObjEntry;

                    return 0;
                }
                else if(pDicEntry->pNext == NULL)
                {
                    /*Last entry reached => add object to list tail*/
                    pDicEntry->pNext = pNewObjEntry;
                    pNewObjEntry->pPrev = pDicEntry;
                    pNewObjEntry->pNext = NULL;
                    return 0;
                }
                pDicEntry = pDicEntry->pNext;
            }
        }
    }
    return ALSTATUSCODE_UNSPECIFIEDERROR;
}
/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function adds an object to the object dictionary
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::COE_RemoveDicEntry(UINT16 index)
{
    TOBJECT    OBJMEM * pDicEntry = ObjDicList;

    while(pDicEntry != NULL)
    {
        if(pDicEntry->Index == index)
        {
            TOBJECT OBJMEM *pPrevEntry = pDicEntry->pPrev;
            TOBJECT OBJMEM *pNextEntry = pDicEntry->pNext;

            if(pPrevEntry != NULL)
                pPrevEntry->pNext = pNextEntry;

            if(pNextEntry != NULL)
                pNextEntry->pPrev = pPrevEntry;

            pDicEntry->pPrev = NULL;
            pDicEntry->pNext = NULL;
            /*Update Object dictionary pointer if list head was removed*/
            if(pDicEntry->Index == ObjDicList->Index)
            {
                ObjDicList = pNextEntry;
            }
            return;
        }

        pDicEntry = pDicEntry->pNext;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function clear the object dictionary
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::COE_ClearObjDictionary(void)
{
    TOBJECT OBJMEM * pObjEntry = (TOBJECT OBJMEM *) ObjDicList;
    UINT16 Index = 0;

    while(pObjEntry != NULL)
    {
        Index = pObjEntry->Index;
        pObjEntry = pObjEntry->pNext;

        COE_RemoveDicEntry(Index);
    }
    ObjDicList = NULL;
}


UINT16 Ethercat::AddObjectsToObjDictionary(TOBJECT OBJMEM * pObjEntry)
{
    UINT16 result = 0;
    TOBJECT OBJMEM * pEntry = (TOBJECT OBJMEM *)pObjEntry;

    while(pEntry->Index != 0xFFFF)
    {
        result = COE_AddObjectToDic(pEntry);

        if(result != 0)
            return result;

        pEntry++;
    }

    return result;

}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    0               object dictionary created successful
            ALSTATUSCODE_XX create object dictionary failed

 \brief    This function initialize the object dictionary
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 Ethercat::COE_ObjDictionaryInit(void)
{
    UINT16 result = 0;

    /*Reset object dictionary pointer*/
    ObjDicList = NULL;

    result = AddObjectsToObjDictionary((TOBJECT OBJMEM *) GenObjDic);

    if(result != 0)
        return result;
    if(ApplicationObjDic != NULL)
    {
        result = AddObjectsToObjDictionary((TOBJECT OBJMEM *) ApplicationObjDic);
    }

    return result;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    is called for background calculations which should not influence the
             ECAT_Application in synchronous modes
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::COE_Main(void)
{
/* ECATCHANGE_START(V5.01) SDO6*/
     UINT8 abort = 0;
    if(pSdoPendFunc != NULL)
    {
        abort = pSdoPendFunc(u16StoreIndex,u8StoreSubindex,u32StoreDataSize,pStoreData,bStoreCompleteAccess);

        if(abort != ABORTIDX_WORKING)
        {
            switch(u8PendingSdo)
            {
                case SDO_PENDING_SEG_WRITE:
                    if(pSdoSegData)
                    {
                        /* the allocated buffer can be released */
                        FREEMEM( (UINT16 VARMEM *) pSdoSegData );
                        pSdoSegData = NULL;
                    }
                case SDO_PENDING_WRITE:
                     /*send SDO Download Response*/
                    SDOS_SdoRes(abort, 0, NULL);
                break;

                case SDO_PENDING_SEG_READ:
                case SDO_PENDING_READ:
                    /* send SDO upload response */
                      SDOS_SdoRes(abort, u32StoreDataSize, pStoreData);
                    break;

            }

            u8PendingSdo = 0;
            u16StoreIndex = 0;
            u8StoreSubindex = 0;
            u32StoreDataSize = 0;
            pStoreData = NULL;
            bStoreCompleteAccess = 0;
            pSdoPendFunc = NULL;
        }
    }
/* ECATCHANGE_END(V5.01) SDO6*/
}
/** @} */



