/**
\defgroup sdoserv sdoserv.c: CoE (CAN application layer over EtherCAT) - SDO Server
\brief The SDO server handles all sdo and sdo information services\n
\brief Changes to version V5.0:
\brief V5.01 MBX1: Allocate always complete 16Bit memory areas
\brief V5.01 SDO2: Prevent invalid memory access in case of 8Bit mailbox memory handling
\brief V5.01 SDO3: Update mailbox data length calculation
\brief V5.01 SDO4: Update length calculation in case of an segmented SDO list response
\brief V5.01 SDO6: Update SDO response interface handling. (used if the object access function returns "ABORTIDX_WORKING" and SDO_RES_INTERFACE is active)
\brief V5.01 TEST2: Change Test 0x2020.1 (force Segmented transfer) to 16Byte
\brief Changes to version V4.40:
\brief V5.0 TEST1: Add test application. see Application Note ET9300 for more details.
\brief V5.0 MBX3: Calculate MBX datagram length independent of SM size.
\brief V5.0 SDO2: SDO toggle bit don not do be cleared for segmented communication.
\brief V5.0 SDO3: Set SDO OpCode in list response.
\brief V5.0 SDO4: Update abort code for "BIG_ENDIAN_16BIT" configuration.
\brief V5.0 SDO5: Object code was calculated wrong for SDO Info service.
\brief V5.0 SDO6: Handle SDO segmented transfer if only 16Bit MBX memory access is allowed. 
\brief Changes to version V4.20:\n
\brief V4.40 SDO1: change size calculation for SDO services
\brief V4.40 MBX1: Prevent accessing odd address
\brief V4.40 COE1: Abort code is set in OBJ_GetObjectList()
\brief Changes to version V4.11:\n
\brief V4.20 PIC24: Add EL9800_4 (PIC24) required source code
\brief V4.20 SDO 2: SDO mask value bug
\brief V4.20 SDO 1: check zero size object length
\brief Changes to version V4.10:\n
\brief V4.11 SDO 1: fixed calculation of frame fragments during a object dictionary array list request\n
\brief V4.11 SDO 2-3: fixed size of entry description\n
\brief V4.11 SDO 4-7: add STRUCT_PACKED defines\n
\brief Changes to version V4.08:\a
\brief V4.10 SDO 1: fixed zero size SDO comparison\n
\brief V4.10 SDO 2: set SdoService_CommandOffset to 0\n
\brief V4.10 SDO 3: fixed zero size SDO comparison\n
\brief V4.10 SDO 4: fixed struct_packed definition\n
\brief Changes to version V4.07:\n
\brief V4.08 SDO 1: For an upload segment response the toggle bit was overwritten\n
\brief V4.08 SDO 2: For a segmented response the command was wrong in the response\n
\brief Changes to version V4.06:\n
\brief V4.07 SDO 1: In SdoRes the command specifier was not set correctly in case of an abort\n
\brief V4.07 ECAT 1: The sources for SPI and MCI were merged (in ecat_def.h
\brief                    set the switch MCI_HW to 1 when using the MCI,
\brief                    set the switch SPI_HW to 1 when using the SPI\n
\brief Changes to version V4.05:\n
\brief V4.06 SDO 1: The variable dataSize was used wrong in function SdoRes\n
\brief Changes to version V4.03:\n
\brief V4.04 SDO 1: The SDO interface was changed in that way that a SDO response
\brief              could be sent by the application to a later time. In that case
\brief                  the functions OBJ_Read and OBJ_Write shall return the value
\brief                  ABORTIDX_WORKING. To send the SDO response the new function SDOS_SdoRes
\brief                  has to be called by the application. While waiting for the call
\brief                  of SDOS_SdoRes the SDO interface will answer to another SDO request
\brief                  with the error MBXERR_SERVICEINWORK in the mailbox protocol \n
\brief Changes to version V3.20:\n
\brief V4.00 SDO 1: The size of the written data in case of a SDO Download will be
\brief              in the function OBJ_Write to be more flexible\n
\brief V4.00 SDO 2: The object lists will be generated in the functions OBJ_GetNoOfObjects
\brief              and OBJ_GetObjectList in objdef.c to decouple the SDO services from
\brief              the implementation of the object dictionary\n
\brief V4.00 SDO 3: The name of an object or entry description will only be transmitted
\brief              if it fits in the mailbox because the fragmentation is not supported in the sample code,\n\n
\brief V4.00 SDO 4: SDOs with size greater than 65535 were not handled correctly, that is fixed now\n


\version 5.01
*/

//---------------------------------------------------------------------------------------
/**
\ingroup sdoserv
\file    sdoserv.c
\brief Implementation.
*/
//---------------------------------------------------------------------------------------

/*---------------------------------------------------------------------------------------
------
------    Includes
------
---------------------------------------------------------------------------------------*/

#include "ecat_def.h"


#include "ethercat.h"


//extern OBJCONST TOBJECT OBJMEM asObjDef[];
//extern UINT16 OBJ_GetDesc( UINT16 index, UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData );
//extern OBJCONST TSDOINFOENTRYDESC OBJMEM * OBJ_GetEntryDesc(OBJCONST TOBJECT OBJMEM * pObjEntry, UINT8 Subindex);
//extern OBJCONST TSDOINFOOBJDESC OBJMEM * OBJ_GetObjDesc(OBJCONST TOBJECT OBJMEM * pObjEntry);
BOOL    VARMEM                            Ethercat::bSdoInWork = FALSE;
/*---------------------------------------------------------------------------------------
------
------    Modulintern variable definitions
------
---------------------------------------------------------------------------------------*/
/*
const UINT32 MBXMEM Ethercat::cAbortCode[] =
{
    ABORT_NOERROR,
    ABORT_TOGGLE_BIT_NOT_CHANGED,
    ABORT_SDO_PROTOCOL_TIMEOUT,
    ABORT_COMMAND_SPECIFIER_UNKNOWN,
    ABORT_OUT_OF_MEMORY,
    ABORT_UNSUPPORTED_ACCESS,
    ABORT_WRITE_ONLY_ENTRY,
    ABORT_READ_ONLY_ENTRY,
    ABORT_OBJECT_NOT_EXISTING,
    ABORT_OBJECT_CANT_BE_PDOMAPPED,
    ABORT_MAPPED_OBJECTS_EXCEED_PDO,
    ABORT_PARAM_IS_INCOMPATIBLE,
    ABORT_INTERNAL_DEVICE_INCOMPATIBILITY,
    ABORT_HARDWARE_ERROR,
    ABORT_PARAM_LENGTH_ERROR,
    ABORT_PARAM_LENGTH_TOO_LONG,
    ABORT_PARAM_LENGTH_TOO_SHORT,
    ABORT_SUBINDEX_NOT_EXISTING,
    ABORT_VALUE_EXCEEDED,
    ABORT_VALUE_TOO_GREAT,
    ABORT_VALUE_TOO_SMALL,
    ABORT_MAX_VALUE_IS_LESS_THAN_MIN_VALUE,
    ABORT_GENERAL_ERROR,
    ABORT_DATA_CANNOT_BE_READ_OR_STORED,
    ABORT_DATA_CANNOT_BE_READ_OR_STORED_BECAUSE_OF_LOCAL_CONTROL,
    ABORT_DATA_CANNOT_BE_READ_OR_STORED_IN_THIS_STATE,
    ABORT_NO_OBJECT_DICTIONARY_IS_PRESENT,
    ABORT_ENTRY_CANT_BE_WRITTEN_SI0_NOT_0
};
*/
/*
UINT16 VARMEM                            nSdoInfoIndex;
OBJCONST TOBJECT OBJMEM * VARMEM        pSdoInfoObjEntry;

TINITSDOMBX MBXMEM *    VARMEM            pSdoResStored;
BOOL    VARMEM                            bSdoInWork = FALSE;

UINT8    VARMEM                         nSdoSegService;
UINT8    VARMEM                         bSdoSegFollows;
UINT8    VARMEM                         bSdoSegAccess;
UINT16 VARMEM                           nSdoSegIndex;
UINT8 VARMEM                            nSdoSegSubindex;
UINT32 VARMEM                           nSdoSegBytesToHandle;
UINT8 VARMEM                            bSdoSegLastToggle;
UINT32 VARMEM                           nSdoSegCompleteSize;
OBJCONST TOBJECT OBJMEM * VARMEM        pSdoSegObjEntry;
*/
/*---------------------------------------------------------------------------------------
------
------    module internal function declarations
------
---------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------
------
------    Functions
------
---------------------------------------------------------------------------------------*/

/**
\addtogroup sdoserv
@{
*/

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pSdoInd    Pointer to the received mailbox data from the master.

 \return    Indicates if an error occurred while the operation ( good = 0 ).

 \brief    This function is called when a Download SDO Segment request
            service is received from the master. If its the last segment
            the data will be written to the object dictionary. The
            function sends a response by itself.
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 Ethercat::SdoDownloadSegmentInd( TDOWNLOADSDOSEGREQMBX MBXMEM * pSdoInd )
{
    UINT8 abort = 0;
    UINT32 bytesToSave = 0;

    if ( (pSdoInd->SdoHeader.SegHeader & SEGHEADER_TOGGLE) == bSdoSegLastToggle )
    {
        /* toggle bit has not toggled... */
        abort = ABORTIDX_TOGGLE_BIT_NOT_CHANGED;
    }
    else
    {
        /* maxData contains the maximum data to be received with a SDO-DownloadSegment */
        UINT16 maxData =    u16ReceiveMbxSize - MBX_HEADER_SIZE - SEGMENT_NORM_HEADER_SIZE;
        /* the new toggle bit is stored in bSdoSegLastToggle */
        bSdoSegLastToggle = pSdoInd->SdoHeader.SegHeader & SEGHEADER_TOGGLE;

        /* a SDO-Download Segment is only allowed if a SDO-Download Request was received before,
           in that case a buffer for the received data was allocated in SDOS_SdoInd before */
        if ( pSdoSegData )
        {
            /* bytesToSave contains the remaining data with this and maybe the following
               SDO-Download Segment services */
            bytesToSave = nSdoSegCompleteSize - nSdoSegBytesToHandle;

            if ( pSdoInd->SdoHeader.SegHeader & SEGHEADER_NOMOREFOLLOWS )
            {
                /* the last segment is received, check if the length of the remaining data is the
                   same as the length of the received data */
                if ( bytesToSave <= maxData )
                {
                    /* for the check it is distinguished if the remaining bytes are less than 8 (in that
                       case 7 data bytes were sent and the SDO-Download Segment header contains the information
                        how much bytes are valid (CAN-compatibility)), otherwise the length has to match exactly
                        and the SDO-Download Segment-Headerbyte is ignored */
                if ( ( ( bytesToSave <= (UINT32)(pSdoInd->MbxHeader.Length - SEGMENT_NORM_HEADER_SIZE) )
                         &&( bytesToSave == ((UINT16) (MIN_SEGMENTED_DATA - ((pSdoInd->SdoHeader.SegHeader & SEGHEADER_SEGDATASIZE) >> SEGHEADERSHIFT_SEGDATASIZE))) )
                          )
                        ||( ( bytesToSave > MIN_SEGMENTED_DATA )
                        &&( bytesToSave == (pSdoInd->MbxHeader.Length - SEGMENT_NORM_HEADER_SIZE) )
                        ) )
                    {
                        /* length is correct */
                        bSdoSegFollows = FALSE;
                    }
                    else
                        abort = ABORTIDX_PARAM_LENGTH_ERROR;
                }
                else
                    abort = ABORTIDX_PARAM_LENGTH_ERROR;
            }
            else
            {
                /* its not the last segment */
                bSdoSegFollows = TRUE;
                /* we have to check if we expect less bytes than the maximum size which can be send with a single
                   SDO Download Segment */
                if ( bytesToSave <= maxData )
                    abort = ABORTIDX_PARAM_LENGTH_ERROR;
                else
                    /* length is okay, bytesToSave contains the data size to be copied */
                    bytesToSave = maxData;
            }

            if ( abort == 0 )
            {
                /* the received data is copied in the buffer */
/*ECATCHANGE_START(V5.01) SDO2*/
                MBXMEMCPY( ((UINT8*)pSdoSegData) + (nSdoSegBytesToHandle), pSdoInd->SdoHeader.Data, bytesToSave ); 
/*ECATCHANGE_END(V5.01) SDO2*/

                if ( bSdoSegFollows == FALSE    )
                {
                    /* it was the last segment, OBJ_Write will called to make the Write-operation */
                    abort = OBJ_Write( nSdoSegIndex, nSdoSegSubindex, nSdoSegCompleteSize, pSdoSegObjEntry, (UINT16 MBXMEM *) pSdoSegData, bSdoSegAccess );
                    if ( abort == ABORTIDX_WORKING )
                    {
/* ECATCHANGE_START(V5.01) SDO6*/
                        /* the application generates the SDO-Response later on by calling SDOS_SdoRes (only possible if object access function pointer is defined) */
                        u8PendingSdo = SDO_PENDING_SEG_WRITE;
                        bStoreCompleteAccess = bSdoSegAccess;
                        u8StoreSubindex = nSdoSegSubindex;
                        u16StoreIndex = nSdoSegIndex;
                        u32StoreDataSize = nSdoSegCompleteSize;
                        pStoreData = pSdoSegData;

                        pSdoPendFunc = pSdoSegObjEntry->Write;

                        bSdoInWork = TRUE;
                        pSdoResStored = (TINITSDOMBX MBXMEM *) pSdoInd;

                        bSdoSegFollows = FALSE;
                        nSdoSegService    = 0;
                        nSdoSegBytesToHandle = 0;

                        return ABORTIDX_WORKING;
/* ECATCHANGE_END(V5.01) SDO6*/
                    }
                    else
                    {
                        /* the allocated buffer can be released */
                        FREEMEM( (UINT16 VARMEM *) pSdoSegData );
                        pSdoSegData = NULL;
                    }
                }
            }
        }
        else
        {
            abort = ABORTIDX_COMMAND_SPECIFIER_UNKNOWN;
        }
    }

    if ( abort == 0)
    {
        /* send the SDO Download Segment response */
        pSdoInd->MbxHeader.Length = SEGMENT_NORM_RES_SIZE;
        pSdoInd->CoeHeader &= ~COEHEADER_COESERVICEMASK;
        pSdoInd->CoeHeader |= ((UINT16)COESERVICE_SDORESPONSE) << COEHEADER_COESERVICESHIFT;
        /* the SDO Download Segment header depends if it was the last segment or not */
        if ( bSdoSegLastToggle )
        {
           pSdoInd->SdoHeader.SegHeader        = SDOSERVICE_DOWNLOADSEGMENTRES|SEGHEADER_TOGGLE;
        }
        else
           pSdoInd->SdoHeader.SegHeader        = SDOSERVICE_DOWNLOADSEGMENTRES;

        if ( bSdoSegFollows == TRUE )
        {
            /* segments are still expected, nSdoSegBytesToHandle contains the number of received data bytes */
            nSdoSegBytesToHandle += bytesToSave;
        }
        else
        {
            /* the last segment was received, the variables are reset */
            nSdoSegBytesToHandle = 0;
            nSdoSegService    = 0;
        }
    }
    else 
    {
        /* the Abort-Response will be sent in SDOS_SdoInd*/
        bSdoSegFollows = FALSE;
        nSdoSegService    = 0;
        if (pSdoSegData)
        {
            /* the memory has to be released if it is not released before.
            In case of AbortIdx_Working the buffer will be freed in SDOS_SdoRes*/
            FREEMEM( (UINT16 VARMEM *) pSdoSegData );
            pSdoSegData = NULL;
        }

        nSdoSegBytesToHandle = 0;
    }

    return abort;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pSdoInd            Pointer to the received mailbox data from the master.

 \return    Indicates if an error occurred while the operation ( good = 0 ).

 \brief    This function is called when a Upload SDO Segment request service
            is received from the master. It prepares and operates the
            response and sends it by itself.
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 Ethercat::SdoUploadSegmentInd( TUPLOADSDOSEGREQMBX MBXMEM * pSdoInd )
{
    UINT8 abort = 0;
    TUPLOADSDOSEGRESMBX MBXMEM * pSdoSegRes = (TUPLOADSDOSEGRESMBX MBXMEM *)pSdoInd;

   if ( (pSdoInd->SegHeader & SEGHEADER_TOGGLE) == bSdoSegLastToggle )
    {
        /* toggle bit has not toggled... */
        abort = ABORTIDX_TOGGLE_BIT_NOT_CHANGED;
    }
    else
    {
        /* maxData contains the maximum data to be sent with a SDO-Upload Segment response */
         
        UINT32 size = 0;
        UINT16 maxData;
        {
            maxData =    u16SendMbxSize - MBX_HEADER_SIZE - SEGMENT_NORM_HEADER_SIZE;
        }

        /* the new toggle bit is stored in bSdoSegLastToggle */
        bSdoSegLastToggle = pSdoInd->SegHeader & SEGHEADER_TOGGLE;

        if ( nSdoSegCompleteSize < (nSdoSegBytesToHandle + maxData) )
        {
            /* the remaining data can be send with one SDO Upload Segment response,
               size contains the data to be copied */
            size = nSdoSegCompleteSize - nSdoSegBytesToHandle;
            bSdoSegFollows = FALSE;
        }
        else
        {
            /* more data will follow, size contains the data to be copied */
            size = maxData;
            bSdoSegFollows = TRUE;
        }

        /* copy the object data in the SDO Upload segment response */
/*ECATCHANGE_START(V5.01) SDO2*/
        MBXMEMCPY( pSdoSegRes->SdoHeader.Data, &(((UINT8*)pSdoSegData)[nSdoSegBytesToHandle]), size );
/*ECATCHANGE_START(V5.01) SDO2*/
        
        /* the SDO Upload Segment header depends if there is still data to be sent */
        pSdoSegRes->CoeHeader &= ~COEHEADER_COESERVICEMASK;
        pSdoSegRes->CoeHeader |= ((UINT16)COESERVICE_SDORESPONSE) << COEHEADER_COESERVICESHIFT;
        
        /*Clear SDO header*/
        pSdoSegRes->SdoHeader.SegHeader &= ~SEGHEADER_MASK;
        if (bSdoSegFollows)
            pSdoSegRes->SdoHeader.SegHeader        |= SDOSERVICE_UPLOADSEGMENTRES | bSdoSegLastToggle;
        else
            pSdoSegRes->SdoHeader.SegHeader        |= SDOSERVICE_UPLOADSEGMENTRES | bSdoSegLastToggle | SEGHEADER_NOMOREFOLLOWS;

        // operate CAN specific flag segDataSize:
        /* HBu 06.02.06: the sizes were wrong */
        if ( size < MIN_SEGMENTED_DATA )
        {
            // at least    MIN_SEGMENTED_DATA bytes have to be send:
            pSdoSegRes->MbxHeader.Length = SEGMENT_NORM_RES_SIZE;
            pSdoSegRes->SdoHeader.SegHeader    |= (MIN_SEGMENTED_DATA - size) << SEGHEADERSHIFT_SEGDATASIZE;
        }
        else
        {
            pSdoSegRes->MbxHeader.Length         = ((UINT16) size) + SEGMENT_NORM_HEADER_SIZE;
        }

        if ( bSdoSegFollows == TRUE )
            // updating the value of send bytes:
            nSdoSegBytesToHandle += size;
        else
        {
            FREEMEM( (UINT16 VARMEM *) pSdoSegData );
            pSdoSegData = NULL;
            nSdoSegBytesToHandle = 0;
            nSdoSegService    = 0;
        }
    }

    return abort;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    abort                 Result of the SDO access
 \param    command		         SDO command index 
 \param    completeAccess        Indicates if complete access was requested
 \param    dataSize              Size of the object data send with the response
 \param    objLength             Complete size of the object
 \param     pSdoRes                Pointer to the mailbox buffer

 \brief    This function is called when a SDO response shall be sent
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::SdoRes(UINT8 abort, UINT8 command, UINT8 completeAccess, UINT16 dataSize, UINT32 objLength, TINITSDOMBX MBXMEM *pSdoRes)
{
    /* for an upload segment response the toggle bit was overwritten */
    if ((command != SDOSERVICE_UPLOADSEGMENTREQ) && (command != SDOSERVICE_DOWNLOADSEGMENTREQ))
    {
        pSdoRes->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET] = 0;
    }
    if ( abort == 0 )
    {
        /* SDO-Download or SDO-Upload was successful, generate the SDO- and CoE-Header */
        pSdoRes->CoeHeader &= ~COEHEADER_COESERVICEMASK;
        pSdoRes->CoeHeader |= ((UINT16)COESERVICE_SDORESPONSE) << COEHEADER_COESERVICESHIFT;
        if ( command == SDOSERVICE_INITIATEUPLOADREQ )
        {
            // HBu 06.02.06: Complete Access Bit in the SDO-Upload-Response too */
            if ( (objLength <= 4) && (objLength > 0) )
            {
                /* Expedited Upload Response */
                pSdoRes->MbxHeader.Length             =         EXPEDITED_FRAME_SIZE;
                pSdoRes->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]    |=     SDOHEADER_SIZEINDICATOR     |
                                                                                        SDOHEADER_TRANSFERTYPE        |
                                                                                        completeAccess |
                                                                                        ((MAX_EXPEDITED_DATA - ((UINT8)objLength)) << SDOHEADERSHIFT_DATASETSIZE) |
                                                                                        SDOSERVICE_INITIATEUPLOADRES;
            }
            else
            {
                /* Normal or Segmented Upload Response */
/*ECATCHANGE_START(V5.01) SDO3*/
                if (((dataSize < MBX_HEADER_SIZE + UPLOAD_NORM_RES_SIZE)) || (objLength > (UINT32)(dataSize - MBX_HEADER_SIZE - UPLOAD_NORM_RES_SIZE)) )
/*ECATCHANGE_END(V5.01) SDO3*/
                {
                    pSdoRes->MbxHeader.Length         =         SWAPWORD(UPLOAD_NORM_RES_SIZE+dataSize);
                }
                else
                {
                    pSdoRes->MbxHeader.Length         =         SWAPWORD(UPLOAD_NORM_RES_SIZE+((UINT16)objLength));
                }
                   ((TINITSDOUPLOADNORMRESMBX MBXMEM *) pSdoRes)->CompleteSize[0] = SWAPWORD((UINT16)objLength);
                   ((TINITSDOUPLOADNORMRESMBX MBXMEM *) pSdoRes)->CompleteSize[1] = SWAPWORD((UINT16)(objLength>>16));
                pSdoRes->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]    |=     SDOHEADER_SIZEINDICATOR     |
                                                                                        completeAccess |
                                                                                        SDOSERVICE_INITIATEUPLOADRES;
            }
        }
        /* for a segmented response the command was wrong in the response */
        else if ( command == SDOSERVICE_DOWNLOADSEGMENTREQ )
        {
            /* Download segmented response */
            pSdoRes->MbxHeader.Length         = DOWNLOAD_NORM_RES_SIZE;
            pSdoRes->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]    |= SDOSERVICE_DOWNLOADSEGMENTRES;
      }
        else if ( command != SDOSERVICE_UPLOADSEGMENTREQ )
        {
            /* Download response */
            pSdoRes->MbxHeader.Length         = DOWNLOAD_NORM_RES_SIZE;
            pSdoRes->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]    |= SDOSERVICE_INITIATEDOWNLOADRES;
        }
    }
    else
    {
        /* generate a SDO-Abort-Request */
        pSdoRes->MbxHeader.Length         = ABORT_NORM_RES_SIZE;
        pSdoRes->CoeHeader &= ~COEHEADER_COESERVICEMASK;
        pSdoRes->CoeHeader |= ((UINT16)COESERVICE_SDOREQUEST) << COEHEADER_COESERVICESHIFT;
        pSdoRes->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]    = SDOSERVICE_ABORTTRANSFER;
        ((TABORTSDOTRANSFERREQMBX MBXMEM *) pSdoRes)->AbortCode = SWAPDWORD(cAbortCode[abort]);
    }

    // HBu 02.05.06: if the CoE-response could not be sent because the
    //               send mailbox is full it should be stored
    if (MBX_MailboxSendReq((TMBX MBXMEM *) pSdoRes, COE_SERVICE) != 0)
    {
        /* we store the CoE mailbox service to send it later (in COE_ContinueInd) when the mailbox is read */
        pCoeSendStored = (TMBX MBXMEM *) pSdoRes;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pSdoInd    Pointer to the received mailbox data from the master.

 \return    Indicates if an error occurred while the operation ( good = 0 ).

 \brief    This function is called when a SDO request service
            is received from the master and calls depending from
            the command the concerning function.
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 Ethercat::SDOS_SdoInd(TINITSDOMBX MBXMEM *pSdoInd)
{
    UINT8 abort = 0;
    UINT8 sdoHeader = pSdoInd->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET] & SDOHEADER_COMMANDMASK;
    /* the SDO-command is in bit 5-7 of the first SDO-Byte */
    UINT8 command = (sdoHeader & SDOHEADER_COMMAND);
    /* mbxSize contains the size of the mailbox (CoE-Header (2 Bytes) + SDO-Header (8 Bytes) + SDO-Data (if the data length is greater than 4)) */
    UINT16 mbxSize = pSdoInd->MbxHeader.Length;
    UINT16 index;
    UINT8 subindex;
    OBJCONST TOBJECT OBJMEM * pObjEntry;
    /* this variable contains the information, if all entries of an object will be read (bCompleteAccess > 0) or a single entry */
    UINT8 bCompleteAccess = 0;
    UINT32 objLength = 0;
    UINT32 dataSize = 0;

    if ( bSdoInWork )
    {
        /* the last SDO is still in work */
        return MBXERR_SERVICEINWORK;
    }

    switch (command)
    {
    case SDOSERVICE_INITIATEDOWNLOADREQ:
    case SDOSERVICE_INITIATEUPLOADREQ:
        /* the variable index contains the requested index of the SDO service */
        index = pSdoInd->SdoHeader.Sdo[SDOHEADER_INDEXHIOFFSET] & SDOHEADER_INDEXHIMASK;
        index <<= 8;
        index += pSdoInd->SdoHeader.Sdo[SDOHEADER_INDEXLOOFFSET] >> SDOHEADER_INDEXLOSHIFT;
        /* the variable subindex contains the requested subindex of the SDO service */
        subindex    = pSdoInd->SdoHeader.Sdo[SDOHEADER_SUBINDEXOFFSET] >> SDOHEADER_SUBINDEXSHIFT;

        /* OBJ_GetObjectHandle checks if the requested index is defined in the object dictionary */
        pObjEntry = OBJ_GetObjectHandle( index );

        if ( pObjEntry )
        {
            /* transferType contains the information if the SDO Download Request or the SDO Upload Response
               can be an expedited service (SDO data length <= 4, that means the data is stored in the
                SDO-Header completely */
            UINT8 bTransferType = 0;
            /* pData is the pointer to the received (SDO-Download) or sent (SDO-Upload) SDO data in the mailbox */
            UINT16 MBXMEM * pData = NULL;
            UINT8 segTransfer = 0;

            {
                dataSize = objLength = OBJ_GetObjectLength( index, subindex, pObjEntry, (UINT8) (sdoHeader & SDOHEADER_COMPLETEACCESS) );

                if ( command == SDOSERVICE_INITIATEUPLOADREQ )
                {
                    /* SDO Upload */
                    if ( mbxSize != EXPEDITED_FRAME_SIZE )
                        /* a SDO Upload request has always a fixed size (2 Byte CoE-Header plus 8 Byte SDO-Header) */
                        return MBXERR_INVALIDSIZE;
                    /* distinguish between expedited and normal upload response within the length of the response data */
                    if ( (objLength <= MAX_EXPEDITED_DATA) && objLength != 0 )
                    {
                        /* Expedited Upload */
                        bTransferType = 1;
                        /* pData is the pointer where the object data has to be copied for the response */
                        pData = ((TINITSDOUPLOADEXPRESMBX MBXMEM *) pSdoInd)->Data;
                        /* initialize the 4 data bytes of the SDO upload response because the requested object data
                           could be less than 4 */
                        pData[0] = 0;
                        pData[1] = 0;
                    }
                    else
                    {
                        /* HBu 06.02.06: the variable dataSize has to be set to the available size in one mailbox */
                    dataSize = u16SendMbxSize - MBX_HEADER_SIZE - UPLOAD_NORM_RES_SIZE;
                        if ( dataSize < objLength )
                            /* Segmented Upload */
                            segTransfer = 1;
                        else
                            /* Normal Upload */
                            pData = ((TINITSDOUPLOADNORMRESMBX MBXMEM *) pSdoInd)->Data;
                    }
                }
                else
                {
                    /* SDO-Download: store if the request is a expedited or normal request  */
                    bTransferType = sdoHeader & SDOHEADER_TRANSFERTYPE;
                }
            }

            if ( command == SDOSERVICE_INITIATEDOWNLOADREQ )
            {
                /* SDO Download */
                if ( bTransferType )
                {
                    /* Expedited Download */
                    if ( mbxSize != EXPEDITED_FRAME_SIZE )
                        /* an Expedited SDO Download request has always a fixed size (2 Byte CoE-Header plus 8 Byte SDO-Header) */
                        return MBXERR_INVALIDSIZE;
                    /* dataSize gets the real size of the downloaded object data (1,2,3 or 4) */
                    dataSize = MAX_EXPEDITED_DATA - ((sdoHeader & SDOHEADER_DATASETSIZE) >> SDOHEADERSHIFT_DATASETSIZE);
                    /* pData is the pointer to the downloaded object data */
                    pData = (UINT16 MBXMEM *) &pSdoInd[1];
                }
                else
                {
                    /* Normal Download */
                    /* downloadSize gets the real size of the downloaded data */
                    /* '&' operator was too much */

                    UINT32 downloadSize = ((UINT32)(SWAPWORD(((TINITSDODOWNLOADNORMREQMBX MBXMEM *) pSdoInd)->CompleteSize[1]))<<16) + (SWAPWORD(((TINITSDODOWNLOADNORMREQMBX MBXMEM *) pSdoInd)->CompleteSize[0]));

                    /* HBu 29.03.06: if it is a segmented download the mbxSize has to be the complete mailbox size */
                    if ( (MBX_HEADER_SIZE+EXPEDITED_FRAME_SIZE+downloadSize) > u16ReceiveMbxSize )
                    {
                        if ( mbxSize != (u16ReceiveMbxSize-MBX_HEADER_SIZE) )
                            return MBXERR_INVALIDSIZE;
                    }
                    else
                    {
                        if ( mbxSize != (EXPEDITED_FRAME_SIZE+downloadSize) )
                            /* the mbxSize and the downloadSize are not consistent (mbxSize = downloadSize + 2 byte CoE-Header + 8 byte SDO Header */
                            return MBXERR_INVALIDSIZE;
                    }

                    /* pData is the pointer to the downloaded object data */
                    pData = (UINT16 MBXMEM *) ((TINITSDODOWNLOADNORMREQMBX MBXMEM *) pSdoInd)->Data;
                    /* the received dataSize will be checked in the object specific functions called from
                       OBJ_Write (in objdef.c) */
                    dataSize = downloadSize;
                    if ( dataSize > (UINT32)(mbxSize - DOWNLOAD_NORM_REQ_SIZE) )
                        /* Segmented Download */
                        segTransfer = 1;
                }
            }

            if ( sdoHeader & SDOHEADER_COMPLETEACCESS )
            {
                bCompleteAccess = 1;
                // HBu 02.05.06: Complete Access is only supported with subindex 0 and 1
                if (subindex > 1)
                    abort = ABORTIDX_UNSUPPORTED_ACCESS;
            }

            if ( abort == 0 )
            {
                if ( segTransfer )
                {
                    bSdoSegFollows         = TRUE;
                    bSdoSegLastToggle     = 1;
                    bSdoSegAccess             = bCompleteAccess;
                    nSdoSegIndex             = index;
                    nSdoSegSubindex         = subindex;
                    pSdoSegObjEntry        = pObjEntry;
                    if ( command == SDOSERVICE_INITIATEUPLOADREQ )
                        nSdoSegCompleteSize    = objLength;
                    else
                        nSdoSegCompleteSize    = dataSize;

                    if (pSdoSegData != NULL)
                    {
                        FREEMEM( (UINT16 VARMEM *) pSdoSegData);
                        pSdoSegData = NULL;
                    }
/*ECATCHANGE_START(V5.01) MBX1*/
                    pSdoSegData = (UINT16 VARMEM *) ALLOCMEM( ROUNDUPBYTE2WORD(nSdoSegCompleteSize) );
/*ECATCHANGE_END(V5.01) MBX1*/

                    if ( pSdoSegData == NULL )
                        abort = ABORTIDX_OUT_OF_MEMORY;
                    else
                    {
                        if ( command == SDOSERVICE_INITIATEUPLOADREQ )
                        {
                            /* Segmented Upload */
                            abort = OBJ_Read( index, subindex, objLength, pObjEntry, (UINT16 MBXMEM *) pSdoSegData, bCompleteAccess );
                            if ( abort == 0 )
                            {
                                MBXMEMCPY( ((TINITSDOUPLOADNORMRESMBX MBXMEM *) pSdoInd)->Data, pSdoSegData, dataSize );
                                nSdoSegService    = SDOSERVICE_UPLOADSEGMENTREQ;
                            }
                            else if ( abort == ABORTIDX_WORKING )
                            {
/* ECATCHANGE_START(V5.01) SDO6*/
                                /* the application generates the SDO-Response later on by calling SDOS_SdoRes (only possible if object access function pointer is defined) */
                                u8PendingSdo = SDO_PENDING_SEG_READ;
                                bStoreCompleteAccess = bCompleteAccess;
                                u8StoreSubindex = subindex;
                                u16StoreIndex = index;
                                u32StoreDataSize = objLength;
                                pStoreData = pSdoSegData;
                                pSdoPendFunc = pObjEntry->Read;

                                bSdoInWork = TRUE;
                                /* we have to store the buffer and the response header */
                                pSdoResStored = pSdoInd;

                                /*update command field*/
                                pSdoResStored->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]   &= ~SDOHEADER_COMMANDMASK;
                                pSdoResStored->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]   |= (sdoHeader & (SDOHEADER_COMPLETEACCESS | SDOHEADER_COMMAND));
                                nSdoSegService    = SDOSERVICE_UPLOADSEGMENTREQ;
                                return 0;
/* ECATCHANGE_END(V5.01) SDO6*/
                            }
                        }
                        else
                        {
                            /* Segmented Download */
                            MBXMEMCPY( pSdoSegData, ((TINITSDODOWNLOADNORMREQMBX MBXMEM *) pSdoInd)->Data, mbxSize-DOWNLOAD_NORM_REQ_SIZE );
                            nSdoSegService    = SDOSERVICE_DOWNLOADSEGMENTREQ;
                            dataSize = (mbxSize-DOWNLOAD_NORM_REQ_SIZE);
                        }

                        nSdoSegBytesToHandle = dataSize;
                    }
                }
                else
                {
                    if ( objLength == 0 )
                    {
                        /* the objLength is not known, therefore the variables for a possible segmented transfer
                            should be initialized */
                        nSdoSegIndex             = index;
                        nSdoSegSubindex         = subindex;
                        pSdoSegObjEntry        = pObjEntry;
                    }
                    if ( command == SDOSERVICE_INITIATEUPLOADREQ )
                    {
                        /* Expedited or Normal Upload */
                        abort = OBJ_Read( index, subindex, objLength, pObjEntry, pData, bCompleteAccess );
                        if ( abort == ABORTIDX_WORKING )
                        {
/* ECATCHANGE_START(V5.01) SDO6*/
                            /* the application generates the SDO-Response later on by calling SDOS_SdoRes (only possible if object access function pointer is defined) */
                            u8PendingSdo = SDO_PENDING_READ;
                            bStoreCompleteAccess = bCompleteAccess;
                            u8StoreSubindex = subindex;
                            u16StoreIndex = index;
                            u32StoreDataSize = objLength;
                            pStoreData = pData;
                            pSdoPendFunc = pObjEntry->Read;

                            bSdoInWork = TRUE;
                            /* we have to store the buffer and the response header */
                            pSdoResStored = pSdoInd;
                            
                            /*update command field*/
                            pSdoResStored->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET] &= ~SDOHEADER_COMMANDMASK;
                            pSdoResStored->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]    |= (sdoHeader & (SDOHEADER_COMPLETEACCESS | SDOHEADER_COMMAND));
                            return 0;
/* ECATCHANGE_END(V5.01) SDO6*/
                        }
                    }
                    else
                    {
                        /* Expedited or Normal Download */
                        abort = OBJ_Write( index, subindex, dataSize, pObjEntry, pData, bCompleteAccess );
                        if ( abort == ABORTIDX_WORKING )
                        {
/* ECATCHANGE_START(V5.01) SDO6*/
                            /* the application generates the SDO-Response later on by calling SDOS_SdoRes (only possible if object access function pointer is defined) */
                            u8PendingSdo = SDO_PENDING_WRITE;
                            bStoreCompleteAccess = bCompleteAccess;
                            u8StoreSubindex = subindex;
                            u16StoreIndex = index;
                            u32StoreDataSize = dataSize;
                            pStoreData = pData;
                            pSdoPendFunc = pObjEntry->Write;

                            bSdoInWork = TRUE;
                            /* we have to store the buffer and the response header */
                            pSdoResStored = pSdoInd;

                            /*update command field*/
                            pSdoResStored->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET] &= ~SDOHEADER_COMMANDMASK;
                            pSdoResStored->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET]    |= (sdoHeader & (SDOHEADER_COMPLETEACCESS | SDOHEADER_COMMAND));
                            return 0;
/* ECATCHANGE_END(V5.01) SDO6*/
                        }
                    }
                } /* else if ( objLength == 0 ) */
            } /* if ( abort == 0 ) */

        }
        else
        {
            abort = ABORTIDX_OBJECT_NOT_EXISTING;
        }
        break;

    case SDOSERVICE_DOWNLOADSEGMENTREQ:
    case SDOSERVICE_UPLOADSEGMENTREQ:
        if ( command == nSdoSegService )
        {
            if ( command == SDOSERVICE_DOWNLOADSEGMENTREQ )
                abort = SdoDownloadSegmentInd( (TDOWNLOADSDOSEGREQMBX MBXMEM *) pSdoInd );
            else
                abort = SdoUploadSegmentInd( (TUPLOADSDOSEGREQMBX MBXMEM *) pSdoInd );
        }
        else
            abort = ABORTIDX_COMMAND_SPECIFIER_UNKNOWN;
        break;

    default:
        abort = ABORTIDX_COMMAND_SPECIFIER_UNKNOWN;
        break;
    }

/* ECATCHANGE_START(V5.01) SDO6*/
    if(abort != ABORTIDX_WORKING)
/* ECATCHANGE_END(V5.01) SDO6*/
    {
        /*  type cast was added because of warning */
        SdoRes(abort, command, (UINT8) (sdoHeader & SDOHEADER_COMPLETEACCESS), (UINT16) dataSize, objLength, pSdoInd);
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    abort                Result of the SDO access
 \param    objLength            Complete size of the object
 \param    pData                Pointer to the mailbox buffer

 \brief    This function is called when a SDO response shall be sent
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::SDOS_SdoRes(UINT8 abort, UINT32 objLength, UINT16 MBXMEM *pData)
{
    UINT16 dataSize = 0;

    if (bSdoInWork)
    {
        /* SDO-Response is expected */
        UINT8 command = pSdoResStored->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET] & SDOHEADER_COMMAND;
        UINT8 completeAccess = pSdoResStored->SdoHeader.Sdo[SDOHEADER_COMMANDOFFSET] & SDOHEADER_COMPLETEACCESS;

        if ( command == SDOSERVICE_INITIATEUPLOADREQ )
        {
            /* dataSize contains the available size in one mailbox */
            dataSize = u16SendMbxSize - MBX_HEADER_SIZE - UPLOAD_NORM_RES_SIZE;
            if ( dataSize < objLength )
            {
                /* Segmented Upload, the variables for the segmented transfer should be initialized */
                bSdoSegFollows         = TRUE;
                bSdoSegLastToggle     = 1;
                bSdoSegAccess             = completeAccess;
                nSdoSegCompleteSize    = objLength;
                nSdoSegService            = SDOSERVICE_UPLOADSEGMENTREQ;
                pSdoSegData                = (UINT16 VARMEM *) pData;
                /* the first segment shall be copied */
                MBXMEMCPY(((TINITSDOUPLOADNORMRESMBX MBXMEM *) pSdoResStored)->Data, pData, dataSize);
/* ECATCHANGE_START(V5.01) SDO6*/
                nSdoSegBytesToHandle = dataSize;
/* ECATCHANGE_END(V5.01) SDO6*/
            }
            else
            if ( (objLength <= 4) && (objLength > 0) )
            {
                /* Expedited response */
                if ( pData != ((TINITSDOUPLOADEXPRESMBX MBXMEM *) pSdoResStored)->Data )
                {
                    /* the data is not in the response buffer yet, it shall be copied */
                    MBXMEMCPY(((TINITSDOUPLOADEXPRESMBX MBXMEM *) pSdoResStored)->Data, pData, objLength);
                }
            }
            else
            {
                /* Normal response */
                if ( pData != ((TINITSDOUPLOADNORMRESMBX MBXMEM *) pSdoResStored)->Data )
                {
                    /* the data is not in the response buffer yet, it shall be copied */
                    MBXMEMCPY(((TINITSDOUPLOADNORMRESMBX MBXMEM *) pSdoResStored)->Data, pData, objLength);
                }
            }
        }

        /* SDO access is finished, send the response */
        bSdoInWork = FALSE;
        SdoRes(abort, command, completeAccess, dataSize, objLength, pSdoResStored);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pSdoInfoInd      Pointer to the received mailbox data from the master.

 \return    Indicates if an error occurred while the operation ( good = 0 ).

 \brief    This function is called when a SDO-Info request service
            is received from the master and calls depending from
            the opcode the concerning function.
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 Ethercat::SDOS_SdoInfoInd( TSDOINFORMATION MBXMEM *pSdoInfoInd )
{
    UINT8 abort = 0;
    /* the variable opCode contains the requested SDO Information type */
    UINT8 opCode = (UINT8) ((pSdoInfoInd->SdoHeader.InfoHead & INFOHEAD_OPCODE_MASK) >> INFOHEAD_OPCODE_SHIFT);
    OBJCONST TOBJECT OBJMEM * pObjEntry;
    UINT16 index;
    UINT8 flags = COE_SERVICE;

    /* it has to be checked if the mailbox protocol is correct, the sent mailbox data length has to
       great enough for the service header of the requested SDO Information type */
    if ( opCode == SDOINFOSERVICE_ENTRYDESCRIPTION_Q )
    {
        if ( pSdoInfoInd->MbxHeader.Length < SIZEOF_SDOINFOENTRYREQSTRUCT )
            return MBXERR_SIZETOOSHORT;
    }
    else
    {
        if ( pSdoInfoInd->MbxHeader.Length < SIZEOF_SDOINFOLISTSTRUCT )
            return MBXERR_SIZETOOSHORT;
    }

    switch ( opCode )
    {
    case SDOINFOSERVICE_OBJDICTIONARYLIST_Q:
        /* an object list is requested, check if the list type is supported */
        if ( SWAPWORD(pSdoInfoInd->SdoHeader.Data.List.ListType) <= INFO_LIST_TYPE_MAX )
        {
            UINT16 size = 0;
            /* the variable listType contains the requested listType */
            UINT8 listType = (UINT8) SWAPWORD(pSdoInfoInd->SdoHeader.Data.List.ListType);

            /* the SDO Information Header has to be stored because this function will be
               called again if the response could not be sent with one mailbox service, the
               variable nSdoInfoFragmentsLeft is 0 zero for the first call and unequal 0
               for the following calls */
            MBXMEMCPY(aSdoInfoHeader, pSdoInfoInd, SDO_INFO_HEADER_BYTE_SIZE);
            if ( listType-- == 0 )
            {
                /* List-Type 0: length of the lists */
                UINT8 i;

                /* the needed mailbox size for List-Type 0 response is just 24 bytes, the mailbox has always
                   to be at least 24 bytes to support the SDO Information service */
                nSdoInfoFragmentsLeft = 0;
                for (i = 0; i < INFO_LIST_TYPE_MAX; i++)
                {
                    UINT16 n = OBJ_GetNoOfObjects(i);

                    /* copy the number of objects of the list type in the SDO Information response */
                    ((UINT16 MBXMEM *) &pSdoInfoInd->CoeHeader)[(SIZEOF_SDOINFOLISTSTRUCT>>1)+i] = SWAPWORD(n);
                }

                /* size of the mailbox service response */
                size = (INFO_LIST_TYPE_MAX << 1) + SIZEOF_SDOINFOLISTSTRUCT;
            }
            else
            {
                /* object list with indexes is requested */
                UINT16 MBXMEM * pData;
                UINT16 n = 0;

                if ( nSdoInfoFragmentsLeft )
                {
                    /* the next fragment of the SDO Information response shall be sent */
                    /* initialize size with the maximum size fits into one mailbox service */
                    {
                        size = u16SendMbxSize - SIZEOF_SDOINFO - MBX_HEADER_SIZE;
                    }
                    /* initialize pData with the pointer where the fragment has to be copied */
                    pData = &((UINT16 MBXMEM *) &pSdoInfoInd->CoeHeader)[SIZEOF_SDOINFO>>1];
                    /* initialize index with the next index to be sent */
                    index = nSdoInfoIndex;
                    /* decrement the number of fragments to be sent */
                    nSdoInfoFragmentsLeft--;
                }
                else
                {
                    /* the first fragment of the SDO Information response has to be sent */
                    /* get the number of objects of the requested object list */
                    n = OBJ_GetNoOfObjects(listType);
                    /* we start with index 0x1000 */
                    index = 0x1000;
                    /* initialize size with the maximum size fits into one mailbox service */
                    {
                        size = u16SendMbxSize - SIZEOF_SDOINFOLISTSTRUCT - MBX_HEADER_SIZE;
                    }
                    /* initialize pData with the pointer where the fragment has to be copied */
                    pData = &((UINT16 MBXMEM *) &pSdoInfoInd->CoeHeader)[SIZEOF_SDOINFOLISTSTRUCT>>1];
/*ECATCHANGE_START(V5.01) SDO4*/
                    /*Check if List need to be send in fragments*/
                    if( (n<<1) > size)
                    {
                        /*number of Bytes to transmit don't fit into one mailbox datagram*/

                        /*calculate number of fragments which need to be send
                        total number of bytes - bytes which will be transmitted with the current response divided by the size of the following fragments 
                        */
                        nSdoInfoFragmentsLeft = (((n<<1)-size) /(size + 2));
                    
                    }
/*ECATCHANGE_END(V5.01) SDO4*/
                    else
                    {
                        nSdoInfoFragmentsLeft = 0;
                    }
                }

                /* get the next part of the requested object list */
                size = OBJ_GetObjectList(listType, &index, size, pData,&abort);

                /* store index for next fragment */
                nSdoInfoIndex = index;
                /* size contains before the instruction the size still available in the mailbox buffer
                    and shall contain the size of the mailbox response data after the next instruction */
                {
                    size = u16SendMbxSize - size - MBX_HEADER_SIZE;
                }
            }

            /* size of the mailbox response data */
            pSdoInfoInd->MbxHeader.Length    = size;

            if(abort == 0)
            {
             pSdoInfoInd->SdoHeader.InfoHead &= ~INFOHEAD_OPCODE_MASK;
             pSdoInfoInd->SdoHeader.InfoHead |= (UINT16) (SDOINFOSERVICE_OBJDICTIONARYLIST_S << INFOHEAD_OPCODE_SHIFT);
                /* number of fragments still has to be sent */
                pSdoInfoInd->SdoHeader.FragmentsLeft             = SWAPWORD(nSdoInfoFragmentsLeft);

                if (nSdoInfoFragmentsLeft)
                {
                    /* there still are fragments to be sent,
                       the InComplete flag in the SDO Information response has to be sent */
                    pSdoInfoInd->SdoHeader.InfoHead &= ~ INFOHEADER_INCOMPLETE_MASK;
                    pSdoInfoInd->SdoHeader.InfoHead |= (UINT16) (SDOINFOSERVICE_INCOMPLETE << INFOHEAD_OPCODE_SHIFT);
                    /* the FRAGMENTS_FOLLOW flag has to be set for the function MBX_MailboxSendReq to
                       indicate the mailbox handler that still fragments has to be sent so that this
                        function shall be called again from COE_ContinueInd when the actual mailbox buffer
                        was sent */
                    flags = FRAGMENTS_FOLLOW | COE_SERVICE;
                }
            }
        }
        break;

    case SDOINFOSERVICE_OBJDESCRIPTION_Q:
    case SDOINFOSERVICE_ENTRYDESCRIPTION_Q:
        /* get the requested index */
        index = SWAPWORD(pSdoInfoInd->SdoHeader.Data.Obj.Index);
        /* get the object handle of the requested index */
        pObjEntry = OBJ_GetObjectHandle( index );

        if ( pObjEntry )
        {
            /* object exists */
            UINT16 size = 0;
            if ( opCode == SDOINFOSERVICE_OBJDESCRIPTION_Q )
            {
                /* object description is requested */
                OBJTOMBXMEMCPY(&pSdoInfoInd->SdoHeader.Data.Obj.Res, OBJ_GetObjDesc(pObjEntry), SDO_INFO_OBJ_DESC_SIZE);
                /* the mailbox should be big enough that the maximum object description
                   fits in the mailbox (the fragmentation is not done in the sample code),
                    so it will be checked only if the object description fits */
                size = OBJ_GetDesc(index, 0, pObjEntry, NULL) + SIZEOF_SDOINFOOBJSTRUCT;

                if ( size > (u16SendMbxSize - MBX_HEADER_SIZE) )
                {
                    /* size of the object description does not fit in the mailbox,
                       the object description will be sent without the name */
                    size = SIZEOF_SDOINFOOBJSTRUCT;
                }
                else
                {
                    /* object description fits in the mailbox, get the name of the object */
                    size = OBJ_GetDesc(index, 0, pObjEntry, ((UINT16 MBXMEM *) &(&pSdoInfoInd->SdoHeader.Data.Obj.Res)[1])) + SIZEOF_SDOINFOOBJSTRUCT;
                }
            }
            else
            {
                /* entry description is requested,
                   get the requested subindex */
                UINT8 subindex = (UINT8) ((pSdoInfoInd->SdoHeader.Data.Entry.Info & ENTRY_MASK_SUBINDEX) >> ENTRY_SUBINDEX_SHIFT);

                /* get the maximum subindex */
                UINT8 maxSubindex = (OBJ_GetObjDesc(pObjEntry)->ObjFlags & OBJFLAGS_MAXSUBINDEXMASK) >> OBJFLAGS_MAXSUBINDEXSHIFT;

                if ( subindex <= maxSubindex )
                {
                    UINT16 ObjectFlags;
                    /* requested subindex is not too great */
                    /* get the entry description of the requested entry */
                    OBJTOMBXMEMCPY(&pSdoInfoInd->SdoHeader.Data.Entry.Res, OBJ_GetEntryDesc(pObjEntry, subindex), SIZEOF(TSDOINFOENTRYDESC));
                    /* the transmission of the value info is not supported yet of the sample code */
                    pSdoInfoInd->SdoHeader.Data.Entry.Info &= ~ENTRY_MASK_VALUEINFO;
                    ObjectFlags = OBJ_GetObjDesc(pObjEntry)->ObjFlags;
                    ObjectFlags = (ObjectFlags & OBJFLAGS_OBJCODEMASK) >> OBJFLAGS_OBJCODESHIFT;

                    if ( (ObjectFlags & (OBJCODE_ARR | OBJCODE_REC)) && (subindex == 0) )
                    {
                        OBJTOMBXSTRCPY( ((UINT16 MBXMEM *) &(&pSdoInfoInd->SdoHeader.Data.Entry.Res)[1]), aSubindexDesc, SIZEOF(aSubindexDesc) );
                            size = 12 + SIZEOF_SDOINFO + SIZEOF(TSDOINFOENTRY); // 12: Length of "SubIndex 000"
                    }
                    else
                    {
                        /* the mailbox should be big enough that the maximum entry description
                           fits in the mailbox (the fragmentation is not done in the sample code),
                            so it will be checked only if the entry description fits */
                        size = OBJ_GetDesc(index, subindex, pObjEntry, NULL) + SIZEOF_SDOINFO + SIZEOF(TSDOINFOENTRY);
                        if ( size > (u16SendMbxSize - MBX_HEADER_SIZE) )
                        {
                            /* size of the object description does not fit in the mailbox,
                               the object description will be sent without the name */
                            size =  SIZEOF_SDOINFO + SIZEOF(TSDOINFOENTRY);
                        }
                        else
                        {
                            /* object description fits in the mailbox, get the name of the entry */
                            size = OBJ_GetDesc(index, subindex, pObjEntry, ((UINT16 MBXMEM *) &(&pSdoInfoInd->SdoHeader.Data.Entry.Res)[1])) + SIZEOF_SDOINFO + SIZEOF(TSDOINFOENTRY);
                        }
                    }
                }
                else
                    abort = ABORTIDX_SUBINDEX_NOT_EXISTING;
            }

            if ( abort == 0 )
            {
                {
                    /* for the object and entry description the sample code does not support the fragmentation,
                       the mailbox has to be big enough */
                    pSdoInfoInd->SdoHeader.FragmentsLeft = 0;
                    /* store the size of the mailbox data in the mailbox buffer */
                    pSdoInfoInd->MbxHeader.Length = size;
                    /* set the opCode of the SDO Information response */
                    pSdoInfoInd->SdoHeader.InfoHead &= ~INFOHEAD_OPCODE_MASK;
                    pSdoInfoInd->SdoHeader.InfoHead |= (UINT16) ((opCode+1) << INFOHEAD_OPCODE_SHIFT);
                }
            }
        }
        else
            abort = ABORTIDX_OBJECT_NOT_EXISTING;
        break;
    default:
        abort = ABORTIDX_COMMAND_SPECIFIER_UNKNOWN;
    }

    if ( abort )
    {
        /* send a SDO Information Error response */
        pSdoInfoInd->MbxHeader.Length = SIZEOF_SDOINFOERRORSTRUCT;

        pSdoInfoInd->SdoHeader.InfoHead &= ~INFOHEAD_OPCODE_MASK;
        pSdoInfoInd->SdoHeader.InfoHead |= (UINT16) ((SDOINFOSERVICE_ERROR_Q) << INFOHEAD_OPCODE_SHIFT);

        pSdoInfoInd->SdoHeader.FragmentsLeft = 0;
        pSdoInfoInd->SdoHeader.Data.Error.ErrorCode = SWAPDWORD(cAbortCode[abort]);

        nSdoInfoFragmentsLeft = 0;
    }

    if (MBX_MailboxSendReq((TMBX MBXMEM *) pSdoInfoInd, flags) != 0)
    {
        /* if the mailbox response could not be sent (or stored), the response will be
           stored in the variable pCoeSendStored and will be sent automatically
            from the mailbox handler (COE_ContinueInd) when the send mailbox will be read
            the next time from the master */
        pCoeSendStored = (TMBX MBXMEM *) pSdoInfoInd;
    }

    return 0;
}

/** @} */



