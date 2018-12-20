#ifndef ETHERCAT_H
#define ETHERCAT_H

#include "ebox_core.h"

#include "ecat_def.h"
#include "esc.h"
#include "ecatslv.h"
#include "ecatappl.h"
#include "ecatcoe.h"
#include "mailbox.h"
#include "sdoserv.h"
#include "objdef.h"

//==========================HWDIFINES====================================
#define ECAT_TIMER_INC_P_MS                1000    //1000 micros/ms

/*--------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
--------------------------------------------------------------------------------------*/

typedef union
{
    unsigned short    Word;
    unsigned char    Byte[2];
} UBYTETOWORD;

typedef union 
{
    UINT8           Byte[2];
    UINT16          Word;
}
UALEVENT;


class Ethercat
{
public:
	Ethercat();
	~Ethercat();
private:
	//=========================ECATSLV_H=================================
    BOOL                           bEcatOutputUpdateRunning;           // indicates the OP state, will be set in StartOutputHandler
                                                                            // and reset in StopOutputHandler
	BOOL                            bEcatInputUpdateRunning;           // indicates the SAFEOP or OP state, will be set in StartInputHandler
                                                                            // and reset in Stop InputHandler
    BOOL                           bEcatFirstOutputsReceived;          // indicates if outputs were received (SM2-event)
                                                                            // or inputs were read (SM3-event, if the output size is 0),
                                                                            // has to be set by the application and reset in StopOutputHandler
    BOOL                           bWdTrigger;                         // indicates that the SM2 WD trigger bit (0x814 bit6) is set.
    BOOL                           bDcSyncActive;                      // indicates that the Distributed Clocks synchronization is active,
    INT16                          EsmTimeoutCounter;               // Counter used to detect an ESM timeout. -1 indicates a deactivated counter and 0 is expired
    BOOL                           bDcRunning;                         //indicates if Sync0 events are received
	BOOL                              bSmSyncToggle;                      //this variable is set to false in the PDI Isr and  to true in der corresponding Sync event
	BOOL                              bPllRunning;                        //set to true if Esc/Sync0 sequence is valid
	INT16                             i16WaitForPllRunningTimeout;        //the time bPllRunnig shall be true while state change from SafeOp to Op
	INT16                             i16WaitForPllRunningCnt;
	UINT16                            Sync0WdCounter;
	UINT16                            Sync0WdValue;
	BOOL                              bEscIntEnabled;                      // indicates that the ESC interrupt is enabled (SM2/3 or SYNC0/1-event),
                                                                             // will be set in StartInputHandler and reset in StopInputHandler
	BOOL                              b3BufferMode;                        // indicates that inputs and outputs are running in 3-Buffer-Mode
	BOOL                              bLocalErrorFlag;                     // contains the information if the application has a local error
	UINT16                            u16LocalErrorCode;                   //reason for local error
	BOOL                              bApplEsmPending;                     //indicates if the local application ESM function need to be called from Al_ConntrolRes (is true if NOERR_INWORK is returned by generic ESM function)
	BOOL                              bEcatWaitForAlControlRes;            // contains the information that the state machine waits for an acknowledge
                                                                             // for the last AL_ControlInd from the application/generic stack
	UINT16                            nEcatStateTrans;
	UINT8                             u8EcatErrorLed;
	UINT8                             u8EcatRunLed;
	UINT16                            nPdInputSize;                        // contains the input size (SM3 size), has to be written by the application
	UINT16                            nPdOutputSize;                       // contains the output size (SM3 size), has to be written by the application
	UINT8                             nMaxSyncMan;                         // contains the maximum number of Sync Manager channels, will be initialized in ECAT_Main
	UINT8                             nAlStatus;                           // contains the actual AL Status, will be written in AL_ControlInd
	UINT16                            EcatWdValue;                         // contains the value of the watchdog in 100us, will be written in StartInputHandler. 
    UINT16                         nEscAddrOutputData;                  // contains the SM2 address
    UINT16                         nEscAddrInputData;                   // contains the SM3 address
	void SetALStatus(UINT8 alStatus, UINT16 alStatusCode);
	void AL_ControlInd(UINT8 alControl, UINT16 alStatusCode);
	void DC_CheckWatchdog(void);
    void CheckIfEcatError(void);
	void ECAT_Init(void);
	void ECAT_StateChange(UINT8 alStatus, UINT16 alStatusCode);
	void ECAT_Main(void);

	//===============================ECATAPPL_H====================================
	BOOL bEcatWaitForInputUpdate;
	BOOL bEtherCATRunLed;
	BOOL bEtherCATErrorLed;
	BOOL bRunApplication;
public:
	UINT16 MainInit(void);
	void 	   MainLoop(void);
    void       PDI_Isr(void);
    void       Sync0_Isr(void);
private:
    void       ECAT_CheckTimer(void);
    void       ECAT_Application(void);
    void       PDO_ResetOutputs(void);
    void       PDO_ReadInputs(void);
    void       PDO_InputMapping(void);
	void       PDO_OutputMapping(void);
    void       ECAT_SetLedIndication(void);
    void       CalcSMCycleTime(void);
	
	//========================MAILBOX_H==============================
	BOOL                    bReceiveMbxIsLocked;
	BOOL                    bSendMbxIsFull;
	BOOL                    bMbxRunning;
	BOOL                    bMbxRepeatToggle;
	UINT16                  u16SendMbxSize;
	UINT16                  u16ReceiveMbxSize;
	UINT16                  u16EscAddrReceiveMbx;
	UINT16                  u16EscAddrSendMbx;
	UINT8                   u8MbxWriteCounter;
	UINT8                   u8MbxReadCounter;
	TMBX MBXMEM             asMbx[2];
	UINT8                   u8MailboxSendReqStored;
	TMBX MBXMEM *           psWriteMbx;
	TMBX MBXMEM *           psReadMbx;
	TMBX MBXMEM *           psRepeatMbx;
	TMBX MBXMEM *           psStoreMbx;
	TMBXQUEUE MBXMEM        sMbxSendQueue;
	TMBXQUEUE MBXMEM        sMbxReceiveQueue;
	void     MBX_Init(void);
	UINT8    MBX_StartMailboxHandler(void);
	void     MBX_StopMailboxHandler(void);
	void     MBX_MailboxWriteInd(TMBX MBXMEM *pMbx);
	void     MBX_MailboxReadInd(void);
	void     MBX_MailboxRepeatReq(void);
	UINT8    MBX_MailboxSendReq(TMBX MBXMEM * pMbx, UINT8 flags);
	void     MBX_CheckAndCopyMailbox(void);
	UINT8    MBX_CopyToSendMailbox(TMBX MBXMEM *pMbx);
	void     MBX_Main(void);
	

	//==========================OBJ_H==================================
	TCYCLEDIAG sCycleDiag;
    OBJCONST TOBJECT OBJMEM *  OBJ_GetObjectHandle( UINT16 index );
    UINT16     OBJ_GetObjectLength( UINT16 index, UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT8 bCompleteAccess);
    UINT16    OBJ_GetNoOfObjects(UINT8 listType);
    UINT16    OBJ_GetObjectList(UINT16 listType, UINT16 *pIndex, UINT16 size, UINT16 MBXMEM *pData,UINT8 *pAbort);
    UINT16     OBJ_GetDesc( UINT16 index, UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData );
    OBJCONST TSDOINFOENTRYDESC OBJMEM * OBJ_GetEntryDesc(OBJCONST TOBJECT OBJMEM * pObjEntry, UINT8 Subindex);
    OBJCONST TSDOINFOOBJDESC OBJMEM * OBJ_GetObjDesc(OBJCONST TOBJECT OBJMEM * pObjEntry);
    UINT16     OBJ_GetEntryOffset(UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry);
    UINT8     OBJ_Read( UINT16 index, UINT8 subindex, UINT32 completeSize, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData, UINT8 bCompleteAccess );
    UINT8     OBJ_Write( UINT16 index, UINT8 subindex, UINT32 completeSize, OBJCONST TOBJECT OBJMEM * pObjEntry, UINT16 MBXMEM * pData, UINT8 bCompleteAccess );
    void    COE_WriteBackupEntry(UINT8 subindex, OBJCONST TOBJECT OBJMEM * pObjEntry);
public:
 	TSYNCMANPAR MBXMEM sSyncManOutPar;
	TSYNCMANPAR MBXMEM sSyncManInPar;
private:	
	//========================ECATCOE_H================================
	TMBX MBXMEM * VARMEM pCoeSendStored;                /* if the mailbox service could not be sent (or stored),
                                                                the CoE service will be stored in this variable
                                                                and will be sent automatically from the mailbox handler
                                                                (COE_ContinueInd) when the send mailbox will be read
                                                                the next time from the master */
	void     COE_Init(void);
	UINT8    COE_ServiceInd(TCOEMBX MBXMEM *pCoeMbx);
	void     COE_ContinueInd(TMBX MBXMEM * pMbx);
	
	//======================SDOSERVE_H=================================
	//Member variables
	/* ECATCHANGE_START(V5.01) SDO6*/
	#define     SDO_PENDING_WRITE       0x1
	#define     SDO_PENDING_SEG_WRITE   0x2
	#define     SDO_PENDING_READ        0x3
	#define     SDO_PENDING_SEG_READ    0x4
	UINT8         u8PendingSdo;
	BOOL          bStoreCompleteAccess;
	UINT8         u8StoreSubindex;
	UINT16        u16StoreIndex;
	UINT32        u32StoreDataSize;
	UINT16 MBXMEM *pStoreData;
	UINT8 (* pSdoPendFunc)( UINT16 Index, UINT8 Subindex, UINT32 Size, UINT16 MBXMEM * pData, UINT8 bCompleteAccess );
	/* ECATCHANGE_END(V5.01) SDO6*/
	UINT16 VARMEM * VARMEM                pSdoSegData;
	#define SDO_INFO_HEADER_BYTE_SIZE         ((SIZEOF_SDOINFOSTRUCT)+(SIZEOF_SDOINFOLISTHEAD))
	MEM_ADDR      VARMEM                aSdoInfoHeader[GET_MEM_SIZE(SDO_INFO_HEADER_BYTE_SIZE)];
	UINT16        VARMEM                nSdoInfoFragmentsLeft;
	//Functions
	UINT8 SDOS_SdoInfoInd(TSDOINFORMATION MBXMEM *pSdoInfoInd);
	UINT8 SDOS_SdoInd(TINITSDOMBX MBXMEM *pSdoMbx);
	void  SDOS_SdoRes(UINT8 abort, UINT32 objLength, UINT16 MBXMEM *pData);
	void SdoRes(UINT8 abort, UINT8 command, UINT8 completeAccess, UINT16 dataSize, UINT32 objLength, TINITSDOMBX MBXMEM *pSdoRes);
	//=======================COEAPPL_H=================================
	OBJCONST TOBJECT OBJMEM * COE_GetObjectDictionary(void);
	void COE_ObjInit(void);
	void COE_Main(void);
	UINT16 COE_ObjDictionaryInit(void);
	UINT16 COE_AddObjectToDic(TOBJECT OBJMEM * pNewObjEntry);
	void COE_RemoveDicEntry(UINT16 index);
	void COE_ClearObjDictionary(void);
	UINT16 AddObjectsToObjDictionary(TOBJECT OBJMEM * pObjEntry);
	
	//====================HARDWARE_H===================================
#define ESC_RD                    0x02            ///< read access to ESC
#define ESC_WR                    0x04            ///< write access to ESC	
	unsigned long lastTime;
	unsigned long inline HW_GetTimer()
	{	return micros()-lastTime;	}	//auto overflow
	void inline HW_ClearTimer()
	{	lastTime = micros();	}

public:
	UINT8 HW_Init(void);

private:
	void HW_Release(void);
	UINT16 HW_GetALEventRegister(void);
	UINT16 HW_GetALEventRegister_Isr(void);
	void HW_ResetALEventMask(UINT16 intMask);
	void HW_SetALEventMask(UINT16 intMask);
	void HW_SetLed(UINT8 RunLed,UINT8 ErrLed);

	void HW_EscRead( MEM_ADDR * pData, UINT16 Address, UINT16 Len );
	void inline HW_EscReadWord( UINT16& WordValue, UINT16 Address)
	{	HW_EscRead(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2);	}
	void inline HW_EscReadDWord( UINT32& DWordValue, UINT16 Address)
	{	HW_EscRead(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4);	}
	void inline HW_EscReadMbxMem(MEM_ADDR *pData, UINT16 Address,UINT16 Len)
	{	HW_EscRead(((MEM_ADDR *)(pData)),((UINT16)(Address)),(Len));	}


	void HW_EscReadIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len );
	void inline HW_EscReadWordIsr(UINT16 &WordValue, UINT16 Address)
	{	HW_EscReadIsr(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2);	}
	void inline HW_EscReadDWordIsr(UINT32 &DWordValue, UINT16 Address) 
	{	HW_EscReadIsr(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4);	}

	void HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len );
	void inline HW_EscWriteWord(UINT16 WordValue, UINT16 Address) 
	{	HW_EscWrite(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2);	}
	void inline HW_EscWriteDWord(UINT32 DWordValue, UINT16 Address) 
	{	HW_EscWrite(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4);	}
	void inline HW_EscWriteMbxMem(MEM_ADDR *pData,UINT16 Address,UINT16 Len) 
	{	HW_EscWrite(((MEM_ADDR *)(pData)),((UINT16)(Address)),(Len));	}

	void HW_EscWriteIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len );
	void inline HW_EscWriteWordIsr(UINT16 WordValue, UINT16 Address) 
	{	HW_EscWriteIsr(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2);	}
	void inline HW_EscWriteDWordIsr(UINT32 DWordValue, UINT16 Address) 
	{	HW_EscWriteIsr(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4);	}

	void HW_DisableSyncManChannel(UINT8 channel);
	void HW_EnableSyncManChannel(UINT8 channel);
	TSYNCMAN ESCMEM *HW_GetSyncMan(UINT8 channel);	
	
	//=====================COEAPPL_C==========================
	TOBJECT    OBJMEM * ObjDicList;	
	//====================ECATAPPL_C==========================
	/*variables only required to calculate values for SM Synchronisation objects (0x1C3x)*/
	UINT16 u16BusCycleCntMs;        //used to calculate the bus cycle time in Ms
	UINT32 StartTimerCnt;    //variable to store the timer register value when get cycle time was triggered
	BOOL bCycleTimeMeasurementStarted; // indicates if the bus cycle measurement is started
	#if MAX_PD_OUTPUT_SIZE > 0
	UINT16             aPdOutputData[(MAX_PD_OUTPUT_SIZE>>1)];
	#endif
	#if MAX_PD_INPUT_SIZE > 0
	UINT16           aPdInputData[(MAX_PD_INPUT_SIZE>>1)];
	#endif
	//===================ECATSLV_C============================
	//VARVOLATILE UINT16    u16dummy;
	UINT16    u16dummy;
	UINT16    u16ALEventMask;
	UINT8    CheckSmSettings(UINT8 maxChannel);
	UINT16 StartInputHandler(void);
	UINT16 StartOutputHandler(void);
	void StopOutputHandler(void);
	void StopInputHandler(void);
	void AL_ControlRes(void);
	//===================OBJDEF_C=============================
	static const UINT16 cBitMask[16];
	static CHAR OBJMEM aSubindexDesc[13];
	void OBJ_CopyNumberToString(UCHAR MBXMEM *pStr, UINT8 Number);
	//==================MAILBOX_C=============================
	UINT8 PutInMbxQueue(TMBX MBXMEM * pMbx, TMBXQUEUE MBXMEM * pQueue);
	TMBX MBXMEM * GetOutOfMbxQueue(TMBXQUEUE MBXMEM * pQueue);
	UINT8 MailboxServiceInd(TMBX MBXMEM *pMbx);
	//===================SDOSERVE_C===========================
	static const UINT32 MBXMEM cAbortCode[];
	UINT16 VARMEM                            nSdoInfoIndex;
	OBJCONST TOBJECT OBJMEM * VARMEM        pSdoInfoObjEntry;

	TINITSDOMBX MBXMEM *    VARMEM            pSdoResStored;
	static BOOL    VARMEM                            bSdoInWork;

	UINT8    VARMEM                         nSdoSegService;
	UINT8    VARMEM                         bSdoSegFollows;
	UINT8    VARMEM                         bSdoSegAccess;
	UINT16 VARMEM                           nSdoSegIndex;
	UINT8 VARMEM                            nSdoSegSubindex;
	UINT32 VARMEM                           nSdoSegBytesToHandle;
	UINT8 VARMEM                            bSdoSegLastToggle;
	UINT32 VARMEM                           nSdoSegCompleteSize;
	OBJCONST TOBJECT OBJMEM * VARMEM        pSdoSegObjEntry;
	UINT8 SdoDownloadSegmentInd( TDOWNLOADSDOSEGREQMBX MBXMEM * pSdoInd );
	UINT8 SdoUploadSegmentInd( TUPLOADSDOSEGREQMBX MBXMEM * pSdoInd );
	//===================HARDWARE_C============================
	UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc
	UINT16            nAlEventMask;        //current ALEventMask (content of register 0x204:0x205)
	TSYNCMAN        TmpSyncMan;
	void GetInterruptRegister(void);
	void ISR_GetInterruptRegister(void);
	void AddressingEsc( UINT16 Address, UINT8 Command );
	void ISR_AddressingEsc( UINT16 Address, UINT8 Command );
	//==============Application handlers========================
	void APPL_Application(void);

	void   APPL_AckErrorInd(UINT16 stateTrans);
	UINT16 APPL_StartMailboxHandler(void);
	UINT16 APPL_StopMailboxHandler(void);
	UINT16 APPL_StartInputHandler(UINT16 *pIntMask);
	UINT16 APPL_StopInputHandler(void);
	UINT16 APPL_StartOutputHandler(void);
	UINT16 APPL_StopOutputHandler(void);

	UINT16 APPL_GenerateMapping(UINT16 *pInputSize,UINT16 *pOutputSize);
	void APPL_InputMapping(UINT16* pData);
	void APPL_OutputMapping(UINT16* pData);
};

extern Ethercat ethercat;


#endif
