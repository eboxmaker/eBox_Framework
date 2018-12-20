/**
\defgroup ecatappl ecatappl: EtherCAT Slave - application interface
\brief This file contains the EtherCAT State Machine and Process Data interface.\n
\brief Changes to version V5.0:
\brief V5.01 APPL3: Include library demo application
\brief V5.01 ESC1: Change ESC access function (if EEPROM Emulation is active)
\brief V5.01 ESC2: Add missed value swapping
\brief Changes to version V4.40:
\brief V5.0 TEST1: Add test application. See Application Note ET9300 for more details.
\brief V5.0 ECAT2: Application specific functions are moved to application files.
\brief V5.0 ECAT3: Global dummy variables used for dummy ESC operations.
\brief V5.0 ESC1: ESC 32Bit Access added.
\brief V5.0 ESC3: Add EEPROM emulation support.
\brief V5.0 ESM3: Handling pending ESM transitions.
\brief V5.0 ESC5: Enhance EEPROM access handling.
\brief V5.0 PDO1: AL Event flags are not rechecked in PDO_OutputMappping(). (Already checked before call function)
\brief V5.0 SYNC1: Add missed SM event indication (0x1C32/0x1C33 SI11).
\brief Changes to version V4.30:\n
\brief V4.40 DIAG1: add diagnosis message support
\brief V4.40 PDO1: merge content of HW_InputMapping (spihw.c/mcihw.c) to PDO_InputMapping. merge content of HW_OutputMapping (spihw.c/mcihw.c) to PDO_OutputMapping.
\brief V4.40 PDO2: Generic process data length calculation
\brief V4.40 ECAT2: call cyclic CheckIfLocalError() to check the local flags
\brief V4.40 HW0: Generic hardware access functions. Add Function (PDI_Isr()), content merged from spihw.c and mcihw.c.
\brief V4.40 WD1: define (ESC_SM_WD_SUPPORTED) to choose ESC SyncManager watchdog or local watchdog
\brief V4.40 ESM2: Change state transition behaviour from SafeOP to OP
\brief V4.40 TIMER1: Change bus cycle time calculation and trigger of ECAT_CheckTimer() if ECAT_TIMER_INT is reset
\brief V4.40 HW1: Add support for fc1100 hardware
\brief Changes to version V4.20:\n
\brief V4.30 EL9800: EL9800_x cyclic application is moved to el9800.c
\brief V4.30 OBJ 3:    add object dictionary initialization
\brief V4.30 SYNC: add CalcSMCycleTime() (calculation of bus cycle time); change synchronisation control functions
\brief V4.30 PDO: include PDO specific functions (moved from coeappl.c).
\brief                xxx_InputMapping(); xxx_OutputMapping(); xxx_ReadInputs(); xxx_ResetOutputs(); xxx_Application()
\brief V4.30 CiA402: Add CiA402_StateMachine() and CiA402_Application() call
\brief V4.20 DC 1: Add DC pending Statemachine handling
\brief V4.20 PIC24: Add EL9800_4 (PIC24) required source code
\brief V4.20 LED 1: Modified LED Handling
\brief V4.11 APPL 1: The checkWatchdog() function should not called in checkTimer() if this function is triggered by an Interrupt\n
\brief Changes to version V4.08:\n
\brief V4.10 LED 1: The handling of the EtherCAT-Error-LED was added\n
\brief V4.10 AOE 3: The AoE fragment size has to be initialized during the state transition
\brief                  from INIT to PREOP\n
\brief Changes to version V4.07:\n
\brief V4.08 LED 1: The handling of the EtherCAT-LED can be (de-)selected by the switch LEDS_SUPPORTED
\brief                  because the ET1100 and ET1200 have an output port which could be connected directly.\n
\brief Changes to version V4.01:\n
\brief V4.02 ECAT 1: The watchdog timer variables shall be initialized.\n
\brief Changes to version V4.00:\n
\brief V4.01 APPL 1: If the application is running in synchron mode and no SM event
\brief               is received, the application should be called from the main loop\n
\brief V4.01 APPL 2: In FreeRun mode the output should only be copied if the slave is in OP\n
\brief Changes to version V3.20:\n
\brief V4.00 APPL 1: The watchdog checking should be done by a microcontroller
\brief                  timer because the watchdog trigger of the ESC will be reset too
\brief                  if only a part of the sync manager data is written\n
\brief V4.00 APPL 2: The setting of EtherCAT state LEDs were included\n
\brief V4.00 APPL 3: The outputs should be reset to a safe state,
\brief                    when the state OP is left\n
\brief V4.00 APPL 4: An example for the EEPROM access through the ESC is shown in
\brief                    the function APPL_StartMailboxHandler\n
\brief V4.00 APPL 5: The inputs should be read once when the state transition
\brief                    from PREOP to SAFEOP is made\n
\brief V4.00 APPL 6: The main function was split in MainInit and MainLoop\n


\version 5.01
*/

//---------------------------------------------------------------------------------------
/**
\ingroup ecatappl
\file ecatappl.c
\brief Implementation.
*/
//---------------------------------------------------------------------------------------

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/

//#define    _ECATAPPL_ 1
//#include "ecatappl.h"
//#undef _ECATAPPL_
//#define    _ECATAPPL_ 0

//#include "objdef.h"
//#include "ecatslv.h"
//#include "el9800appl.h"
#include "ethercat.h"



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


/*variables are declared in ecatslv.c*/ //move to class var
//    extern VARVOLATILE UINT16    u16dummy;
/*-----------------------------------------------------------------------------------------
------
------    local functions
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    Functions
------
-----------------------------------------------------------------------------------------*/

/**
\addtogroup ecatappl
@{
*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief      This function will copies the inputs from the local memory to the ESC memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::PDO_InputMapping()
{
    APPL_InputMapping((UINT16*)aPdInputData);
    HW_EscWriteIsr(((MEM_ADDR *) aPdInputData), nEscAddrInputData, nPdInputSize );
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will copies the outputs from the ESC memory to the local memory
          to the hardware. This function is only called in case of an SM2 
          (output process data) event.
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::PDO_OutputMapping()
{

    HW_EscReadIsr(((MEM_ADDR *)aPdOutputData), nEscAddrOutputData, nPdOutputSize );

    APPL_OutputMapping((UINT16*) aPdOutputData);
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function shall be called every 1ms.
 \brief If the switch ECAT_TIMER_INT is 0, the watchdog control is implemented without using
 \brief interrupts. In this case a local timer register is checked every ECAT_Main cycle
 \brief and the function is triggered if 1 ms is elapsed
 *////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::ECAT_CheckTimer(void)
{
    if(sSyncManOutPar.u32CycleTime == 0)
    {
        u16BusCycleCntMs++;
    }

    /*decrement the state transition timeout counter*/
    if(bEcatWaitForAlControlRes &&  (EsmTimeoutCounter > 0))
    {
        EsmTimeoutCounter--;
    }

    ECAT_SetLedIndication();

    DC_CheckWatchdog();
}

void Ethercat::PDI_Isr(void)
{
    /* get the AL event register */
    UINT16  ALEvent = HW_GetALEventRegister_Isr();
    ALEvent = SWAPWORD(ALEvent);

    if ( ALEvent & PROCESS_OUTPUT_EVENT )
    {
        if(bDcSyncActive && bDcRunning)
        {
                bSmSyncToggle = FALSE;
        }
        if(sSyncManOutPar.u32SmEventMissedCounter > 0)
            sSyncManOutPar.u32SmEventMissedCounter--;

        /*calculate the cycle time if device is in SM Sync mode and Cycle time was not calculated yet*/
        if ( !bDcSyncActive && bEscIntEnabled)
        {
            if(sSyncManOutPar.u16GetCycleTime == 1)
            {
                /*get bus cycle time triggered */
                sSyncManOutPar.u32CycleTime = 0;
                sSyncManInPar.u32CycleTime  = 0;
                u16BusCycleCntMs = 0;
                bCycleTimeMeasurementStarted = TRUE;
                sSyncManOutPar.u16GetCycleTime = 0;
                StartTimerCnt = (UINT32) HW_GetTimer();
            }
            else
            {
                if(bCycleTimeMeasurementStarted == TRUE)
                {
                    UINT32 CurTimerCnt = (UINT32)HW_GetTimer();

#if ECAT_TIMER_INC_P_MS
                    sSyncManOutPar.u32CycleTime = (UINT32)u16BusCycleCntMs * 1000000 + (((INT32)(CurTimerCnt-StartTimerCnt))*1000000/ECAT_TIMER_INC_P_MS);    //get elapsed cycle time in ns
#else
                    sSyncManOutPar.u32CycleTime = 0;
#endif
                    sSyncManInPar.u32CycleTime  = sSyncManOutPar.u32CycleTime ;
                    u16BusCycleCntMs = 0;
                    StartTimerCnt = 0;
                    bCycleTimeMeasurementStarted = FALSE;


                /* CiA402 Motion controller cycle time is only set if DC Synchronisation is active*/
                }
            }
        }
        /* Outputs were updated, set flag for watchdog monitoring */
        bEcatFirstOutputsReceived = TRUE;
        if ( bEcatOutputUpdateRunning )
        {
            /* slave is in OP, update the outputs */
            PDO_OutputMapping();
        }
        else
        {
        HW_EscReadWordIsr(u16dummy,nEscAddrOutputData);
        HW_EscReadWordIsr(u16dummy,(nEscAddrOutputData+nPdOutputSize-2));
        }
    }

    /*if DC Mode is enabled ECAT_Application is called from Sync0Isr*/

    if ( !bDcSyncActive && bEscIntEnabled )    //only set in SM Sync mode
    {
        /* ECAT synchron Mode is active */
        if ( ALEvent & (PROCESS_OUTPUT_EVENT | PROCESS_INPUT_EVENT) )
        {
            /* Application is synchronized to SM2-event (output size > 0)
                or SM3-event (output size = 0, in that case PDO_InputMapping will be called
                from ECAT_Application to acknowledge the SM3-event) */
            ECAT_Application();
        }
    }
    /*if next SM event was triggered during runtime increment cycle exceed counter*/
    ALEvent = HW_GetALEventRegister_Isr();
    ALEvent = SWAPWORD(ALEvent);

    if ( ALEvent & PROCESS_OUTPUT_EVENT )
    {
        sSyncManOutPar.u32CycleExceededCounter++;
        sSyncManInPar.u32CycleExceededCounter = sSyncManOutPar.u32CycleExceededCounter;
    }
}

void Ethercat::Sync0_Isr(void)
{
    bDcRunning = TRUE;
    Sync0WdCounter = 0;

    /*if SM Sync toggle is true no SM event received between two Sync events*/
    if(bSmSyncToggle)
    {
        sSyncManOutPar.u32SmEventMissedCounter = sSyncManOutPar.u32SmEventMissedCounter + 3;
    }
    bSmSyncToggle = TRUE;

    if(bDcSyncActive)
    {
        /* Application is synchronized to SYNC0 event*/
        ECAT_Application();
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function shall called within a 1ms cycle.
        Set Run and Error Led depending on the Ledstate

*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::ECAT_SetLedIndication(void)
{
    static UINT16 ms = 0;
    static UINT16 RunCounter = 0;
    static UINT16 ErrorCounter = 0;

    static UINT8 u8PrevErrorLed = LED_OFF ;
    static UINT8 u8PrevRunLed = LED_OFF ;

    // this code should be called every ms in average
    if ( bEcatOutputUpdateRunning )
    {
        // in OP the EtherCAT state LED is always 1 and ErrorLED is 0
        bEtherCATRunLed = TRUE;
        bEtherCATErrorLed = FALSE;
    }
    else
    {
        ms++;
        if(ms == 50 || ms == 100 ||ms == 150 ||ms == 200)    //set flickering LED if required
        {
            /*Set run Led State*/
            switch ( nAlStatus & STATE_MASK)
            {
            case STATE_INIT:
                // in INIT the EtherCAT state LED is off
                u8EcatRunLed = LED_OFF;
                break;
            case STATE_PREOP:
                // in PREOP the EtherCAT state LED toggles every 200 ms
                u8EcatRunLed = LED_BLINKING;
                break;
            case STATE_SAFEOP:
                // in SAFEOP the EtherCAT state LED is 200 ms on and 1s off
                u8EcatRunLed = LED_SINGLEFLASH;
                break;
            case STATE_OP:
                u8EcatRunLed = LED_ON;
                break;
            case STATE_BOOT:
                u8EcatRunLed = LED_FLICKERING;
                break;
            default:
                u8EcatRunLed = LED_OFF;
            break;
            }//switch nAlStatus

            /*Calculate current Run LED state*/
            if(u8EcatRunLed & 0x20 || ms == 200)    //if fast flag or slow cycle event
            {
                UINT8 NumFlashes = (u8EcatRunLed & 0x1F)+((u8EcatRunLed & 0x1F)-1);    //total number
                /*generate LED code*/
                if(u8EcatRunLed != u8PrevRunLed)    //state changed start with active LED
                {
                    if(u8EcatRunLed & 0x80)    //invert flag enable?
                            bEtherCATRunLed = FALSE;
                    else
                        bEtherCATRunLed = TRUE;

                    RunCounter = 1;
                }
                else    //second and following LED cycle
                {
                    if(u8EcatRunLed & 0x40)    //toggle LED bit on
                    {
                        bEtherCATRunLed = !bEtherCATRunLed;

                        if(NumFlashes)    //NumFlashes defined => limited LED toggle
                        {
                            RunCounter++;

                            if(RunCounter > NumFlashes)    //toggle led finished
                            {
                                if(u8EcatRunLed & 0x80)    //invert flag enable?
                                    bEtherCATRunLed = TRUE;
                                else
                                    bEtherCATRunLed = FALSE;

                                if(RunCounter > (NumFlashes+5))        //toggle time + 5 cycles low
                                    RunCounter = 0;
                            }
                        }
                    }
                    else
                        bEtherCATRunLed = (u8EcatRunLed & 0x01);
                }
                u8PrevRunLed = u8EcatRunLed;
            }

            /*Calculate current Error LED state*/
            if(u8EcatErrorLed & 0x20 || ms == 200)    //if fast flag or slow cycle event
            {
                UINT8 NumFlashes = (u8EcatErrorLed & 0x1F)+((u8EcatErrorLed & 0x1F)-1);    //total number
                /*generate LED code*/
                if(u8EcatErrorLed != u8PrevErrorLed)    //state changed start with active LED
                {
                    if(u8EcatErrorLed & 0x80)    //invert flag enable?
                        bEtherCATErrorLed = FALSE;
                    else
                        bEtherCATErrorLed = TRUE;

                    ErrorCounter = 1;
                }
                else    //second and following LED cycle
                {
                    if(u8EcatErrorLed & 0x40)    //toggle LED bit on
                    {
                        bEtherCATErrorLed = !bEtherCATErrorLed;

                        if(NumFlashes)    //NumFlashes defined => limited LED toggle
                        {
                            ErrorCounter++;

                            if(ErrorCounter > NumFlashes)    //toggle led finished
                            {
                                if(u8EcatErrorLed & 0x80)    //invert flag enable?
                                    bEtherCATErrorLed = TRUE;
                                else
                                    bEtherCATErrorLed = FALSE;

                                if(ErrorCounter > (NumFlashes+5))        //toggle time + 5 cycles low
                                    ErrorCounter = 0;
                            }
                        }
                    }
                    else
                        bEtherCATErrorLed = (u8EcatErrorLed & 0x01);
                }

                u8PrevErrorLed = u8EcatErrorLed;
            }

            if(ms == 200)
                ms = 0;
        }
    }
    /* set the EtherCAT-LED */
    HW_SetLed(((UINT8)bEtherCATRunLed),((UINT8)bEtherCATErrorLed));
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pObjectDictionary   Pointer to application specific object dictionary.
                                NULL if no specific object are available.
\return     0 if initialization was successful

 \brief    This function initialize the EtherCAT Sample Code

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 Ethercat::MainInit()
{
    UINT16 Error = 0;
/*Hardware init function need to be called from the application layer*/
    /* initialize the EtherCAT Slave Interface */
    ECAT_Init();
    /* initialize the objects */
    COE_ObjInit();


    /*Timer initialization*/
    u16BusCycleCntMs = 0;
    StartTimerCnt = 0;
    bCycleTimeMeasurementStarted = FALSE;

/*Application Init need to be called from the application layer*/
     return Error;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function shall be called cyclically from main

*////////////////////////////////////////////////////////////////////////////////////////

void Ethercat::MainLoop(void)
{
        /* FreeRun-Mode:  bEscIntEnabled = FALSE, bDcSyncActive = FALSE
           Synchron-Mode: bEscIntEnabled = TRUE, bDcSyncActive = FALSE
           DC-Mode:       bEscIntEnabled = FALSE, bDcSyncActive = TRUE */
        if ( (!bEscIntEnabled || !bEcatFirstOutputsReceived)     /* SM-Synchronous, but not SM-event received */
          && !bDcSyncActive                                               /* DC-Synchronous */
            )
        {
            /* if the application is running in ECAT Synchron Mode the function ECAT_Application is called
               from the ESC interrupt routine (in mcihw.c or spihw.c),
               in ECAT Synchron Mode it should be additionally checked, if the SM-event is received
               at least once (bEcatFirstOutputsReceived = 1), otherwise no interrupt is generated
               and the function ECAT_Application has to be called here (with interrupts disabled,
               because the SM-event could be generated while executing ECAT_Application) */
            if ( !bEscIntEnabled )
            {
                /* application is running in ECAT FreeRun Mode,
                   first we have to check, if outputs were received */
                UINT16 ALEvent = HW_GetALEventRegister();
                ALEvent = SWAPWORD(ALEvent);

                if ( ALEvent & PROCESS_OUTPUT_EVENT )
                {
                    /* set the flag for the state machine behaviour */
                    bEcatFirstOutputsReceived = TRUE;
                    if ( bEcatOutputUpdateRunning )
                    {
                        /* update the outputs */
                        PDO_OutputMapping();
                    }
                }
                else if ( nPdOutputSize == 0 )
                {
                    /* if no outputs are transmitted, the watchdog must be reset, when the inputs were read */
                    if ( ALEvent & PROCESS_INPUT_EVENT )
                    {
                        /* Outputs were updated, set flag for watchdog monitoring */
                        bEcatFirstOutputsReceived = TRUE;
                    }
                }
            }

            no_interrupts();
            ECAT_Application();
            interrupts();
        }

        /* there is no interrupt routine for the hardware timer so check the timer register if the desired cycle elapsed*/
        {
            UINT16 CurTimer = HW_GetTimer();

            if(CurTimer>= ECAT_TIMER_INC_P_MS)
            {
                ECAT_CheckTimer();

                HW_ClearTimer();
            }
        }

        /* call EtherCAT functions */
		//cx
//		Serial.println("Mainloop");
        ECAT_Main();

        /* call lower prior application part */
        COE_Main();
        CheckIfEcatError();

}

/*The main function was moved to the application files.*/
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    ECAT_Application (prev. SSC versions "COE_Application")
 this function calculates and the physical process signals and triggers the input mapping
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::ECAT_Application(void)
{
    {
        APPL_Application();
    }

    if ( bEcatInputUpdateRunning )
    {
        /* EtherCAT slave is at least in SAFE-OPERATIONAL, update inputs */
        PDO_InputMapping();
    }

}

/*The PDO generation and mapping functions are moved to the application files*/
/*PDO_ResetOutputs() is removed. Resetting the outputs can be done in the specific ESM function */




/** @} */


