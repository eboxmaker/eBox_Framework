/**
\defgroup mailbox mailbox.c: Mailbox functions
\brief Changes to version V5.0:
\brief V5.01 ESC2: Add missed value swapping
\brief V5.01 MBX2: Return an error if length specified in mailbnox header is too large.
\brief Changes to version V4.40:
\brief V5.0 ECAT3: Global dummy variables used for dummy ESC operations.
\brief V5.0 ESC1: ESC 32Bit Access added.
\brief V5.0 MBX2: Prevent double swapping (MBX repeat service) of MBX length field.
\brief V5.0 MBX4: Unlock SM buffer based on SM state.
\brief Changes to version V4.11:\n
\brief V4.40 MBX8: Handle pending mailbox data even if mailbox queue is enabled
\brief V4.40 MBX7: Reset SyncManger 1 Repeat Acknowledge bit if local variable is reset
\brief V4.40 EMCY2: Send queued emergency messages
\brief V4.40 BOOT2: Support only FoE in Bootstrap state
\brief V4.40 MBX5: Prevent multiple free buffer
\brief V4.40 MBX6: change return value if no mailbox buffer is available
\brief Changes to version V4.08:\n
\brief V4.11 MBX 1: free some dynamic memory to avoid memory leaks in the mailbox queue\n
\brief Changes to version V4.07:\n
\brief V4.10 MBX 1: check Mailbox message buffer and print emergency messages
\brief                during state change Init -> PreOP\n
\brief Changes to version V4.07:\n
\brief V4.08 MBX 1: The mailbox queue was added (switch MAILBOX_QUEUE), if the
\brief              mailbox should be handled in the ESC-Interrupt Service Routine
\brief              the defines ENTER_MBX_CRITICAL and LEAVE_MBX_CRITICAL could be used
\brief              to disable the interrupts or change the priority\n
\brief V4.08 AOE 1:    AoE was added\n
\brief Changes to version V4.06:\n
\brief V4.07 ECAT 1: The sources for SPI and MCI were merged (in ecat_def.h
\brief                    set the switch MCI_HW to 1 when using the MCI,
\brief                    set the switch SPI_HW to 1 when using the SPI\n
\brief Changes to version V3.20:\n

\version 5.01
*/

//---------------------------------------------------------------------------------------
/**
\ingroup mailbox
\file    mailbox.c
\brief Implementation.
*/
//---------------------------------------------------------------------------------------

/*
\brief Description of the mailbox buffer handling (MAILBOX_QUEUE = 0):\n
\brief There are two mailbox buffer for sending and receiving mailbox services.\n
\brief Normal operation:\n
\brief When starting the mailbox handler psWriteMbx contains mailbox buffer 1,\n
\brief psReadMbx, psRepeatMbx and psStoreMbx are 0.\n
\brief In this state a repeat request would be ignored because there was no service sent yet.\n
\brief When the first mailbox service is sent (in MBX_CopyToSendMailbox), psWriteMbx gets mailbox buffer 2\n
\brief and psReadMbx gets the sent mailbox buffer 1, psRepeatMbx and psStoreMbx are still 0.\n
\brief When the first mailbox service was read from the master, the sent mailbox buffer 1 is stored\n
\brief in psRepeatMbx (in MBX_MailboxReadInd).\n
\brief After that psReadMbx gets always the actual sent mailbox buffer, psWriteMbx is set to 0 (another\n
\brief received mailbox service from the master will not be handled until the sent mailbox service was read\n
\brief and MBX_MailboxReadInd was called).\n
\brief When the mailbox service is read, psWriteMbx gets the Buffer of psRepeatMbx and psRepeatMbx gets the \n
\brief buffer of psReadMbx.\n
\brief Repeat Request from the master:\n
\brief When a Repeat from the master is requested (MBX_MailboxRepeatReq), there are three different possibilities:\n
\brief 1. no mailbox service was sent since the mailbox handler was started (psRepeatMbx = 0): nothing to do\n
\brief 2. the acknowledge of the last sent mailbox service was received (in MBX_MailboxReadInd) (bSendMbxIsFull = 0):\n
\brief    the last sent mailbox service (psRepeatMbx) will be sent again (in MBX_CopyToSendMailbox) and stored in psReadMbx,\n
\brief    psRepeatMbx will be set to 0\n
\brief 3. the acknowledge of the last sent mailbox service was not received (psReadMbx and psRepeatMbx contain different buffers,\n
\brief    psReadMbx is still in the mailbox (because MBX_MailboxReadInd is not called yet, bSendMbxIsFull = 1): \n
\brief    psReadMbx will be deleted in the mailbox (call of HW_DisableSyncManChannel and HW_EnableSyncManChannel) and\n
\brief    stored in psStoreMbx, psRepeatMbx will be sent again (in MBX_CopyToSendMailbox) and stored in psReadMbx,\n
\brief    psRepeatMbx will be set to 0.\n
\brief    When the repeated mailbox service was sent (call of MBX_MailboxReadInd), psReadMbx will be stored in psRepeatMbx\n
\brief    and psStoreMbx will be sent (in MBX_CopyToSendMailbox) and stored in psReadMbx, psStoreMbx will be set to 0.\n

\brief Description of the mailbox buffer handling (MAILBOX_QUEUE = 1):\n
\brief There are two mailbox buffer for sending and receiving mailbox services.\n
\brief Normal operation (psWriteMbx is only used for local storage):\n
\brief When starting the mailbox handler psReadMbx, psRepeatMbx and psStoreMbx are 0.\n
\brief In this state a repeat request would be ignored because there was no service sent yet.\n
\brief When a mailbox service is received from the master (in MBX_CheckAndCopyMailbox) a mailbox buffer\n
\brief will be get with APPL_AllocMailboxBuffer and the corresponding protocol service function will\n
\brief be called (in MBX_WriteMailboxInd). This buffer shall be used for the protocol service response.\n
\brief When the first mailbox service is sent (in MBX_CopyToSendMailbox), psReadMbx gets the sent mailbox buffer,\n
\brief psRepeatMbx and psStoreMbx are still 0.\n
\brief When the first mailbox service was read from the master, the sent mailbox buffer (psReadMbx) is stored\n
\brief in psRepeatMbx (in MBX_MailboxReadInd).\n
\brief After that psReadMbx gets always the actual sent mailbox buffer (in MBX_CopyToSendMailbox)\n
\brief When the mailbox service is read, psRepeatMbx is returned (with APPL_FreeMailboxBuffer) and psRepeatMbx gets the \n
\brief buffer of psReadMbx.\n
\brief Repeat Request from the master:\n
\brief When a Repeat from the master is requested (MBX_MailboxRepeatReq), there are three different possibilities:\n
\brief 1. no mailbox service was sent since the mailbox handler was started (psRepeatMbx = 0): nothing to do\n
\brief 2. the acknowledge of the last sent mailbox service was received (in MBX_MailboxReadInd) (bSendMbxIsFull = 0):\n
\brief    the last sent mailbox service (psRepeatMbx) will be sent again (in MBX_CopyToSendMailbox) and stored in psReadMbx,\n
\brief    psRepeatMbx will be set to 0\n
\brief 3. the acknowledge of the last sent mailbox service was not received (psReadMbx and psRepeatMbx contain different buffers,\n
\brief    psReadMbx is still in the mailbox (because MBX_MailboxReadInd is not called yet, bSendMbxIsFull = 1): \n
\brief    psReadMbx will be deleted in the mailbox (call of HW_DisableSyncManChannel and HW_EnableSyncManChannel) and\n
\brief    stored in psStoreMbx, psRepeatMbx will be sent again (in MBX_CopyToSendMailbox) and stored in psReadMbx,\n
\brief    psRepeatMbx will be set to 0.\n
\brief    When the repeated mailbox service was sent (call of MBX_MailboxReadInd), psReadMbx will be stored in psRepeatMbx\n
\brief    and psStoreMbx will be sent (in MBX_CopyToSendMailbox) and stored in psReadMbx, psStoreMbx will be set to 0.\n
*/

/*---------------------------------------------------------------------------------------
------
------    Includes
------
---------------------------------------------------------------------------------------*/

#include "ecat_def.h"

#include "ethercat.h"

/*--------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------
------
------    internal Variables
------
--------------------------------------------------------------------------------------*/
/*variables are declared in ecatslv.c*/
//extern VARVOLATILE UINT16    u16dummy;
/*--------------------------------------------------------------------------------------
------
------    internal functions
------
--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------
------
------    functions
------
---------------------------------------------------------------------------------------*/

/**
\addtogroup mailbox
@{
*/


///////////////////////////////////////////////////////////////////////////////////////////
//
//    PutInMbxQueue
//

UINT8 Ethercat::PutInMbxQueue(TMBX MBXMEM * pMbx, TMBXQUEUE MBXMEM * pQueue)
{
    UINT16 lastInQueue;
    ENTER_MBX_CRITICAL;


    lastInQueue = pQueue->lastInQueue+1;
    if (lastInQueue == pQueue->maxQueueSize)
    {
        // Umbruch der Queue
        lastInQueue = 0;
    }

    if (pQueue->firstInQueue == lastInQueue)
    {
        // Ueberlauf der Queue -> letztes Element wieder herausnehmen
        LEAVE_MBX_CRITICAL;
        return MBXERR_NOMOREMEMORY;
    }

    pQueue->queue[pQueue->lastInQueue] = pMbx;
    pQueue->lastInQueue = lastInQueue;

    LEAVE_MBX_CRITICAL;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//    GetOutOfMbxQueue
//

TMBX MBXMEM * Ethercat::GetOutOfMbxQueue(TMBXQUEUE MBXMEM * pQueue)
{
    TMBX MBXMEM * pMbx;
    ENTER_MBX_CRITICAL;

    if (pQueue->firstInQueue != pQueue->lastInQueue)
    {
        // Queue ist nicht leer
        UINT16 firstInQueue = pQueue->firstInQueue;
        pMbx = pQueue->queue[firstInQueue++];
        pQueue->firstInQueue = firstInQueue;
        if (pQueue->firstInQueue == pQueue->maxQueueSize)
        {
            // Umbruch der Queue
            pQueue->firstInQueue = 0;
        }
    }
    else
        pMbx = 0;


    LEAVE_MBX_CRITICAL;

    return pMbx;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function intialize the Mailbox Interface.
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::MBX_Init(void)
{
    u16ReceiveMbxSize = MIN_MBX_SIZE;
    u16SendMbxSize = MAX_MBX_SIZE;
    u16EscAddrReceiveMbx = MIN_MBX_WRITE_ADDRESS;
    u16EscAddrSendMbx = MIN_MBX_READ_ADDRESS;

    sMbxReceiveQueue.firstInQueue    = 0;
    sMbxReceiveQueue.lastInQueue     = 0;
    sMbxReceiveQueue.maxQueueSize = MAX_MBX_QUEUE_SIZE;
    sMbxSendQueue.firstInQueue        = 0;
    sMbxSendQueue.lastInQueue         = 0;
    sMbxSendQueue.maxQueueSize     = MAX_MBX_QUEUE_SIZE;
    psWriteMbx  = NULL;

    psRepeatMbx = NULL;
    psReadMbx    = NULL;
    psStoreMbx    = NULL;

    bMbxRepeatToggle    = FALSE;
    /*Reset Repeat acknowledge bit of SyncManager1 (0x80F bit 2)*/
    {
        UINT16 sm1Activate = 0;
        HW_EscReadWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
        sm1Activate &= SWAPWORD(~0x0200);
        HW_EscWriteWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
    }
    bMbxRunning = FALSE;
    bSendMbxIsFull = FALSE;
    bReceiveMbxIsLocked = FALSE;
    u8MailboxSendReqStored    = 0;
    u8MbxWriteCounter = 0;
    u8MbxReadCounter    = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function includes the state transition from INIT to
 \brief    PRE-OPERATIONAL in the EtherCAT Slave corresponding to
 \brief  local management service Start Mailbox Handler
 \brief  it is checked if the mailbox areas overlaps each other
 \brief  and the Sync Manager channels 0 and 1 are enabled
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 Ethercat::MBX_StartMailboxHandler(void)
{
    /* get address of the receive mailbox sync manager (SM0) */
    TSYNCMAN ESCMEM * pSyncMan = (TSYNCMAN ESCMEM *)HW_GetSyncMan(MAILBOX_WRITE);

    /* store size of the receive mailbox */
    u16ReceiveMbxSize     = pSyncMan->Length;
    /* store the address of the receive mailbox */
    u16EscAddrReceiveMbx = pSyncMan->PhysicalStartAddress;

    /* get address of the send mailbox sync manager (SM1) */
    pSyncMan =(TSYNCMAN ESCMEM *) HW_GetSyncMan(MAILBOX_READ);

    /* store the size of the send mailbox */
    u16SendMbxSize = pSyncMan->Length;
    /* store the address of the send mailbox */
    u16EscAddrSendMbx = pSyncMan->PhysicalStartAddress;

    // HBu 02.05.06: it should be checked if there are overlaps in the sync manager areas
    if ((u16EscAddrReceiveMbx+u16ReceiveMbxSize) > u16EscAddrSendMbx && (u16EscAddrReceiveMbx < (u16EscAddrSendMbx+u16SendMbxSize)))
    {
        return ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    }

    /* enable the receive mailbox sync manager channel */
    HW_EnableSyncManChannel(MAILBOX_WRITE);
    /* enable the send mailbox sync manager channel */
    HW_EnableSyncManChannel(MAILBOX_READ);

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function includes the state transition from
 \brief    PRE-OPERATIONAL to INIT in the EtherCAT Slave corresponding to
 \brief  local management service Stop Mailbox Handler
 \brief  the Sync Manager channels 0 and 1 are disabled
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::MBX_StopMailboxHandler(void)
{
    TMBX MBXMEM * pMbx;

    /* mailbox handler is stopped */
    bMbxRunning = FALSE;
    /* disable the receive mailbox sync manager channel */
    HW_DisableSyncManChannel(MAILBOX_WRITE);
    /* disable the send mailbox sync manager channel */
    HW_DisableSyncManChannel(MAILBOX_READ);
    /* initialize variables again */


    if (psRepeatMbx != NULL)
        APPL_FreeMailboxBuffer(psRepeatMbx);

    if (psStoreMbx != NULL && psStoreMbx != psRepeatMbx)
        APPL_FreeMailboxBuffer(psStoreMbx);
    if (psReadMbx != NULL && psReadMbx != psRepeatMbx && psReadMbx != psStoreMbx)
        APPL_FreeMailboxBuffer(psReadMbx);

    psRepeatMbx = NULL;
    psReadMbx = NULL;
    psStoreMbx = NULL;

    bMbxRepeatToggle    = FALSE;
    /*Reset Repeat acknowledge bit of SyncManager1 (0x080F bit 2)*/
    {
        UINT16 sm1Activate = 0;
        HW_EscReadWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
        sm1Activate &= SWAPWORD(~0x0200);
        HW_EscWriteWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
    }
    bSendMbxIsFull         = FALSE;
    bReceiveMbxIsLocked = FALSE;
    u8MailboxSendReqStored    = 0;
    u8MbxWriteCounter         = 0;
    u8MbxReadCounter        = 0;

    do
    {
        pMbx = GetOutOfMbxQueue(&sMbxReceiveQueue);
        if (pMbx)
        {
            APPL_FreeMailboxBuffer(pMbx);
            pMbx = NULL;
        }
    } while (pMbx != NULL);
    do
    {
        pMbx = GetOutOfMbxQueue(&sMbxSendQueue);
        if (pMbx)
        {
            APPL_FreeMailboxBuffer(pMbx);
            pMbx = NULL;
        }
    } while (pMbx != NULL);

}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pMbx        Pointer to the received Mailbox command from Master.

 \brief    The function checks the mailbox header for the requested service and calls the
 \brief    corresponding XXXX_ServiceInd-function
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 Ethercat::MailboxServiceInd(TMBX MBXMEM *pMbx)
{
    UINT8 result;

    switch ( (pMbx->MbxHeader.Flags[MBX_OFFS_TYPE] & MBX_MASK_TYPE) >> MBX_SHIFT_TYPE )
    {
    case MBX_TYPE_COE:
        /* CoE datagram received */
        result = COE_ServiceInd((TCOEMBX MBXMEM *) pMbx);
        break;

    default:

        result = MBXERR_UNSUPPORTEDPROTOCOL;
        break;
    }
	
	//cx leakage?
	if(pMbx)
		APPL_FreeMailboxBuffer(pMbx);

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pMbx        Pointer to the received Mailbox command from Master.

 \brief    This function is called when the Master has written the Receive-Mailbox.
 \brief    It will only be called if the send mailbox is empty, that a response for the
 \brief    mailbox service could be stored.
 \brief    The function checks the mailbox header for the requested service and calls the
 \brief    corresponding XXXX_ServiceInd-function
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::MBX_MailboxWriteInd(TMBX MBXMEM *pMbx)
{
    UINT8 result = 0;
    UINT8 mbxCounter = pMbx->MbxHeader.Flags[MBX_OFFS_COUNTER] >> MBX_SHIFT_COUNTER;
/* ECATCHANGE_START(V5.01) MBX2*/
    UINT16 MbxLen = SWAPWORD(pMbx->MbxHeader.Length);

    if(MbxLen > MBX_BUFFER_SIZE)
    {
        /* Mailbox error response: size specified in mailbox header too large*/
        pMbx->MbxHeader.Length     = 4;
        pMbx->MbxHeader.Flags[MBX_OFFS_TYPE]        &= ~MBX_MASK_TYPE;
        pMbx->Data[0]                        = SWAPWORD(MBXSERVICE_MBXERRORCMD);
        pMbx->Data[1]                        = SWAPWORD(MBXERR_INVALIDSIZE);
        MBX_MailboxSendReq(pMbx, 0);
    }
/* ECATCHANGE_END(V5.01) MBX2*/
    else
    /* if the mailbox datagram counter (Bit 4-6 of Byte 5 of the mailbox header) is unequal zero,
       the master supports the mailbox data link layer,
        in that case a repeated mailbox write request will be detected, if the counter is unequal zero
        and unchanged */
    if ( mbxCounter == 0 || mbxCounter != u8MbxWriteCounter )
    {
        /* new mailbox service received */
        /* mbxCounter = 0: old EtherCAT master */
        /* new MBX service received, store the new mailbox counter */
        u8MbxWriteCounter = mbxCounter;

        {
            /* check the protocol type and call the XXXX_ServiceInd-function */

            result = PutInMbxQueue(pMbx, &sMbxReceiveQueue);
        }

        if ( result != 0 )
        {
            /* Mailbox error response: type 0 (mailbox service protocol) */
            pMbx->MbxHeader.Length     = 4;
            pMbx->MbxHeader.Flags[MBX_OFFS_TYPE]        &= ~MBX_MASK_TYPE;
            pMbx->Data[0]                        = SWAPWORD(MBXSERVICE_MBXERRORCMD);
            pMbx->Data[1]                        = SWAPWORD(result);
            MBX_MailboxSendReq(pMbx, 0);
        }
    }
    // the mailbox buffer has to be freed here
    else
    {
        APPL_FreeMailboxBuffer(pMbx);
        pMbx = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief This function is called when the Master has read the Send-Mailbox.
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::MBX_MailboxReadInd(void)
{
    bSendMbxIsFull = FALSE;
    // HBu 02.05.06: the pointer psRepeatMbx is only free if there is no stored
    //               mailbox service from the last repeat
    if ( psRepeatMbx && psStoreMbx == NULL )
    {
    /* the last sent service is not stored for repeat any longer */
        APPL_FreeMailboxBuffer(psRepeatMbx);
        psRepeatMbx = NULL;
    }

    /* the actual sent service has to be stored for repeat */
    psRepeatMbx = psReadMbx;

      if ( psStoreMbx )
      {
        /* there was a buffer stored */
        MBX_CopyToSendMailbox(psStoreMbx);
        /* no more buffer to be stored any more */
        psStoreMbx = NULL;
      }
      else
    {
        TMBX MBXMEM *pMbx = GetOutOfMbxQueue(&sMbxSendQueue);
        if (pMbx)
        {
            MBX_CopyToSendMailbox(pMbx);
        }
    }

      if ( u8MailboxSendReqStored )
    {
        /* there are mailbox services stored to be sent */
        if ( u8MailboxSendReqStored & COE_SERVICE )
        {
            /* reset the flag indicating that CoE service to be sent was stored */
            u8MailboxSendReqStored &= ~COE_SERVICE;
            /* call CoE function that will send the stored CoE service */
            COE_ContinueInd(psWriteMbx);
        }
        else
        {
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief This function is called if the Master has requested a resending of the last
 \brief sent mailbox
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::MBX_MailboxRepeatReq(void)
{
    if (psRepeatMbx)
    {
        TMBX MBXMEM *pMbx = psRepeatMbx;
        /* send mailbox service stored for repeat */
        /* HBu 13.10.06: if a repeat request is received (again) before the previously repeated mailbox telegram
           was read from the master (psStoreMbx != NULL) the next mailbox telegram to be sent is still in the
            read mailbox so it has not to updated exchanged */
        ENTER_MBX_CRITICAL;

       if (bSendMbxIsFull && psStoreMbx == NULL)
        {
            /* mailbox is full, take the buffer off */
            HW_DisableSyncManChannel(MAILBOX_READ);

            /* store the buffer to be sent next */
            psStoreMbx = psReadMbx;
            /* enable the mailbox again */
            HW_EnableSyncManChannel(MAILBOX_READ);

            /* HBu 15.02.06: flag has to be reset otherwise the mailbox service
                             will not be copied by MBX_CopyToSendMailbox */
            bSendMbxIsFull = FALSE;
        }


        MBX_CopyToSendMailbox(pMbx);
        // HBu 17.06.06: psRepeatMbx has to be set to 0, when it was repeated, otherwise it would be returned twice
        // to the empty queue (MAILBOX_QUEUE=1) or a buffer get lost, if the the next repeat request will happen before
        // the repeated buffer was read
        psRepeatMbx = NULL;
        LEAVE_MBX_CRITICAL;
    }

    // Repeat was finished, toggle the acknowledge bit
    bMbxRepeatToggle = !bMbxRepeatToggle;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pMbx            Pointer to a Mailbox command to be sent (read by the Master)
 \param flags            Bit 0-6:    mailbox protocol type:
                                        0x01 - emergency service
                                        0x02 - CoE service
                                        0x04 - SoE service
                                        0x80 - EoE service
                                        0x10 - AoE service
                                        0x20 - VoE service
                             Bit 7:   0 - no more fragments to be sent for the requested mailbox service
                                       1 - additional fragments to be sent for the mailbox service, the
                                            corresponding XXXX_ContinueInd-function will be called to get
                                            the next fragment

 \return    0: Success - mailbox command could be stored in the send mailbox
            1: Failed - mailbox command could not be stored in the send mailbox, the
                            XXXX_ContinueInd service will be called when the mailbox was
                            read from the master to

 \brief        This function puts a new Mailbox service in the Send Mailbox
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 Ethercat::MBX_MailboxSendReq( TMBX MBXMEM * pMbx, UINT8 flags )
{
    UINT8 result = 0;

    /* HBu 06.02.06: in INIT-state a mailbox send request shall be refused */
    if ( (nAlStatus & STATE_MASK) == STATE_INIT )
        return ERROR_INVALIDSTATE;


    ENTER_MBX_CRITICAL;
    DISABLE_MBX_INT;

    /* the counter in the mailbox header has to be incremented with every new mailbox service to be sent
       if the mailbox data link layer is supported (software switch MAILBOX_REPEAT_SUPPORTED set)*/
    pMbx->MbxHeader.Flags[MBX_OFFS_COUNTER] &= ~MBX_MASK_COUNTER;
    /* HBu 13.02.06: Repeat-Counter was incremented too much if the mailbox service could not be sent */
    /* u8MbxCounter holds the actual counter for the mailbox header, only the values
       1-7 are allowed if the mailbox data link layer is supported  */
    if ( (u8MbxReadCounter & 0x07) == 0 )
        u8MbxReadCounter = 1;

    pMbx->MbxHeader.Flags[MBX_OFFS_COUNTER] |= u8MbxReadCounter << MBX_SHIFT_COUNTER;

    /* try to copy the mailbox command in the ESC */
    if ( MBX_CopyToSendMailbox(pMbx) != 0 )
    {
        /* no success, send mailbox was full, set flag  */
        result = PutInMbxQueue(pMbx, &sMbxSendQueue);
        if (result != 0)
            flags |= FRAGMENTS_FOLLOW;
        else
            u8MbxReadCounter++;
    }
    /* HBu 13.02.06: Repeat-Counter was incremented too much if the mailbox service could not be sent */
    else
    {
        u8MbxReadCounter++;
    }

    if ( flags & FRAGMENTS_FOLLOW )
    {
        /* store the mailbox service that the corresponding XXX_ContinueInd function will
           be called when the send mailbox will have been read by the master because there
           are mailbox commands to be sent for this service */
        u8MailboxSendReqStored |= (flags & ((UINT8) ~FRAGMENTS_FOLLOW));
    }

    ENABLE_MBX_INT;
    LEAVE_MBX_CRITICAL;

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function is used to check if the received mailbox command can be processed.

        Also the contents of the Receive Mailbox will be copied in the variable sMbx.
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::MBX_CheckAndCopyMailbox( void )
{
    UINT16 mbxLen;

    /* get the size of the received mailbox command and acknowledge the event*/
    HW_EscReadWord(mbxLen,u16EscAddrReceiveMbx);
    
    /* the size has to be swapped here, all other bytes of the mailbox service will be swapped later */
    mbxLen = SWAPWORD(mbxLen);

    /* the length of the mailbox data is in the first two bytes of the mailbox,
       so the length of the mailbox header has to be added */
    mbxLen += MBX_HEADER_SIZE;

    /* in this example there are only two mailbox buffers available in the firmware (one for processing and
       one to stored the last sent response for a possible repeat request), so a
       received mailbox service can only be processed if a free buffer is available */
    if ( ( bSendMbxIsFull )                /* a received mailbox service will not be processed
                                                    as long as the send mailbox is still full
                                                    (waits to be read from the master) */
        ||( u8MailboxSendReqStored )    /* a mailbox service to be sent is still stored
                                                    so the received mailbox service will not be processed
                                                    until all stored mailbox services are sent */
        )
    {
        /* set flag that the processing of the mailbox service will be checked in the
            function MBX_Main (called from ECAT_Main) */
        bReceiveMbxIsLocked = TRUE;
    }
    else
    {
        /* received mailbox command can be processed, reset flag */
        bReceiveMbxIsLocked = FALSE;
        /* if the read mailbox size is too big for the buffer, set the copy size to the maximum buffer size, otherwise
           memory could be overwritten,
           the evaluation of the mailbox size will be done in the mailbox protocols called from MBX_WriteMailboxInd */
        if (mbxLen > MBX_BUFFER_SIZE)
            mbxLen = MBX_BUFFER_SIZE;

        psWriteMbx = (TMBX MBXMEM *) APPL_AllocMailboxBuffer(MBX_BUFFER_SIZE);

        /* if there is no more memory for mailbox buffer, the mailbox should not be read */
        if (psWriteMbx == NULL)
        {
            /* set flag that the processing of the mailbox service will be checked in the
                function MBX_Main (called from ECAT_Main) */
            bReceiveMbxIsLocked = TRUE;
            return;
        }
        /* copy the mailbox header and data*/
        HW_EscReadMbxMem((MEM_ADDR MBXMEM *) psWriteMbx,u16EscAddrReceiveMbx,mbxLen);

        {
        /*Read Control and Status of SyncManager 0 to check if the buffer is unlocked*/
//        VARVOLATILE UINT16 smstate = 0x00;
		UINT16 smstate = 0x00;
        HW_EscReadWord(smstate,ESC_SYNCMAN_CONTROL_OFFSET);

        if(smstate & SM_STATUS_MBX_BUFFER_FULL)
//        while(smstate & SM_STATUS_MBX_BUFFER_FULL)	//cx
        {
            /*Unlock the mailbox SyncManger buffer*/
            u16dummy = 0;
            HW_EscReadWord(u16dummy,(u16EscAddrReceiveMbx + u16ReceiveMbxSize - 2));

//	        HW_EscReadWord(smstate,ESC_SYNCMAN_CONTROL_OFFSET);//cx

        }

        }

        /* in MBX_MailboxWriteInd the mailbox protocol will be processed */
        MBX_MailboxWriteInd( psWriteMbx );

    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pMbx        Pointer to the Mailbox command to be send to the master.

 \brief    This function copies data to the Send Mailbox.
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 Ethercat::MBX_CopyToSendMailbox( TMBX MBXMEM *pMbx )
{
    if ( (nAlStatus & STATE_MASK) == STATE_INIT)
    {
        /* the mailbox is disabled if the slave is in the INIT state */
        return( ERROR_INVALIDSTATE );
    }


    if ( !bMbxRunning )
    {
        /* the mailbox is disabled if the slave is in the INIT state */
        return( ERROR_INVALIDSTATE );
    }

    if ( bSendMbxIsFull )
    {
        /* mailbox service cannot be sent because the send mailbox is still full */
        return MBXERR_NOMOREMEMORY;
    }
    else
    {
        /* the variable mbxSize contains the size of the mailbox data to be sent */
        UINT16 mbxSize = pMbx->MbxHeader.Length;
//        HW_EscWriteMbxMem((UINT8 *)pMbx, u16EscAddrSendMbx, (mbxSize + MBX_HEADER_SIZE));
        HW_EscWriteMbxMem((MEM_ADDR *)pMbx, u16EscAddrSendMbx, (mbxSize + MBX_HEADER_SIZE));

        {
        /*Read Control and Status of SyncManager 1 to check if the buffer is still marked as empty*/
//        VARVOLATILE UINT16 smstate = 0x00;
        UINT16 smstate = 0x00;
		HW_EscReadWord(smstate,(ESC_SYNCMAN_CONTROL_OFFSET + SYNCMAN_REG_SIZE));

        if(!(smstate & SM_STATUS_MBX_BUFFER_FULL))
        {
            UINT8 BytesLeft = u16SendMbxSize - (mbxSize + MBX_HEADER_SIZE);

            /*Write last Byte to trigger mailbox full flag*/
            /*Read last 2 Bytes and write them again (required if low Byte of the WORD were written before)*/
            u16dummy = 0;
            if(BytesLeft < 2)
            {
                /*The last 2Bytes are overlapping the already written buffer*/
                                
                /*Get the valid 16Bit address*/
                UINT32 LastDataAddress = ((mbxSize + MBX_HEADER_SIZE)/2)*2;
            
                /*Copy the buffer to overwrite*/
                MEMCPY((UINT16 *)&u16dummy,(((UINT8 *)pMbx) + LastDataAddress),(2 - BytesLeft));
            }
            
            HW_EscWriteWord(u16dummy,(u16EscAddrSendMbx + u16SendMbxSize - 2));
        }
        }

        /* store last send mailbox service for a possible repeat
            one buffer includes the last send service (psRepeatMbx),
            the other one the actual service to be sent (psReadMbx),
            there is no buffer available for a mailbox receive service
            until the last sent buffer was read from the master
            the exception is after the INIT2PREOP transition, in that
            case there is no last sent service (psReadMbx = 0) */
        if ( psReadMbx )
            psWriteMbx = NULL;
        else
        {
            /* only the first time after the INIT2PREOP-transition */
            psWriteMbx = &asMbx[1];
        }
        // HBu 17.06.06: psRepeatMbx was already updated in MBX_MailboxReadInd
        // psRepeatMbx = psReadMbx;
        psReadMbx = pMbx;

        /* set flag that send mailbox is full now */
        bSendMbxIsFull = TRUE;
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function is called cyclically to check if a received Mailbox service was
             stored.
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::MBX_Main(void)
{
    TMBX MBXMEM *pMbx = NULL;

    do
    {
        UINT8 result = 0;

        pMbx = GetOutOfMbxQueue(&sMbxReceiveQueue);
        if ( pMbx )
            result = MailboxServiceInd(pMbx);

        if ( result != 0 )
        {
            /* Mailbox error response: type 0 (mailbox service protocol) */
            pMbx->MbxHeader.Length     = 4;
            pMbx->MbxHeader.Flags[MBX_OFFS_TYPE]        &= ~(MBX_MASK_TYPE);
            pMbx->Data[0]                        = SWAPWORD(MBXSERVICE_MBXERRORCMD);
            pMbx->Data[1]                        = SWAPWORD(result);
            MBX_MailboxSendReq(pMbx, 0);
        }
    }
    while ( pMbx != NULL );



      if ( bReceiveMbxIsLocked )
      {
          /* the work on the receive mailbox is locked, check if it can be unlocked (if all
             mailbox commands has been sent */
          MBX_CheckAndCopyMailbox();
      }
}

/** @} */



