/**
\defgroup ecatslv ecatslv.c: EtherCAT State Machine
\brief Changes to version V5.0:
\brief V5.01 APPL3: Include library demo application
\brief V5.01 ESC2: Add missed value swapping
\brief V5.01 ESM1: Don't overwrite the error reason in case of an failed PS transition
\brief V5.01 ESM2: Don't check the "appl trigger" flag in case on an regular transition to a lower state (OS, SP, PI).
\brief V5.01 ESM3: Call Error acknowledge indication only if error was acknowledged by the master
\brief V5.01 HW3: Update blink code of an SM watchdog error
\brief Changes to version V4.42:
\brief V5.0 ECAT1: Support Explicit Device ID.
\brief V5.0 ECAT2: Application specific functions are moved to application files.
\brief V5.0 ECAT3: Global dummy variables used for dummy ESC operations.
\brief V5.0 ESC1: ESC 32Bit Access added.
\brief V5.0 ESC2: Support ESC EtherCAT LED Indication.
\brief V5.0 ESC3: Support EEPROM Emulation.
\brief V5.0 ESM1: Update "LocalErrorFlag" handling.
\brief V5.0 ESM2: Update Error Acknowledge by ALControl INIT (without error acknowledge)

\brief V5.0 ESM3: Handle pending ESM transition
\brief V5.0 ESM4: ECAT_StateChange() will only be called form application. In case of an communication error AL_ControlInd is called.
\brief V5.0 MBX1: Support configuration without mailbox protocol support.
\brief V5.0 TEST1: Add test application. See Application Note ET9300 for more details.
\brief Changes to version V4.40:\n
\brief V4.42 ESM1: Reset local error flag if master set the acknowledge bit (0x120.4)
\brief Changes to version V4.30:\n
\brief V4.40 ESM5: Enable output SyncManager if local error acknowledged
\brief V4.40 HW0: Use common hardware access functions
\brief V4.40 PDO3: Add support if only input process data is used
\brief V4.40 ECAT4: Add read SM activation register to acknowledge SM Change event
\brief V4.40 PDO2: Check if max process data size was exceed
\brief V4.40 DIAG1: add diagnosis message support
\brief V4.40 ESM4: Change Check WD setup; add define OP_PD_REQUIRED (defines if process data required in state change to OP)
\brief V4.40 WD1: change WD behaviour depending if process data required in OP state
\brief V4.40 MBX4: Change processing order of mailbox SyncManager flags
\brief V4.40 ECAT1: Merge content of HW_Main (spihw.c /mcihw.c) to ECAT_Main
\brief V4.40 ECAT2: Added CheckIfLocalError() to check local flags and set ALStatus /Al Status code if required. This function is called cyclic from MainLoop.
\brief V4.40 ESM2: Add AL_ControlRes() to complete pending state requests. Change SafeOP to OP state response
\brief V4.40 ESM1: Prevent double call of StopOutputHandler()
\brief V4.40 BOOT1: Enable Mailbox SyncManger on state change to BOOT state (to enable FoE)
\brief V4.40 ESM3: Change State machine behaviour according to ETG.1000 V1.0.2 (state change #26)
\brief V4.40 LED1: Set error blink code
\brief V4.40 TIMER1: Added DC_CheckWatchdog() triggered from ECAT_CheckTimer(). Change local Sync0 watchdog variables. Change bus cycle calculation
\brief V4.40 WD1: Change check process data watchdog settings
\brief Changes to version V4.20:\n
\brief V4.30 OBJ 3: initialize the object dictionary in state change INIT->PREOP; clear object dictionary in state change PREOP->INIT
\brief V4.30 SYNC: add 0x1C32:10; 0x1C33:10 (Sync0 cycle), change synchronisation control functionality
\brief V4.30 CiA402: add CiA402_Init() call in state change from PREOP to SAFEOP if DC synchronisation is enabled,
\brief                    else the Init function is called when bus cycle time is calculated [CalcSMCycleTime() ].
\brief                    trigger error handling if the EtherCAT state machine gets a transition from OP to an "lower" state
\brief V4.20 ECAT 1: add LEGACY_MODE behaviour in ECAT_CheckWatchdog()
\brief V4.20 DC 1: Add DC pending state machine handling and Dc watchdog functionality
\brief V4.20 ESM 2: Add State transition from BOOT to INIT
\brief V4.20 ESM 1: Non LEGACY_MODE State change handling
\brief V4.11 Renamed the function parameter "code" of Function "SendSmFailedEmergency() to avoid
\brief problems with some compilers"\n
\brief V4.11 ECAT 1: Fixed a possible problem with state change Init -> SafeOP. The output syncmanager
\brief was enabled by the state change-flag and not by the actual state\n
\brief V4.11 LED 1: Clear the error LED during error acknowledgement\n
\brief V4.11 ESC 1: fixed size of MBXHEADER in the TFOEMBX struct \n
\brief Changes to version V4.08:\n
\brief V4.10 ECAT 1: clear bEcatOutputsReceived in startMailboxhandler()\n
\brief V4.10 ECAT 2: clear bEcatOutputsReceived in stopMailboxhandler()\n
\brief V4.10 ECAT 3: when switching from INIT to BOOT the SM settings shall be checked\n
\brief V4.10 ECAT 4: APPL_StartInputHandler shall always be called and bEcatInputUpdateRunning shall always be set
\brief               in StartInputHandler independent of the input size\n
\brief V4.10 ECAT 5: AL_ControlInd: the error acknowledge behaviour was changed
\brief               according to the protocol enhancements and the conformance test\n
\brief V4.10 ECAT 6: AL_ControlInd: if a state transitions failed the corresponding stop function is
\brief               called to get a consistent set of variables\n
\brief V4.10 ECAT 7: the local application requested to leave the state OP so we have to disable the SM2
\brief                    and make the state change from OP to SAFEOP by calling StopOutputHandler\n
\brief V4.10 ECAT 8: the AL Status Code has to be reset if the error was acknowledged by the master\n
\brief V4.10 ECAT 9: ECAT_StateChange: when waiting for a State Change response from the application the
\brief               AL Status shall only be written if the final state was reached\n
\brief Changes to version V4.07:\n
\brief V4.08 ECAT 1: The watchdog value was not rounded up\n
\brief V4.08 ECAT 2: The value of u16WdValue was not set 0 if the register 0x420 is 0\n
\brief V4.08 ECAT 3: The AlStatusCode is changed as parameter of the function AL_ControlInd\n
\brief V4.08 ECAT 4: In a state transition OP2PREOP, SAFEOP2INIT or OP2INIT is requested,
\brief               this was not working correctly if one of the application functions
\brief               APPL_StopInputHandler or APPL_StopOutputHandler were returning NOERROR_INWORK
\brief               (because only the first state transition was made in that case)\n
\brief V4.08 AOE 1:    AoE was added\n
\brief Changes to version V4.06:\n
\brief V4.07 ECAT 1: The sources for SPI and MCI were merged (in ecat_def.h
\brief                    set the switch MCI_HW to 1 when using the MCI,
\brief                    set the switch SPI_HW to 1 when using the SPI\n
\brief Changes to version V4.00:\n
\brief V4.01 ECAT 1: The Output sync Manager was not disabled when the state OP was left
\brief               by a local request (watchdog or io error)\n
\brief V4.01 ECAT 2: APPL_StopOutputHandler returns an UINT16\n
\brief V4.01 ECAT 3: TwinCAT compatibility mode: The state transition to OP is allowed when the
\brief                     WD-Trigger-Bit of the SM2-Control-Byte (0x814.6) is FALSE, in that case the
\brief                     watchdog will not be started before the outputs were received the first time\n
\brief V4.01 ECAT 4: "else" was too much\n
\brief Changes to version V3.20:\n
\brief V4.00 ECAT 1: The handling of the Sync Manager Parameter was included according to
\brief               the EtherCAT Guidelines and Protocol Enhancements Specification\n
\brief V4.00 ECAT 2: The output sync manager is initialized during the state transition
\brief               from PREOP to SAFEOP that the master can check if the slave could update
\brief               inputs and outputs before switching the slave to OP\n
\brief               behaviour according to the EtherCAT Guidelines and Protocol Enhancements Specification\n
\brief V4.00 ECAT 3: The watchdog will be enabled in SAFE-OP that it can be checked if the last SM event
\brief               was received during the watchdog time before switching to OP\n
\brief V4.00 ECAT 4: The function CheckSmChannelParameters is included in the function
\brief               CheckSmSettings to get a better overview\n
\brief V4.00 ECAT 5: In synchronous mode the slave should support 1- and 3-buffer mode, 3-buffer mode
\brief               should be the standard setting, because the controlling if the process data was updated
\brief               should be done with the TxPDO Toggle, but the 1-buffer mode should be setable too,
\brief               that the master could easily check if all slaves are synchronous by checking the
\brief               the working counter (if the outputs were not read or the inputs were not written
\brief               the ESC of the slave would not increment the working counter with expected value
\brief               if the 1-buffer mode is running)\n
\brief V4.00 ECAT 6: The function ECAT_StateChange was added, which the application should call if a local error
\brief                    is detected (with the parameters alStatus = STATE_SAFEOP, alStatusCode = error code (> 0x1000))
\brief                    or gone (with the parameters alStatus = STATE_OP, alStatusCode = 0)
\brief                    or if one of the functions APPL_StartMailboxHandler, APPL_StopMailboxHandler, APPL_StartInputHandler,
\brief                    APPL_StopInputHandler, APPL_StartOutputHandler, APPL_StopOutputHandler has returned NOERROR_INWORK
\brief                    to acknowledge the last state transition (with the parameters alStatus = new AL-Status, alStatusCode =
\brief                    new AL-Status-Code)\n
\brief V4.00 ECAT 7: The return values for the AL-StatusCode were changed to UINT16\n


\version 5.01
*/

//---------------------------------------------------------------------------------------
/**
\ingroup ecatslv
\file ecatslv.c
\brief Implementation.
*/
//---------------------------------------------------------------------------------------

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/


#include "ethercat.h"
extern TOBJ10F1 sErrorSettings;
/*--------------------------------------------------------------------------------------
------
------    local Types and Defines
------
--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/
//UINT16    u16ALEventMask;                      // Value which will be written to the 0x204 register (AL event mask) during the state transition PreOP to SafeOP

/*Dummy variable to trigger read or writes events in the ESC*/
//    VARVOLATILE UINT16    u16dummy;

/*-----------------------------------------------------------------------------------------
------
------    local functions
------
-----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------    functions
------
-----------------------------------------------------------------------------------------*/

/**
\addtogroup ecatslv
@{
*/



/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param  maxChannel    last SM channel which should be checked

 \return                 0: okay else AL Status Code

 \brief    This function checks all SM channels

*////////////////////////////////////////////////////////////////////////////////////////

UINT8    Ethercat::CheckSmSettings(UINT8 maxChannel)
{
    UINT8 i;
    UINT8 result = 0;
    TSYNCMAN ESCMEM *pSyncMan;
    UINT16 SMLength = 0;
    UINT16 SMAddress = 0;

    /* check the Sync Manager Parameter for the Receive Mailbox (Sync Manager Channel 0) */
    pSyncMan = HW_GetSyncMan(MAILBOX_WRITE);

    SMLength = pSyncMan->Length;
    SMAddress = pSyncMan->PhysicalStartAddress;

    if (!(pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE))
        /* receive mailbox is not enabled */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    else if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_DIRECTION_MASK) != SM_SETTING_DIRECTION_WRITE_VALUE)
       /* receive mailbox is not writable by the master*/
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    else if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_MASK) != SM_SETTING_MODE_ONE_BUFFER_VALUE )
        /* receive mailbox is not in one buffer mode */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    else if ( SMLength < MIN_MBX_SIZE )
        /* receive mailbox size is too small */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    else if ( SMLength > MAX_MBX_SIZE )
        /* receive mailbox size is too great */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
     else if ( SMAddress < MIN_MBX_WRITE_ADDRESS )
        /* receive mailbox address is too small */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
    else if ( SMAddress > MAX_MBX_WRITE_ADDRESS)
        /* receive mailbox address is too great */
        result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;


    if ( result == 0 )
    {
        /* check the Sync Manager Parameter for the Send Mailbox (Sync Manager Channel 1) */
        pSyncMan = HW_GetSyncMan(MAILBOX_READ);

    SMLength = pSyncMan->Length;
    SMAddress = pSyncMan->PhysicalStartAddress;

      if (!(pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE))
            /* send mailbox is not enabled */
            result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
        else if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_DIRECTION_MASK) != SM_SETTING_DIRECTION_READ_VALUE)
           /* receive mailbox is not readable by the master*/
            result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
        else if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_MASK) != SM_SETTING_MODE_ONE_BUFFER_VALUE )
            /* receive mailbox is not in one buffer mode */
            result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
        else if ( SMLength < MIN_MBX_SIZE )
            /* send mailbox size is too small */
            result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
        else if ( SMLength > MAX_MBX_SIZE )
            /* send mailbox size is too great */
            result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
         else if ( SMAddress < MIN_MBX_READ_ADDRESS )
            /* send mailbox address is too small */
            result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;
        else if ( SMAddress > MAX_MBX_READ_ADDRESS )
            /* send mailbox address is too great */
            result = ALSTATUSCODE_INVALIDMBXCFGINPREOP;

    }
    if ( result == 0 && maxChannel > PROCESS_DATA_IN )
    {
        /* b3BufferMode is only set, if inputs and outputs are running in 3-Buffer-Mode when leaving this function */
        b3BufferMode = TRUE;
        /* check the Sync Manager Parameter for the Inputs (Sync Manager Channel 3) */
        pSyncMan = HW_GetSyncMan(PROCESS_DATA_IN);

    SMLength = pSyncMan->Length;
    SMAddress = pSyncMan->PhysicalStartAddress;

        if ( (pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE) != 0 && SMLength == 0 )
            /* the SM3 size is 0 and the SM3 is active */
            result = SYNCMANCHSETTINGS+1;
        else if (pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE)
        {
            /* Sync Manager Channel 3 is active, input size has to greater 0 */
            if ( SMLength != nPdInputSize || nPdInputSize == 0 || SMLength > MAX_PD_INPUT_SIZE)
                /* sizes don't match */
                result = SYNCMANCHSIZE+1;
            else
                /* sizes matches */
            if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_DIRECTION_MASK) == SM_SETTING_DIRECTION_READ_VALUE )
            {
                /* settings match */
                if ( ( ( nAlStatus == STATE_PREOP )&&( SMAddress >= MIN_PD_READ_ADDRESS )&&( SMAddress <= MAX_PD_READ_ADDRESS ) )
                   ||( ( nAlStatus != STATE_PREOP )&&( SMAddress == nEscAddrInputData ) )
                    )
                {
                    /* addresses match */

                    if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_MASK) == SM_SETTING_MODE_ONE_BUFFER_VALUE )
                        /* inputs are running in 1-Buffer-Mode, reset flag b3BufferMode */
                        b3BufferMode = FALSE;
                }
                else
                    /* input address is out of the allowed area or has changed in SAFEOP or OP */
                    result = SYNCMANCHADDRESS+1;
            }
            else
                /* input settings do not match */
                result = SYNCMANCHSETTINGS+1;
        }
        else if ( SMLength != 0 || nPdInputSize != 0 )
            /* input size is not zero although the SM3 channel is not enabled */
            result = SYNCMANCHSIZE+1;

        if ( result != 0 )
        {
            result = ALSTATUSCODE_INVALIDSMINCFG;
        }
    }
//    else

    if ( result == 0 && maxChannel > PROCESS_DATA_OUT )
    {
        /* check the Sync Manager Parameter for the Outputs (Sync Manager Channel 2) */
        pSyncMan = HW_GetSyncMan(PROCESS_DATA_OUT);

    SMLength = pSyncMan->Length;
    SMAddress = pSyncMan->PhysicalStartAddress;

    if ( (pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE) != 0 && SMLength == 0 )
            /* the SM2 size is 0 and the SM2 is active */
            result = SYNCMANCHSETTINGS+1;
        else if (pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET] & SM_SETTING_ENABLE_VALUE)
        {
            /* Sync Manager Channel 2 is active, output size has to greater 0 */
            if ( SMLength == nPdOutputSize && nPdOutputSize != 0 && SMLength <= ((UINT16)MAX_PD_OUTPUT_SIZE))

            {
                /* sizes match */
                if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_DIRECTION_MASK) == SM_SETTING_DIRECTION_WRITE_VALUE )
                {
                    /* settings match */
                    if ( ( ( nAlStatus == STATE_PREOP )&&( SMAddress >= MIN_PD_WRITE_ADDRESS )&&( SMAddress <= MAX_PD_WRITE_ADDRESS ) )
                       ||( ( nAlStatus != STATE_PREOP )&&( SMAddress == nEscAddrOutputData ) )
                        )
                    {
                        /* addresses match */
                        {
                            /* check, if watchdog trigger is enabled */
                            if (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_WATCHDOG_VALUE)
                            {
                                bWdTrigger = TRUE;
                            }
                            else
                            {
                                bWdTrigger = FALSE;
                            }

                            if ( (pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_MASK) == SM_SETTING_MODE_ONE_BUFFER_VALUE )
                                /* outputs are running in 1-Buffer-Mode, reset flag b3BufferMode */
                                b3BufferMode = FALSE;
                        }
                    }
                    else
                        /* output address is out of the allowed area or has changed in SAFEOP or OP */
                        result = SYNCMANCHADDRESS+1;
                }
                else
                    /* output settings do not match */
                    result = SYNCMANCHSETTINGS+1;
            }
            else
                /* output sizes don't match */
                result = SYNCMANCHSIZE+1;
        }
        else if ( SMLength != 0 || nPdOutputSize != 0 )
            /* output size is not zero although the SM2 channel is not enabled */
            result = SYNCMANCHSIZE+1;


        if ( result != 0 )
        {
            result = ALSTATUSCODE_INVALIDSMOUTCFG;
        }
    }

    if ( result == 0 )
    {
        UINT16 SMActivate = 0;
        /* the Enable-Byte of the rest of the SM channels has to be read to acknowledge the SM-Change-Interrupt */
        for (i = maxChannel; i < nMaxSyncMan; i++)
        {
            pSyncMan = HW_GetSyncMan(i);
            SMActivate = pSyncMan->Settings[SM_SETTING_ACTIVATE_OFFSET];
        }
    }
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    This function is called in case of the state transition from PREOP to SAFEOP.
 |brief  the areas of the Sync Managers will be checked for overlapping,
 \brief  the synchronization mode (Free Run, Synchron, Distributed Clocks) is selected,
 \brief  the requested cycle time will be checked, the watchdog is started
 \brief  and the AL Event Mask register will be set

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::StartInputHandler(void)
{
    TSYNCMAN ESCMEM * pSyncMan;
    UINT16        dcControl;
    UINT32     cycleTimeSync0 = 0;
    UINT32     cycleTimeSync1 = 0;
    UINT16     wdiv;
    UINT16     wd;
    UINT16    nPdInputBuffer = 3;
    UINT16    nPdOutputBuffer = 3;

    u16ALEventMask = 0;

    bEcatFirstOutputsReceived = FALSE;

    /* get a pointer to the Sync Manager Channel 2 (Outputs) */
    pSyncMan = HW_GetSyncMan(PROCESS_DATA_OUT);
    /* store the address of the Sync Manager Channel 2 (Outputs) */
    nEscAddrOutputData = pSyncMan->PhysicalStartAddress;
    /* get the number of output buffer used for calculating the address areas */
    if ( pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_ONE_BUFFER_VALUE )
        nPdOutputBuffer = 1;
    /* get a pointer to the Sync Manager Channel 3 (Inputs) */
    pSyncMan = HW_GetSyncMan(PROCESS_DATA_IN);
    /* store the address of the Sync Manager Channel 3 (Inputs) */
    nEscAddrInputData = pSyncMan->PhysicalStartAddress;
    
    /* get the number of input buffer used for calculating the address areas */
    if ( pSyncMan->Settings[SM_SETTING_CONTROL_OFFSET] & SM_SETTING_MODE_ONE_BUFFER_VALUE )
        nPdInputBuffer = 1;

    /* it has be checked if the Sync Manager memory areas for Inputs and Outputs will not overlap
       the Sync Manager memory areas for the Mailbox */
    if ( ((nEscAddrInputData + nPdInputSize * nPdInputBuffer) > u16EscAddrSendMbx && (nEscAddrInputData < (u16EscAddrSendMbx + u16SendMbxSize)))
        ||((nEscAddrInputData + nPdInputSize * nPdInputBuffer) > u16EscAddrReceiveMbx && (nEscAddrInputData < (u16EscAddrReceiveMbx + u16ReceiveMbxSize)))
        )
    {
        return ALSTATUSCODE_INVALIDSMINCFG;
    }

    if (
        ((nEscAddrOutputData + nPdOutputSize * nPdOutputBuffer) > u16EscAddrSendMbx && (nEscAddrOutputData < (u16EscAddrSendMbx + u16SendMbxSize)))
        ||((nEscAddrOutputData + nPdOutputSize * nPdOutputBuffer) > u16EscAddrReceiveMbx && (nEscAddrOutputData < (u16EscAddrReceiveMbx + u16ReceiveMbxSize)))
        ||
        ((nEscAddrOutputData + nPdOutputSize * nPdOutputBuffer) > nEscAddrInputData && (nEscAddrOutputData < (nEscAddrInputData + nPdInputSize)))
        )
    {
        /* Sync Manager Channel 2 memory area (Outputs) overlaps the Sync Manager memory areas for the Mailbox
           or the Sync Manager Channel 3 memory area (Inputs) */
        return ALSTATUSCODE_INVALIDSMOUTCFG;
    }

    /* check the DC-Registers */
    /*Read registers 0x980:0x981 (corresponding masks are adapted)*/
    HW_EscReadWord(dcControl, ESC_DC_UNIT_CONTROL_OFFSET);
    dcControl = SWAPWORD(dcControl);

    /* check if Distributed Clocks are enabled */
    if ( dcControl & (ESC_DC_SYNC0_ACTIVE_MASK | ESC_DC_SYNC1_ACTIVE_MASK) )
    {
        /* DC Mode is requested */
        /* Distributed Clocks are enabled, check the correct SYNC0/SYNC1-setting,
           DC_SYNC_ACTIVE is set per default to SYNC0 (in ecatslv.h)
           but this can be overwritten in ecat_def.h */
        if ( dcControl != (ESC_DC_SYNC_UNIT_ACTIVE_MASK | DC_SYNC_ACTIVE) )
        {
            return ALSTATUSCODE_DCINVALIDSYNCCFG;
        }
        else
        {
            /* DC mask for the AL-Event-Mask-Register (0x204)
                DC_EVENT_MASK is set by default to PROCESS_OUTPUT_EVENT
               (optimized DC Cycle in the Synchronization document)
               but this can be overwritten in ecat_def.h */
            u16ALEventMask = DC_EVENT_MASK;
        }

        /* slave is running in DC-mode */
        bDcSyncActive = TRUE;
        // Cycletime for Sync0
        HW_EscReadDWord(cycleTimeSync0, ESC_DC_SYNC0_CYCLETIME_OFFSET);
/* ECATCHANGE_START(V5.01) ESC2*/
        cycleTimeSync0 = SWAPDWORD(cycleTimeSync0);
/* ECATCHANGE_END(V5.01) ESC2*/

        // Cycletime for Sync1
        HW_EscReadDWord(cycleTimeSync1, ESC_DC_SYNC1_CYCLETIME_OFFSET);
/* ECATCHANGE_START(V5.01) ESC2*/
        cycleTimeSync1 = SWAPDWORD(cycleTimeSync1);
/* ECATCHANGE_END(V5.01) ESC2*/
    }
    else
    {
        bDcSyncActive = FALSE;
        /* check entry 0x1C32:01, if free run or synchron synchronization mode is requested */
        if ( sSyncManOutPar.u16SyncType != SYNCTYPE_FREERUN )
        {
            /* ECAT Synchron Mode, the ESC interrupt is enabled */
            bEscIntEnabled = TRUE;
        }
        else
        {
            /* ECAT FreeRun Mode, sync manager has to run in 3-Buffer Mode */
            if ( !b3BufferMode )
                /* 1-Buffer-Mode, refuse the state transition */
                return ALSTATUSCODE_FREERUNNEEDS3BUFFERMODE;
        }
    }

    /* if ECAT Synchron Mode is active and the output size is zero,
       the ESC interrupt will be generated by the PROCESS_INPUT_EVENT */
    if ( bEscIntEnabled && nPdOutputSize == 0 )
    {
        /* Sync Manager Channel 3 event has to activated in the AL-Event mask register */
        u16ALEventMask |= PROCESS_INPUT_EVENT;
    }

    /* check if the requested cycle time exceeds the minimum and maximum values of the cycle time
       (MIN_PDCYCLE_TIME and MAX_PD_CYCLE_TIME are defined in ecat_def.h) */
    if(bDcSyncActive)
    {
        /*bus is synchronised to Sync1 => check if Sync1 cycle time is valid*/
        if(cycleTimeSync1 != 0)
        {
            if (cycleTimeSync1 < MIN_PD_CYCLE_TIME || cycleTimeSync1 > MAX_PD_CYCLE_TIME)
                return ALSTATUSCODE_DCSYNC1CYCLETIME;
        }
        else
        {
            if ( cycleTimeSync0 != 0 && (cycleTimeSync0 < MIN_PD_CYCLE_TIME || cycleTimeSync0 > MAX_PD_CYCLE_TIME) )
                return ALSTATUSCODE_DCSYNC0CYCLETIME;
        }
    }

    /* reset the error counter indicating synchronization problems */
    sCycleDiag.syncFailedCounter = 0;
    /*get the watchdog time (register 0x420). if value is > 0 watchdog is active*/
    HW_EscReadWord(wd, ESC_PD_WD_TIME);
/* ECATCHANGE_START(V5.01) ESC2*/
    wd = SWAPWORD(wd);
/* ECATCHANGE_END(V5.01) ESC2*/
    if (nPdOutputSize > 0 &&  wd != 0 )
    {
    /*get watchdog divider (register 0x400)*/
    HW_EscReadWord(wdiv, ESC_WD_DIVIDER_OFFSET);
/* ECATCHANGE_START(V5.01) ESC2*/
    wdiv = SWAPWORD(wdiv);
/* ECATCHANGE_END(V5.01) ESC2*/
        if ( wdiv != 0 )
        {
            /* the ESC subtracts 2 in register 0x400 so it has to be added here */
            UINT32 d = wdiv+2;

            d *= wd;
            /* store watchdog in ms in variable u16WdValue */
            /* watchdog value has to be rounded up */
            d += 24999;
            d /= 25000;
            EcatWdValue = (UINT16) d;
        }
        else
        {
            wd = 0;
            /* wd value has to be set to zero, if the wd is 0 */
            EcatWdValue = 0;
        }
    }
    else
    {
        /* the watchdog is deactivated or slave has no output process data*/
        wdiv = 0;
        EcatWdValue = 0;
    }

    if((EcatWdValue == 0 && bWdTrigger) || (EcatWdValue != 0 && !bWdTrigger))
    {
        /* if the WD-Trigger in the Sync Manager Channel 2 Control-Byte is set (Bit 6 of Register 0x814)
            an error has to be returned */
        return ALSTATUSCODE_INVALIDWDCFG;
    }


    if ( bEscIntEnabled && nPdOutputSize != 0 )
    {
        /* ECAT synchron Mode is active, the Sync Manager Channel 2 event
           has to activated in the AL-Event mask register */
        u16ALEventMask |= PROCESS_OUTPUT_EVENT;
    }
/*The application ESM function is separated from this function to handle pending transitions*/

    Sync0WdValue = 0;
    Sync0WdCounter = 0;
    bDcRunning = FALSE;
    bSmSyncToggle = FALSE;
    bPllRunning = FALSE;
    i16WaitForPllRunningTimeout = 0;
    sSyncManInPar.u16SyncError = 0;
    sSyncManOutPar.u16SyncError = 0;
    sSyncManOutPar.u32SmEventMissedCounter = 0;

    /* update the entries 0x1C32:01 and 0x1C33:01 */
    if ( bDcSyncActive )
    {
        /*calculate the Sync0 Watchdog counter value the minimum value is 1 ms
            if the sync0 cycle is greater 500us the Sync0 Wd value is 2*Sycn0 cycle */
        if(cycleTimeSync0 == 0)
        {
            Sync0WdValue = 0;
        }
        else
        {
            UINT32 Sync0Cycle = cycleTimeSync0/100000;
            if(Sync0Cycle < 5)
            {
                /*Sync0 cycle less than 500us*/
                Sync0WdValue = 1;
            }
            else
            {
                Sync0WdValue = (Sync0Cycle*2)/10;
            }
        }
        if(cycleTimeSync1 != 0)
        {
            /*the applications synchronised to Sync1*/
            sSyncManOutPar.u32Sync0CycleTime = cycleTimeSync0;
            sSyncManInPar.u32Sync0CycleTime = cycleTimeSync0;

            sSyncManOutPar.u32CycleTime = cycleTimeSync1;
            sSyncManInPar.u32CycleTime = cycleTimeSync1;

            sSyncManOutPar.u16SyncType = SYNCTYPE_DCSYNC1;
            sSyncManInPar.u16SyncType = SYNCTYPE_DCSYNC1;

        }
        else
        {
            sSyncManOutPar.u32CycleTime = cycleTimeSync0;
            sSyncManInPar.u32CycleTime = cycleTimeSync0;

            /*the bus is synchronised with Sync0*/
            sSyncManOutPar.u16SyncType = SYNCTYPE_DCSYNC0;
            sSyncManInPar.u16SyncType = SYNCTYPE_DCSYNC0;
        }
    }
    else

    if ( bEscIntEnabled )
    {
        /* ECAT Synchron Mode */
        sSyncManOutPar.u16SyncType = SYNCTYPE_SYNCHRON;
        sSyncManInPar.u16SyncType = SYNCTYPE_SM2INT;
        /*Trigger bus cycle calculation*/
        sSyncManOutPar.u16GetCycleTime = 1;
    }
    else
    {
        /* ECAT FreeRun Mode */
        sSyncManOutPar.u16SyncType = SYNCTYPE_FREERUN;
        sSyncManInPar.u16SyncType = SYNCTYPE_FREERUN;
    }
/*CiA specific code is moved to CiA402 ESM functions*/
    if(nPdOutputSize > 0)
    {
        HW_EnableSyncManChannel(PROCESS_DATA_OUT);
    }
    
    if(nPdInputSize > 0)
    {
        HW_EnableSyncManChannel(PROCESS_DATA_IN);
    }

    /*write initial input data*/
    PDO_InputMapping();

    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    This function is called in case of the state transition from SAFEOP to OP.
 \brief  It will be checked if outputs had to be received before switching to OP
 \brief  and the state transition would be refused if outputs are missing

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::StartOutputHandler(void)
{
    UINT16 result = ALSTATUSCODE_NOERROR;
    if(bLocalErrorFlag)
    {
        /*Local error still exists => skip state request to OP and response with "u16LocalErrorCode"*/
        return u16LocalErrorCode;
    }
/*The application ESM function is separated from this function to handle pending transitions*/


    /*DC synchronisation is active wait until pll is valid*/
//CX: Analog Input does not need to assert PDO Output is stable, the following lines are remarked.
//CX: For DO & AO, recover the following line
/*
    if(bDcSyncActive)
    {
        i16WaitForPllRunningTimeout = 200;
        i16WaitForPllRunningCnt = 0;
        result = NOERROR_INWORK;
    }
*/

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function is called in case of the state transition from OP to SAFEOP
 \brief  the outputs can be set to an application specific safe state,
 \brief  the state transition can be delayed by returning NOERROR_INWORK

*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::StopOutputHandler(void)
{
    /* reset the flags that outputs were received and that the slave is in OP */
    bEcatFirstOutputsReceived = FALSE;
     bEcatOutputUpdateRunning = FALSE;

/*The application ESM function is separated from this function to handle pending transitions*/
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
  \brief    This function is called in case of the state transition from SAFEOP to PREOP

*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::StopInputHandler(void)
{
    if(nPdOutputSize > 0)
    {
        /* disable the Sync Manager Channel 2 (outputs) */
        HW_DisableSyncManChannel(PROCESS_DATA_OUT);
    }
    
    if(nPdInputSize > 0)
    {
        /*disable Sync Manager 3 (inputs) if no outputs available*/
        HW_DisableSyncManChannel(PROCESS_DATA_IN);
    }

    /* reset the events in the AL Event mask register (0x204) */
    HW_ResetALEventMask( ~(SYNC0_EVENT | SYNC1_EVENT | PROCESS_INPUT_EVENT | PROCESS_OUTPUT_EVENT) );
    /* reset the flags */
    bEcatFirstOutputsReceived = FALSE;
    bEscIntEnabled = FALSE;
/*The application ESM function is separated from this function to handle pending transitions*/

    bDcSyncActive = FALSE;
    bDcRunning = FALSE;
    bPllRunning = FALSE;
    bSmSyncToggle = FALSE;

    i16WaitForPllRunningTimeout = 0;


    bWdTrigger = FALSE;
    bEcatInputUpdateRunning = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param alStatus        New AL Status (written to register 0x130)
 \param alStatusCode    New AL Status Code (written to register 0x134)

  \brief  The function changes the state of the EtherCAT ASIC to the requested.
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::SetALStatus(UINT8 alStatus, UINT16 alStatusCode)
{
    UINT16 Value = alStatusCode;

    /*update global status variable if required*/
    if(nAlStatus != alStatus)
    {
        nAlStatus = alStatus;
    }


    if (alStatusCode != 0xFFFF)
    {
        Value = SWAPWORD(Value);

        HW_EscWriteWord(Value,ESC_AL_STATUS_CODE_OFFSET);
    }

    Value = nAlStatus;
    Value = SWAPWORD(Value);
    HW_EscWriteWord(Value,ESC_AL_STATUS_OFFSET);

    /*The Run LED state is set in Set LED Indication, only the Error LED blink code is set here*/

    /*set Error blink code*/
    if(alStatusCode == 0x00 || !(alStatus & STATE_CHANGE))
    {
        u8EcatErrorLed = LED_OFF;
    }
    else if((alStatusCode == ALSTATUSCODE_NOSYNCERROR) ||
        (alStatusCode == ALSTATUSCODE_DCPLLSYNCERROR))
    {
        u8EcatErrorLed = LED_SINGLEFLASH;
    }
    else if((alStatusCode == ALSTATUSCODE_SMWATCHDOG))
    {
/* ECATCHANGE_START(V5.01) HW3*/
        u8EcatErrorLed = LED_DOUBLEFLASH;
/* ECATCHANGE_END(V5.01) HW3*/
    }
    else
    {
        u8EcatErrorLed = LED_BLINKING;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    alControl        requested new state
 \param alStatusCode    requested status code

 \brief    This function handles the EtherCAT State Machine. It is called
            * in case of an AL Control event (Bit 0 of AL-Event (Reg 0x220),
               when the Master has written the AL Control Register (from ECAT_Main),
              alControl contains the content of the AL Control (Reg 0x120)
            * in case of a SM-Change event (Bit 4 of AL-Event (Reg 0x220)),
              when an Activate SYNCM y register is written by the master (from ECAT_Main),
              alControl contains the actual state (Bit 0-3 of AL Status (Reg 0x130))
            * in case of a locally expired watchdog (from ECAT_Main),
              alControl contains the requested new state (SAFE_OP)
            * in case of an application specific event to change the EtherCAT state (from application),
              alControl contains the requested new state (INIT, PRE_OP or SAFE_OP)

*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::AL_ControlInd(UINT8 alControl, UINT16 alStatusCode)
{
    UINT16        result = 0;
    UINT8            bErrAck = 0;
    UINT8         stateTrans;
    /*deactivate ESM timeout counter*/
    EsmTimeoutCounter = -1;
    bApplEsmPending = TRUE;

    /* reset the Error Flag in case of acknowledge by the Master */
    if ( alControl & STATE_CHANGE )
    {
        bErrAck = 1;
        nAlStatus &= ~STATE_CHANGE;
        /*enable SM2 is moved to state transition block. First check SM Settings.*/
    }
    else if ( (nAlStatus & STATE_CHANGE)
    // HBu 17.04.08: the error has to be acknowledged before when sending the same (or a higher) state
    //               (the error was acknowledged with the same state before independent of the acknowledge flag)
    /*Error Acknowledge with 0xX1 is allowed*/
           && (alControl & STATE_MASK) != STATE_INIT )
        /* the error flag (Bit 4) is set in the AL-Status and the ErrAck bit (Bit 4)
           is not set in the AL-Control, so the state cannot be set to a higher state
           and the new state request will be ignored */
        return;
    else
    {
        nAlStatus &= STATE_MASK;
    }

    /* generate a variable for the state transition
      (Bit 0-3: new state (AL Control), Bit 4-7: old state (AL Status) */
    alControl &= STATE_MASK;
    stateTrans = nAlStatus;
    stateTrans <<= 4;
    stateTrans += alControl;

    /* check the SYNCM settings depending on the state transition */
    switch ( stateTrans )
    {
    case INIT_2_PREOP:
    case OP_2_PREOP:
    case SAFEOP_2_PREOP:
    case PREOP_2_PREOP:
        /* in PREOP only the SYNCM settings for SYNCM0 and SYNCM1 (mailbox)
           are checked, if result is unequal 0, the slave will stay in or
           switch to INIT and set the ErrorInd Bit (bit 4) of the AL-Status */
        result = CheckSmSettings(MAILBOX_READ+1);
        break;
    case PREOP_2_SAFEOP:
        /* before checking the SYNCM settings for SYNCM2 and SYNCM3 (process data)
           the expected length of input data (nPdInputSize) and output data (nPdOutputSize)
            could be adapted (changed by PDO-Assign and/or PDO-Mapping)
            if result is unequal 0, the slave will stay in PREOP and set
            the ErrorInd Bit (bit 4) of the AL-Status */
        result = APPL_GenerateMapping(&nPdInputSize,&nPdOutputSize);
        if (result != 0)
            break;
    case SAFEOP_2_OP:
    case OP_2_SAFEOP:
    case SAFEOP_2_SAFEOP:
    case OP_2_OP:
        /* in SAFEOP or OP the SYNCM settings for all SYNCM are checked
           if result is unequal 0, the slave will stay in or
           switch to PREOP and set the ErrorInd Bit (bit 4) of the AL-Status */
        result = CheckSmSettings(nMaxSyncMan);
        break;
    }

    if ( result == 0 )
    {
        /* execute the corresponding local management service(s) depending on the state transition */
        nEcatStateTrans = 0;
        switch ( stateTrans )
        {
        case INIT_2_BOOT    :
            result = ALSTATUSCODE_BOOTNOTSUPP;
            break;

        case BOOT_2_INIT    :
            result = ALSTATUSCODE_BOOTNOTSUPP;
            break;
        case INIT_2_PREOP :
            {
            /* MBX_StartMailboxHandler (in mailbox.c) checks if the areas of the mailbox
               sync managers SYNCM0 and SYNCM1 overlap each other
              if result is unequal 0, the slave will stay in INIT
              and sets the ErrorInd Bit (bit 4) of the AL-Status */
            result = MBX_StartMailboxHandler();
            if (result == 0)
            {
                bApplEsmPending = FALSE;
                /* additionally there could be an application specific check (in ecatappl.c)
                   if the state transition from INIT to PREOP should be done
                 if result is unequal 0, the slave will stay in INIT
                 and sets the ErrorInd Bit (bit 4) of the AL-Status */
                result = APPL_StartMailboxHandler();
                if ( result == 0 )
                {
                    bMbxRunning = TRUE;
                }
            }

            if(result != 0 && result != NOERROR_INWORK)
            {
                /*Stop APPL Mbx handler if APPL Start Mbx handler was called before*/
                if(!bApplEsmPending)
                    APPL_StopMailboxHandler();

                 MBX_StopMailboxHandler();
            }

            }
            break;

        case PREOP_2_SAFEOP:
            /* start the input handler (function is defined above) */
            result = StartInputHandler();
            if ( result == 0 )
            {
                bApplEsmPending = FALSE;
                result = APPL_StartInputHandler(&u16ALEventMask);

                if(result == 0)
                {
                    /* initialize the AL Event Mask register (0x204) */
                    HW_SetALEventMask( u16ALEventMask );

                    bEcatInputUpdateRunning = TRUE;
                }
            }

            /*if one start input handler returned an error stop the input handler*/
            if(result != 0 && result != NOERROR_INWORK)
            {
                if(!bApplEsmPending)
                {
                    /*Call only the APPL stop handler if the APPL start handler was called before*/
                    /*The application can react to the state transition in the function APPL_StopInputHandler */
/*ECATCHANGE_START(V5.01) ESM1*/
                    APPL_StopInputHandler();
/*ECATCHANGE_END(V5.01) ESM1*/
                }

                StopInputHandler();
            }
            break;

        case SAFEOP_2_OP:
            /* start the output handler (function is defined above) */
            result = StartOutputHandler();
            if(result == 0)
            {
                bApplEsmPending = FALSE;
                result = APPL_StartOutputHandler();

                if(result == 0)
                {
                    /*Device is in OPERATINAL*/
                    bEcatOutputUpdateRunning = TRUE;
                }

            }

            if ( result != 0 && result != NOERROR_INWORK)
            {
                if(!bApplEsmPending)
                    APPL_StopOutputHandler();

                StopOutputHandler();
            }

            break;

        case OP_2_SAFEOP:
            /* stop the output handler (function is defined above) */
/* ECATCHANGE_START(V5.01) ESM2*/
            APPL_StopOutputHandler();
/* ECATCHANGE_END(V5.01) ESM2*/

            StopOutputHandler();
            break;

        case OP_2_PREOP:
            /* stop the output handler (function is defined above) */
/* ECATCHANGE_START(V5.01) ESM2*/
            result = APPL_StopOutputHandler();
/* ECATCHANGE_END(V5.01) ESM2*/

            StopOutputHandler();

            if (result != 0)
                break;

            stateTrans = SAFEOP_2_PREOP;

        case SAFEOP_2_PREOP:
            /* stop the input handler (function is defined above) */
/* ECATCHANGE_START(V5.01) ESM2*/
            APPL_StopInputHandler();
/* ECATCHANGE_END(V5.01) ESM2*/
           
            StopInputHandler();
            break;

        case OP_2_INIT:
            /* stop the output handler (function is defined above) */
/* ECATCHANGE_START(V5.01) ESM2*/
            result = APPL_StopOutputHandler();
/* ECATCHANGE_END(V5.01) ESM2*/

            StopOutputHandler();
            if (result != 0)
                break;
            
            stateTrans = SAFEOP_2_INIT;

        case SAFEOP_2_INIT:
            /* stop the input handler (function is defined above) */
/* ECATCHANGE_START(V5.01) ESM2*/
            result = APPL_StopInputHandler();
/* ECATCHANGE_END(V5.01) ESM2*/
            
            StopInputHandler();
            if (result != 0)
                break;
            stateTrans = PREOP_2_INIT;

        case PREOP_2_INIT:
            MBX_StopMailboxHandler();
            result = APPL_StopMailboxHandler();
            break;

        case INIT_2_INIT:
        case PREOP_2_PREOP:
        case SAFEOP_2_SAFEOP:
        case OP_2_OP:
/*ECATCHANGE_START(V5.01) ESM3*/
            if(bErrAck)
                APPL_AckErrorInd(stateTrans);
/*ECATCHANGE_END(V5.01) ESM3*/

            if(!bLocalErrorFlag)
            {
                /*no local error flag is currently active, enable SM*/
                if ( nAlStatus & (STATE_SAFEOP | STATE_OP))
                {
                    if(nPdOutputSize > 0)
                    {
                        HW_EnableSyncManChannel(PROCESS_DATA_OUT);
                    }
                    else
                    {
                        HW_EnableSyncManChannel(PROCESS_DATA_IN);
                    }
                }
            }
            result = NOERROR_NOSTATECHANGE;
            break;

        case INIT_2_SAFEOP:
        case INIT_2_OP:
        case PREOP_2_OP:
        case PREOP_2_BOOT:
        case SAFEOP_2_BOOT:
        case OP_2_BOOT:
            result = ALSTATUSCODE_INVALIDALCONTROL;
            break;

        default:
            result = ALSTATUSCODE_UNKNOWNALCONTROL;
            break;
        }
    }
    else
    {
        /* the checking of the sync manager settings was not successful
            switch back the state to PREOP or INIT */
        switch (nAlStatus)
        {
        case STATE_OP:
            /* stop the output handler (function is defined above) */
/* ECATCHANGE_START(V5.01) ESM2*/
            APPL_StopOutputHandler();
/* ECATCHANGE_END(V5.01) ESM2*/
            StopOutputHandler();
        case STATE_SAFEOP:
            /* stop the input handler (function is defined above) */
/* ECATCHANGE_END(V5.01) ESM2*/
            APPL_StopInputHandler();
/* ECATCHANGE_END(V5.01) ESM2*/

            StopInputHandler();

        case STATE_PREOP:

            if ( result == ALSTATUSCODE_INVALIDMBXCFGINPREOP )
            {
                /* the mailbox sync manager settings were wrong, switch back to INIT */
                MBX_StopMailboxHandler();
                APPL_StopMailboxHandler();

                nAlStatus = STATE_INIT;
            }
            else
                nAlStatus = STATE_PREOP;
        }
    }

    if ( result == NOERROR_INWORK )
    {
        /* state transition is still in work
            ECAT_StateChange must be called from the application */
        bEcatWaitForAlControlRes = TRUE;
        /* state transition has to be stored */
        nEcatStateTrans = stateTrans;

        /*Init ESM timeout counter (will be decremented with the local 1ms timer)*/
        switch(nEcatStateTrans)
        {
            case INIT_2_PREOP:
            case INIT_2_BOOT:
                EsmTimeoutCounter = PREOPTIMEOUT;
            break;
            case PREOP_2_SAFEOP:
            case SAFEOP_2_OP:
                EsmTimeoutCounter = SAFEOP2OPTIMEOUT;
                break;
           default:
                EsmTimeoutCounter = 200; //Set default timeout value to 200ms
                break;
        }
        EsmTimeoutCounter -= 50; //subtract 50ms from the timeout to react before the master runs into a timeout.

    }
    else
    /* The AL Status Code register shall not be modified if the function is called
       in case of SM change event or an AL-Control event with the same state */
    if ( alControl != (nAlStatus & STATE_MASK) )
    {
        if ( (result != 0 || alStatusCode != 0) && ((alControl | nAlStatus) & STATE_OP) )
        {
            /* the local application requested to leave the state OP so we have to disable the SM2
               and make the state change from OP to SAFEOP by calling StopOutputHandler */

            //only execute StopOutputHandler() if Output update is still running
            if(bEcatOutputUpdateRunning)
            {
/* ECATCHANGE_START(V5.01) ESM2*/
                APPL_StopOutputHandler();
/* ECATCHANGE_END(V5.01) ESM2*/

                StopOutputHandler();
            }

            if(nPdOutputSize > 0)
            {
                /* disable the Sync Manager Channel 2 (outputs) */
                HW_DisableSyncManChannel(PROCESS_DATA_OUT);
            }
            else
            {
                /*disable Sync Manager 3 (inputs) if no outputs available*/
                HW_DisableSyncManChannel(PROCESS_DATA_IN);
            }

        }
        if ( result != 0 )
        {
            if ( nAlStatus == STATE_OP )
                nAlStatus = STATE_SAFEOP;
            /* save the failed status to be able to decide, if the AL Status Code shall be
               reset in case of a coming successful state transition */
            nAlStatus |= STATE_CHANGE;
        }
        else
        {
            /* state transition was successful */
            if ( alStatusCode != 0 )
            {
                /* state change request from the user */
                result = alStatusCode;
                alControl |= STATE_CHANGE;
            }
            /* acknowledge the new state */
            nAlStatus = alControl;
        }

        bEcatWaitForAlControlRes = FALSE;

        /* write the AL Status register */
        SetALStatus(nAlStatus, result);
    }
    else
    {
         bEcatWaitForAlControlRes = FALSE;

        /* AL-Status has to be updated and AL-Status-Code has to be reset
           if the the error bit was acknowledged */
        SetALStatus(nAlStatus, 0);
    }

}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function is called cyclic if a state transition is pending (bEcatWaitForAlControlRes == TRUE)
 \brief    If the ESM timeout is expired the state transition will be rejected. Otherwise the application specific state transition function is called.
 \brief    If the pending state transition is triggered by the application the transition need to be completed by the application (ECAT_StateChange())
  *////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::AL_ControlRes(void)
{
    if(bEcatWaitForAlControlRes)
    {
        UINT16 result = 0;
        UINT8 Status = 0;
        UINT16 StatusCode = 0;

        if(EsmTimeoutCounter == 0)
        {
            Status =  (UINT8)(nEcatStateTrans >> 4);

            /* ESM timeout expired*/
            switch(nEcatStateTrans)
            {
                case INIT_2_PREOP:
                case INIT_2_BOOT:

                    if(!bApplEsmPending)
                        APPL_StopMailboxHandler();

                    MBX_StopMailboxHandler();
                    if(bLocalErrorFlag)
                    {
                        /*Set application specified error*/
                        StatusCode = u16LocalErrorCode;
                    }
                    else
                    {
                        /*Set unspecified error*/
                        StatusCode = ALSTATUSCODE_UNSPECIFIEDERROR;
                    }
                break;
                case PREOP_2_SAFEOP:
                    if(!bApplEsmPending)
                        APPL_StopInputHandler();

                    StopInputHandler();
                    
                    if(bLocalErrorFlag)
                    {
                        /*Set application specified error*/
                        StatusCode = u16LocalErrorCode;
                    }
                    else
                    {
                        /*Set unspecified error*/
                        StatusCode = ALSTATUSCODE_UNSPECIFIEDERROR;
                    }
                break;
                case SAFEOP_2_OP:
                    if(bDcSyncActive)
                    {
                        /*SafeOP to OP timeout expired check which AL status code need to be written*/
                        if(!bDcRunning)
                        {
                            /*no Sync0 signal received*/
                            StatusCode = ALSTATUSCODE_NOSYNCERROR;
                        }
//                        else if(!bEcatFirstOutputsReceived)	   //??
//                        {
                            /*no process data received*/
//                            StatusCode = ALSTATUSCODE_SMWATCHDOG;
//                        }
                        else
                        {
                            /*Pll is not valid*/
                            StatusCode = ALSTATUSCODE_DCPLLSYNCERROR;
                        }
                    }
                    else
                    {
                        StatusCode = ALSTATUSCODE_SMWATCHDOG;
                    }

                    /*Stop handler on failed transition*/
                    if(StatusCode != 0)
                    {
                        if(!bApplEsmPending)
                            APPL_StopOutputHandler();

                        StopOutputHandler();
                    }
                break;
            }
        } //ESM timeout
        else
        {
            /*Call application specific transition function and complete transition it the function returns 0*/
            switch(nEcatStateTrans)
            {
                case INIT_2_PREOP:
                case INIT_2_BOOT:
                    if(bApplEsmPending)
                    {
                        bApplEsmPending = FALSE;
                        /*APPL_StartMailboxHandler() need to be called*/
                        result = APPL_StartMailboxHandler();

                        if(result == 0)
                        {
                            /*The application specific transition was successful => set active mailbox handler indication*/
                            bMbxRunning = TRUE;
                            Status =  (UINT8)(nEcatStateTrans & STATE_MASK);
                        }
                        else
                        {
                            /*The application specific transition failed.
                            (In pending case the application need to complete the transition)*/

                            if(result != NOERROR_INWORK)
                            {
                                APPL_StopMailboxHandler();
                                MBX_StopMailboxHandler();
                            }
                        }
                    }
                break;
                case PREOP_2_SAFEOP:
                    if(bApplEsmPending)
                    {
                        bApplEsmPending = FALSE;
                        result = APPL_StartInputHandler(&u16ALEventMask);

                        if(result == 0)
                        {
                            bEcatInputUpdateRunning = TRUE;
                            Status = STATE_SAFEOP;
                        }
                        else
                        {
                            /*The application specific transition failed.
                            (In pending case the application need to complete the transition)*/

                            if(result != NOERROR_INWORK)
                            {
                                APPL_StopInputHandler();
                                StopInputHandler();
                            }
                        }
                    }
                break;
                case SAFEOP_2_OP:
                   if(bApplEsmPending)
                    {
                        if(bDcSyncActive)
                        {
                            if(i16WaitForPllRunningTimeout > 0 && i16WaitForPllRunningTimeout <= i16WaitForPllRunningCnt)
                            {
                                /*Pll sequence valid for 200ms (set in APPL_StartOutputHandler() )
                                acknowledge state transition to OP */

                                i16WaitForPllRunningTimeout = 0;
                                i16WaitForPllRunningCnt = 0;

                                result = APPL_StartOutputHandler();

                                if(result == 0)
                                {
                                    /* Slave is OPERATIONAL */
                                    bEcatOutputUpdateRunning = TRUE;
                                    Status = STATE_OP;
                                }
                                else
                                {
                                    if(result != NOERROR_INWORK)
                                    {
                                        APPL_StopOutputHandler();
                                        StopOutputHandler();
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(nPdOutputSize == 0 || bEcatFirstOutputsReceived)
                            {
                                result = APPL_StartOutputHandler();

                                if(result == 0)
                                {
                                    /* Slave is OPERATIONAL */
                                    bEcatOutputUpdateRunning = TRUE;
                                    Status = STATE_OP;
                                }
                                else
                                {
                                    if(result != NOERROR_INWORK)
                                    {
                                        APPL_StopOutputHandler();
                                        StopOutputHandler();
                                    }
                                }
                            }
                        }       
                    }             
                break;
            }//Switch - transition
        }

        if(Status != 0)
        {
            /*Pending state transition finished => write AL Status and AL Status Code*/
            bEcatWaitForAlControlRes = FALSE;

            if(StatusCode != 0)
                Status |= STATE_CHANGE;

            SetALStatus(Status,StatusCode);
        }
    }// Pending state transition (bEcatWaitForAlControlRes == true)
}


/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function checks the current Sync state and set the local flags
 The analyse of the local flags is handled in "CheckIfEcatError"

*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::DC_CheckWatchdog(void)
{
    if(bDcSyncActive)
    {
        /*Check the Sync0 cycle if Sync0 Wd is enabled*/
        if(Sync0WdValue > 0)
        {
            Sync0WdCounter ++;
            if(Sync0WdCounter > Sync0WdValue)
            {
                /*Sync0 watchdog expired*/
                bDcRunning = FALSE;
            }
        }

        if(bDcRunning)
        {
           if(sSyncManOutPar.u32SmEventMissedCounter < sErrorSettings.u32SyncErrorCounterLimit)
            {
                bPllRunning = TRUE;

                /*Wait for PLL is active increment the Pll valid counter*/
                if(i16WaitForPllRunningTimeout > 0)
                {
                    i16WaitForPllRunningCnt++;
                }
            }
            else
            {
                bPllRunning = FALSE;
                sSyncManOutPar.u16SyncError = 1;

                /*Wait for PLL is active reset the Pll valid counter*/
                if(i16WaitForPllRunningTimeout > 0)
                {
                    i16WaitForPllRunningCnt = 0;
                }
            }
        }
        else
        {
            bPllRunning = FALSE;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    Checks communication and synchronisation variables and update AL status / AL status code if an error has occurred

*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::CheckIfEcatError(void)
{
if(bDcSyncActive)
{
    if(bEcatOutputUpdateRunning)
    {
        /*Slave is in OP state*/
        if(!bDcRunning)
        {
            AL_ControlInd(STATE_SAFEOP, ALSTATUSCODE_FATALSYNCERROR);
            return;
        }
//CX: for input modules, donot check PllRunning
//CX: uncommnet the following lines for DO & AO
/*
        else if(!bPllRunning)
        {
            AL_ControlInd(STATE_SAFEOP, ALSTATUSCODE_DCPLLSYNCERROR);
            return;
        }
*/        
    }
}

        /*if the watchdog is enabled check the the process data watchdog in the ESC
        and set the AL status code 0x1B if the watchdog expired*/
        if ( EcatWdValue != 0 )
        {
            /*watchdog time is set => watchdog is active*/
            UINT16 WdStatusOK = 0;

            HW_EscReadWord(WdStatusOK,ESC_PD_WD_STATE);
/* ECATCHANGE_START(V5.01) ESC2*/
            WdStatusOK = SWAPWORD(WdStatusOK);
/* ECATCHANGE_END(V5.01) ESC2*/
            if(!(WdStatusOK & ESC_PD_WD_TRIGGER_MASK))
            {
                /*The device is in OP state*/

                if(bEcatOutputUpdateRunning
                )
                {
                    AL_ControlInd(STATE_SAFEOP,ALSTATUSCODE_SMWATCHDOG);
                    return;
                }
                else
                {
                    bEcatFirstOutputsReceived = FALSE;
                }
            }
        }


}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    alStatus: requested state
 \param    alStatusCode: value for the AL-Status register
 
 \brief    This function changes the state of the EtherCAT slave if the requested state
             is lower than the actual state, otherwise the error condition will be reset.
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::ECAT_StateChange(UINT8 alStatus, UINT16 alStatusCode)
{
    UINT8 Status = alStatus;

    if(bEcatWaitForAlControlRes)
    {
        /*State transition is pending*/

        if(bApplEsmPending)
        {
            /*The generic stack has currently control of the state transition.
            In case on an local error force ESM timeout*/
            if(alStatusCode != 0)
            {
                bLocalErrorFlag = TRUE;
                u16LocalErrorCode = alStatusCode;
                EsmTimeoutCounter = 0;
            }
        }
        else
        {
            /*complete the state transition*/

            if(alStatusCode != 0)
            {
                bLocalErrorFlag = TRUE;
                u16LocalErrorCode = alStatusCode;

                /*State transition failed due to local application reasons*/
                switch(nEcatStateTrans)
                {
                    case INIT_2_PREOP:
                    case INIT_2_BOOT:
                          APPL_StopMailboxHandler();
                          MBX_StopMailboxHandler();
                    break;
                    case PREOP_2_SAFEOP:
                          APPL_StopInputHandler();
                          StopInputHandler();
                    break;
                    case SAFEOP_2_OP:
                          APPL_StopOutputHandler();
                          StopOutputHandler();
                    break;
                }

                /*In case of a failed state transition the */
                Status =  (UINT8)(nEcatStateTrans >> 4);
            }
            else
            {
                /*State transition succeed*/
                 
                switch(nEcatStateTrans)
                {
                    case INIT_2_PREOP:
                    case INIT_2_BOOT:
                        bMbxRunning = TRUE;
                    break;
                    case PREOP_2_SAFEOP:
                        bEcatInputUpdateRunning = TRUE;
                    break;
                    case SAFEOP_2_OP:
                          bEcatOutputUpdateRunning = TRUE;
                    break;
                }

                /*In case of a failed state transition the */
                Status =  (UINT8)(nEcatStateTrans & STATE_MASK);
            }
                /*Pending state transition finished => write AL Status and AL Status Code*/
                bEcatWaitForAlControlRes = FALSE;

                if(alStatusCode != 0)
                    Status |= STATE_CHANGE;

                SetALStatus(Status,alStatusCode);

        }// state transition need to be completed by the local application
    }//State transition pending
    else
    {
        if ( alStatusCode != 0 )
        {
            /* Local error has happened, we change the state if necessary */
            bLocalErrorFlag = TRUE;
            u16LocalErrorCode = alStatusCode;
    
            if ( (alStatus & STATE_MASK) < (nAlStatus & STATE_MASK) )
            {
                AL_ControlInd(alStatus, alStatusCode);
            }
        }
        else if (bLocalErrorFlag)
        {
            /*a local error is gone */
            if ( (nAlStatus & STATE_MASK) == (STATE_SAFEOP | STATE_OP) )
            {
                if(nPdOutputSize > 0)
                {
                    /* we have to enable the output process data SyncManger (default: SM2),
                    because it was disabled when switching back to SAFE-OP */
                    HW_EnableSyncManChannel(PROCESS_DATA_OUT);
                }
                else if (nPdInputSize > 0)
                {
                    /* we have to enable the input process data SyncManger (default: SM3),
                    because it was disabled when switching back to SAFE-OP */
                    HW_EnableSyncManChannel(PROCESS_DATA_IN);
                }
            }
            bLocalErrorFlag = FALSE;
            u16LocalErrorCode = 0x00;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function initialize the EtherCAT Slave Interface.
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::ECAT_Init(void)
{
    UINT8 i;
    /*Get Maximum Number of SyncManagers*/
    UINT16 TmpVar = 0;

    HW_EscReadWord(TmpVar, ESC_COMM_INFO_OFFSET);

    TmpVar = SWAPWORD(TmpVar);
    nMaxSyncMan = (UINT8) ((TmpVar & ESC_SM_CHANNELS_MASK)>> ESC_SM_CHANNELS_SHIFT);

    /* disable all Sync Manager channels */
    for (i = 0; i < nMaxSyncMan; i++)
        HW_DisableSyncManChannel(i);

    /* initialize the mailbox handler */
    MBX_Init();

    /* initialize variables */
    bApplEsmPending = FALSE;
    bEcatWaitForAlControlRes = FALSE;
    bEcatFirstOutputsReceived = FALSE;
     bEcatOutputUpdateRunning = FALSE;
     bEcatInputUpdateRunning = FALSE;
    bWdTrigger = FALSE;
    EcatWdValue = 0;
    Sync0WdCounter = 0;
    Sync0WdValue = 0;
    bDcSyncActive = FALSE;
    bLocalErrorFlag = FALSE;
    u16LocalErrorCode = 0x00;

    u16ALEventMask = 0;

    /* initialize the AL Status register */
    nAlStatus    = STATE_INIT;
    SetALStatus(nAlStatus, 0);
    nEcatStateTrans = 0;
    u8EcatErrorLed = LED_OFF;

    bEscIntEnabled = FALSE;
    /* initialize the COE part */
    COE_Init();
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief        This function has to be called cyclically.
*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::ECAT_Main(void)
{
    UINT16 ALEventReg;
    UINT16 EscAlControl = 0x0000;
    UINT16 sm1Activate = SM_SETTING_ENABLE_VALUE;

    /* check if services are stored in the mailbox */
    MBX_Main();


    if ( bMbxRunning )
    {
        /* Slave is at least in PREOP, Mailbox is running */
        /* get the Activate-Byte of SM 1 (Register 0x80E) to check if a mailbox repeat request was received */
        HW_EscReadWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
        sm1Activate = SWAPWORD(sm1Activate);
    }

    /* Read AL Event-Register from ESC */
    ALEventReg = HW_GetALEventRegister();
    ALEventReg = SWAPWORD(ALEventReg);
	
    if ((ALEventReg & AL_CONTROL_EVENT) && !bEcatWaitForAlControlRes)
    {
        /* AL Control event is set, get the AL Control register sent by the Master to acknowledge the event
          (that the corresponding bit in the AL Event register will be reset) */
        HW_EscReadWord( EscAlControl, ESC_AL_CONTROL_OFFSET);
        EscAlControl = SWAPWORD(EscAlControl);

        /* reset AL Control event and the SM Change event (because the Sync Manager settings will be checked
           in AL_ControlInd, too)*/
        ALEventReg &= ~((AL_CONTROL_EVENT) | (SM_CHANGE_EVENT));

        AL_ControlInd((UINT8)EscAlControl, 0); /* in AL_ControlInd the state transition will be checked and done */
        
        /* SM-Change-Event was handled too */
    }

    if ( (ALEventReg & SM_CHANGE_EVENT) && !bEcatWaitForAlControlRes && (nAlStatus & STATE_CHANGE) == 0 && (nAlStatus & ~STATE_CHANGE) != STATE_INIT )
    {
        /* the SM Change event is set (Bit 4 of Register 0x220), when the Byte 6 (Enable, Lo-Byte of Register 0x806, 0x80E, 0x816,...)
           of a Sync Manager channel was written */
        ALEventReg &= ~(SM_CHANGE_EVENT);

        /* AL_ControlInd is called with the actual state, so that the correct SM settings will be checked */
        AL_ControlInd(nAlStatus & STATE_MASK, 0);
    }

    if(bEcatWaitForAlControlRes)
    {
        AL_ControlRes();
    }
    /*The order of mailbox event processing was changed to prevent race condition errors.
        The SM1 activate Byte (Register 0x80E) was read before reading AL Event register.
        1. Handle Mailbox Read event
        2. Handle repeat toggle request
        3. Handle Mailbox write event
    */
    if ( bMbxRunning )
    {
        /*SnycManger change event (0x220:4) could be acknowledged by reading the SM1 control register without notification to the local application
        => check if the SyncManger 1 is still enabled*/
        if(!(sm1Activate & SM_SETTING_ENABLE_VALUE))
            AL_ControlInd(nAlStatus & STATE_MASK, 0);

        if ( ALEventReg & (MAILBOX_READ_EVENT) )
        {
            /* SM 1 (Mailbox Read) event is set, when the mailbox was read from the master,
               to acknowledge the event the first byte of the mailbox has to be written,
               by writing the first byte the mailbox is locked, too */
            u16dummy = 0;
            HW_EscWriteWord(u16dummy,u16EscAddrSendMbx);

            /* the Mailbox Read event in the variable ALEventReg shall be reset before calling
               MBX_MailboxReadInd, where a new mailbox datagram (if available) could be stored in the send mailbox */
            ALEventReg &= ~(MAILBOX_READ_EVENT);
            MBX_MailboxReadInd();
        }

        DISABLE_MBX_INT;
        /* bMbxRepeatToggle holds the last state of the Repeat Bit (Bit 1) */

        if ( ( (sm1Activate & SM_SETTING_REPAET_REQ_MASK) && !bMbxRepeatToggle )
            ||( !(sm1Activate & SM_SETTING_REPAET_REQ_MASK) && bMbxRepeatToggle ))
        {
            /* Repeat Bit (Bit 1) has toggled, there is a repeat request, in MBX_MailboxRepeatReq the correct
               response will put in the send mailbox again */
            MBX_MailboxRepeatReq();
            /* acknowledge the repeat request after the send mailbox was updated by writing the Repeat Bit
               in the Repeat Ack Bit (Bit 1) of the PDI Ctrl-Byte of SM 1 (Register 0x80F) */
            if(bMbxRepeatToggle)
                sm1Activate |= SM_SETTING_REPEAT_ACK; //set repeat acknowledge bit (bit 9)
            else
                sm1Activate &= ~SM_SETTING_REPEAT_ACK; //clear repeat acknowledge bit (bit 9)

            sm1Activate = SWAPWORD(sm1Activate);
            HW_EscWriteWord(sm1Activate,(ESC_SYNCMAN_ACTIVE_OFFSET + SIZEOF_SM_REGISTER));
        }
        ENABLE_MBX_INT;

        /* Reload the AlEvent because it may be changed due to a SM disable, enable in case of an repeat request */
        ALEventReg = HW_GetALEventRegister();
        ALEventReg = SWAPWORD(ALEventReg);

        if ( ALEventReg & (MAILBOX_WRITE_EVENT) )
        {
            /* SM 0 (Mailbox Write) event is set, when the mailbox was written from the master,
               to acknowledge the event the first byte of the mailbox has to be read,
               which will be done in MBX_CheckAndCopyMailbox */
            /* the Mailbox Write event in the variable ALEventReg shall be reset before calling
               MBX_CheckAndCopyMailbox, where the received mailbox datagram will be processed */
            ALEventReg &= ~(MAILBOX_WRITE_EVENT);
            MBX_CheckAndCopyMailbox();
        }

    }
}


/** @} */


