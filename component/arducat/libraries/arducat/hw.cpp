/**
\defgroup el9800hw el9800hw.c: EL9800 hardware abstraction
\brief Hardware access implementation for EL9800 onboard PIC18/PIC24 connected via SPI to ESC\n
\brief Changes to version V5.0:
\brief V5.01 HW1: Invalid ESC access function was used
\brief Changes to version V4.40:
\brief V5.0 ESC4: Save SM disable/Enable. Operation may be pending due to frame handling.
\brief Changes to version V4.30:\n
\brief V4.40 : File renamed from spihw.c to el9800hw.c
\brief Changes to version V4.20:\n
\brief V4.30 ESM: if mailbox Syncmanger is disabled and bMbxRunning is true the SyncManger settings need to be revalidate
\brief V4.30 EL9800: EL9800_x hardware initialization is moved to el9800.c
\brief V4.30 SYNC: change synchronisation control function. Add usage of 0x1C32:12 [SM missed counter].
\brief Calculate bus cycle time (0x1C32:02 ; 0x1C33:02) CalcSMCycleTime()
\brief V4.30 PDO: rename PDO specific functions (COE_xxMapping -> PDO_xxMapping and COE_Application -> ECAT_Application)
\brief V4.30 ESC: change requested address in GetInterruptRegister() to prevent acknowledge events.
\brief (e.g. reading an SM config register acknowledge SM change event)
\brief GENERIC: renamed several variables to identify used SPI if multiple interfaces are available
\brief V4.20 MBX 1: Add Mailbox queue support
\brief V4.20 SPI 1: include SPI RxBuffer dummy read
\brief V4.20 DC 1: Add Sync0 Handling
\brief V4.20 PIC24: Add EL9800_4 (PIC24) required source code
\brief V4.08 ECAT 3: The AlStatusCode is changed as parameter of the function AL_ControlInd\n
\brief Changes to version V4.02:\n
\brief V4.03 SPI 1: In ISR_GetInterruptRegister the NOP-command should be used.\n
\brief Changes to version V4.01:\n
\brief V4.02 SPI 1: In HW_OutputMapping the variable u16OldTimer shall not be set,
\brief              otherwise the watchdog might exceed too early.\n
\brief Changes to version V4.00:\n
\brief V4.01 SPI 1: DI and DO were changed (DI is now an input for the uC, DO is now an output for the uC)\n
\brief V4.01 SPI 2: The SPI has to operate with Late-Sample = FALSE on the Eva-Board\n
\brief Changes to version V3.20:\n
\brief V4.00 ECAT 1: The handling of the Sync Manager Parameter was included according to
\brief               the EtherCAT Guidelines and Protocol Enhancements Specification\n
\brief V4.00 APPL 1: The watchdog checking should be done by a microcontroller
\brief                  timer because the watchdog trigger of the ESC will be reset too
\brief                  if only a part of the sync manager data is written\n
\brief V4.00 APPL 4: The EEPROM access through the ESC is added\n


\version 5.01
*/

//---------------------------------------------------------------------------------------
/**
\ingroup el9800hw
\file    el9800hw.c
\brief Implementation.
*/

//---------------------------------------------------------------------------------------
/**
\addtogroup el9800hw
@{
*/
//---------------------------------------------------------------------------------------

/*--------------------------------------------------------------------------------------
------
------    Includes
------
--------------------------------------------------------------------------------------*/
#include "ecat_def.h"



#include "ethercat.h"
//#include <SPI.h>
#include "bsp_ebox.h"
/*--------------------------------------------------------------------------------------
--------------------Ethercat pin definitions--------------------------------------------
SSEL = PH2
SIRQ = PE6(INT6)
SYNC = PE7(INT7)


----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------
------
------    SPI defines/macros
------
-----------------------------------------------------------------------------------------*/

//#define    WAIT_SPI_IF                        //spi.end();//while( SPI1_IF );//while( !SPI1_IF );
#define    SPI_CS                          PA4//digitalWrite(15, LOW);//{(LPC_GPIO3->DATA) &= ~(SPI_ACTIVE);}
#define    SPI_CS_INIT                     SPI_CS.mode(OUTPUT_PP)//digitalWrite(15, LOW);//{(LPC_GPIO3->DATA) &= ~(SPI_ACTIVE);}
#define    SELECT_SPI                      SPI_CS.reset()//digitalWrite(15, LOW);//{(LPC_GPIO3->DATA) &= ~(SPI_ACTIVE);}
#define    DESELECT_SPI                    SPI_CS.set()//digitalWrite(15, HIGH);//{(LPC_GPIO3->DATA) |= (SPI_DEACTIVE);}
//#define    INIT_SSPIF                        //{(SPI1_IF)=0;}
#define SPI1_CON0_VALUE                    	0x000000C7//0x027E
#define SPI1_CON0_VALUE_16BIT            	0x000000CF//0x047E
#define SPI1_CON1_VALUE                    	0x00000002//Write at end
#define SPI1_CPSR_VALUE                    	0x00000002//Write at end
#define SPI_DEACTIVE                    (1<<4)
#define SPI_ACTIVE                      (1<<4)

#define SPI spi1
/*-----------------------------------------------------------------------------------------
------
------    Global Interrupt setting
------
-----------------------------------------------------------------------------------------*/

#define DISABLE_GLOBAL_INT           no_interrupts();
#define ENABLE_GLOBAL_INT            interrupts();
#define    DISABLE_AL_EVENT_INT        DISABLE_GLOBAL_INT
#define    ENABLE_AL_EVENT_INT            ENABLE_GLOBAL_INT


/*-----------------------------------------------------------------------------------------
------
------    ESC Interrupt
------
-----------------------------------------------------------------------------------------*/


#define ESC_PIN     PA3
#define	   INIT_ESC_INT		attachInterrupt(6,SIRQ_IRQHandler,LOW );



/*-----------------------------------------------------------------------------------------
------
------    SYNC0 Interrupt
------
-----------------------------------------------------------------------------------------*/
#define SYNC0_PIN   PA2
#define	   INIT_SYNC0_INT		//{(LPC_GPIO1->IE)|=(1<<9);(LPC_GPIO1->IS)|=(1<<9);(LPC_GPIO1->IEV)&=~(1<<9);NVIC_EnableIRQ(EINT1_IRQn);}
//#define    DISABLE_SYNC0_INT    detachInterrupt(7);//            {(LPC_GPIO1->IE)&=~(1<<9);}//disable interrupt source INT3
//#define    ENABLE_SYNC0_INT     attachInterrupt(7,SYNC_IRQHandler,LOW );//           {(LPC_GPIO1->IE)|=(1<<9);} //enable interrupt source INT3

/*-----------------------------------------------------------------------------------------
------
------    Hardware timer
------
-----------------------------------------------------------------------------------------*/


#define INIT_ECAT_TIMER                


#define STOP_ECAT_TIMER 

#define START_ECAT_TIMER 



/*--------------------------------------------------------------------------------------
------
------    internal Variables
------
--------------------------------------------------------------------------------------*/
//UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc
//UINT16            nAlEventMask;        //current ALEventMask (content of register 0x204:0x205)
//TSYNCMAN        TmpSyncMan;

/*--------------------------------------------------------------------------------------
------
------    internal functions
------
--------------------------------------------------------------------------------------*/
void SIRQ_IRQHandler();
void SYNC_IRQHandler();

#define CMD_SERIAL_WRITE 0x02
#define CMD_SERIAL_READ 0x03

UINT32 SPIReadDWord (UINT16 Address)
{
	UINT32 Val;
    //Assert CS line
    SELECT_SPI;
    //Write Command
    SPI.transfer(CMD_SERIAL_READ);
    //Write Address
    SPI.transfer(*((UINT8*)&Address+1));
    SPI.transfer(*((UINT8*)&Address+0));
    //Read Bytes
    *((UINT8*)&Val+0) = SPI.transfer(0xFF);
    *((UINT8*)&Val+1) = SPI.transfer(0xFF);
    *((UINT8*)&Val+2) = SPI.transfer(0xFF);
    *((UINT8*)&Val+3) = SPI.transfer(0xFF);
    //De-Assert CS line
    DESELECT_SPI;

	
    return Val;
}

void SPIWriteDWord (UINT16 Address, UINT32 Val)
{
    //Assert CS line
    SELECT_SPI;
    //Write Command
    SPI.transfer(CMD_SERIAL_WRITE);
    //Write Address
    SPI.transfer(*((UINT8*)&Address+1));
    SPI.transfer(*((UINT8*)&Address+0));
    //Write Bytes
    SPI.transfer(*((UINT8*)&Val+0));
    SPI.transfer(*((UINT8*)&Val+1));
    SPI.transfer(*((UINT8*)&Val+2));
    SPI.transfer(*((UINT8*)&Val+3));

    //De-Assert CS line
    DESELECT_SPI;
}

#define ESC_CSR_CMD_REG		0x304
#define ESC_CSR_DATA_REG	0x300
#define ESC_WRITE_BYTE 		0x80
#define ESC_READ_BYTE 		0xC0
#define ESC_CSR_BUSY		0x80
void SPIReadDRegister(UINT8 *ReadBuffer, UINT16 Address, UINT8 Count)
{
    UINT32 param32_1 = 0;
    UINT8 i = 0;
    UINT16 wAddr = Address;

    *((UINT8*)&param32_1+0) = *((UINT8*)&wAddr+0);
    *((UINT8*)&param32_1+1) = *((UINT8*)&wAddr+1);
    *((UINT8*)&param32_1+2) = Count;
    *((UINT8*)&param32_1+3) = ESC_READ_BYTE;

    SPIWriteDWord (ESC_CSR_CMD_REG, param32_1);

    do
    {
	param32_1 = SPIReadDWord (ESC_CSR_CMD_REG);
		
    }while(*((UINT8*)&param32_1+3) & ESC_CSR_BUSY);

    param32_1 = SPIReadDWord (ESC_CSR_DATA_REG);

    
    for(i=0;i<Count;i++)
         ReadBuffer[i] = *((UINT8*)&param32_1+i);
   
    return;
}
void SPIWriteRegister( UINT8 *WriteBuffer, UINT16 Address, UINT8 Count)
{
    UINT32 param32_1 = 0;
    UINT8 i = 0;
    UINT16 wAddr;

    for(i=0;i<Count;i++)
         *((UINT8*)&param32_1+i) = WriteBuffer[i];

    SPIWriteDWord (ESC_CSR_DATA_REG, param32_1);


    wAddr = Address;

    *((UINT8*)&param32_1+0) = *((UINT8*)&wAddr+0);
    *((UINT8*)&param32_1+1) = *((UINT8*)&wAddr+1);
    *((UINT8*)&param32_1+2) = Count;
    *((UINT8*)&param32_1+3) = ESC_WRITE_BYTE;

    SPIWriteDWord (0x304, param32_1);
    do
    {
	param32_1 = SPIReadDWord (0x304);
    }while(*((UINT8*)&param32_1+3) & ESC_CSR_BUSY);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::GetInterruptRegister(void)
{
      DISABLE_AL_EVENT_INT;
      HW_EscReadIsr(&EscALEvent.Word, 0x220, 2);
      ENABLE_AL_EVENT_INT;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.
        Shall be implemented if interrupts are supported else this function is equal to "GetInterruptRegsiter()"

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::ISR_GetInterruptRegister(void)
{
     HW_EscReadIsr(&EscALEvent.Word, 0x220, 2);
}



/*--------------------------------------------------------------------------------------
------
------    exported hardware access functions
------
--------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if initialization was successful

 \brief    This function intialize the Process Data Interface (PDI) and the host controller.
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 Ethercat::HW_Init(void)
{
    UINT16 intMask;
	UINT32 data;
	//Initial SSEL pin 
    SPI_CS_INIT;

    
    SpiConfig_t config;
    config.bit_order = MSB_FIRST;
    config.dev_num = PA4.id;
    config.mode = SPI_MODE3;
    config.prescaler = SPI_CLOCK_DIV2;
    
	//Initial SPI
	SPI.begin(&config);
      /* initialize the SPI registers for the ESC SPI */
//	SPI.setClockDivider(SPI_CLOCK_DIV8);
//	SPI.setBitOrder(MSBFIRST); 
//	SPI.setDataMode(SPI_MODE3);	//CPOL=CPHA=1
	//Initialize with a rising edge
//	PORTH |= (1<<2);
    PA4.set();
    
	data = 0;
	while(data!=0x87654321)
	{
		data = SPIReadDWord (0x64);
		delay_ms(1);
	}
	Serial.print("ByteOrder Passed:");
	Serial.println(data,HEX);
	Serial.flush();
	data = 0;
	while(!(data&0x08000000))
	{
		data = SPIReadDWord (0x74);
		delay_ms(1);
	}
	Serial.print("Device Ready:");
	Serial.println(data,HEX);
	Serial.flush();
    do
    {
        intMask = 0x93;
        HW_EscWriteWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0;
        HW_EscReadWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while (intMask != 0x93);
	Serial.println("AL_EVENTMASK_OFFSET_Written.");
	Serial.flush();

    //INIT_ESC_INT
    Exti   userbt1(&PA3);

    userbt1.begin();
    //userbt1.attach(SIRQ_IRQHandler, RISE);
    userbt1.attach(SIRQ_IRQHandler, FALL);
    userbt1.interrupt(FALL,ENABLE);

    
    HW_ResetALEventMask(0);

    //IRQ enable,IRQ polarity, IRQ buffer type in Interrupt Configuration register.
    //Wrte 0x54 - 0x00000111
    data = 0x00000101;
    SPIWriteDWord (0x54, data);
    //Write in Interrupt Enable register -->
    //Write 0x5c - 0x00000001
    data = 0x00000001;
    SPIWriteDWord (0x5C, data);
    SPIReadDWord(0x58);

    INIT_SYNC0_INT
//    ENABLE_SYNC0_INT;

    INIT_ECAT_TIMER;
    START_ECAT_TIMER;

    /* enable all interrupts */
    ENABLE_GLOBAL_INT;

    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function shall be implemented if hardware resources need to be release
        when the sample application stops
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_Release(void)
{
//	SPI.end();
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  This function gets the current content of ALEvent register
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 Ethercat::HW_GetALEventRegister(void)
{
    GetInterruptRegister();
    return EscALEvent.Word;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_GetALEventRegister()"
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 Ethercat::HW_GetALEventRegister_Isr(void)
{
     ISR_GetInterruptRegister();
    return EscALEvent.Word;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    intMask        interrupt mask (disabled interrupt shall be zero)

 \brief    This function makes an logical and with the AL Event Mask register (0x204)
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_ResetALEventMask(UINT16 intMask)
{
    UINT16 mask;

    HW_EscReadWord(mask, ESC_AL_EVENTMASK_OFFSET);

    mask &= intMask;
    DISABLE_AL_EVENT_INT;
    HW_EscWriteWord(mask, ESC_AL_EVENTMASK_OFFSET);
    HW_EscReadWord(nAlEventMask, ESC_AL_EVENTMASK_OFFSET);
    ENABLE_AL_EVENT_INT;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    intMask        interrupt mask (enabled interrupt shall be one)

  \brief    This function makes an logical or with the AL Event Mask register (0x204)
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_SetALEventMask(UINT16 intMask)
{
    UINT16 mask;

    HW_EscReadWord(mask, ESC_AL_EVENTMASK_OFFSET);

    mask |= intMask;
    DISABLE_AL_EVENT_INT;
    HW_EscWriteWord(mask, ESC_AL_EVENTMASK_OFFSET);
    HW_EscReadWord(nAlEventMask, ESC_AL_EVENTMASK_OFFSET);
    ENABLE_AL_EVENT_INT;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param RunLed            desired EtherCAT Run led state
 \param ErrLed            desired EtherCAT Error led state

  \brief    This function updates the EtherCAT run and error led
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_SetLed(UINT8 RunLed,UINT8 ErrLed)
{
//      LED_ECATGREEN = RunLed;
//      LED_ECATRED   = ErrLed;
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  This function operates the SPI read access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EscRead( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* loop for all bytes to be read */
    while ( Len > 0 )
    {

        i= (Len > 4) ? 4 : Len;

        if(Address & 01)
        {
           i=1;
        }
        else if (Address & 02)
        {
           i= (i&1) ? 1:2;
        }
        else if (i == 03)
        {
            i=1;
        }

        DISABLE_AL_EVENT_INT;

       SPIReadDRegister(pTmpData,Address,i);
      
        ENABLE_AL_EVENT_INT;

        Len -= i;
        pTmpData += i;
        Address += i;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

\brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_EscRead()"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EscReadIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
   UINT16 i;
   UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */

    /* loop for all bytes to be read */
   while ( Len > 0 )
   {

        i= (Len>4) ? 4:Len;

        if(Address & 01)
        {
           i=1;
        }
        else if (Address & 02)
        {
           i= (i&1) ? 1:2;
        }
        else if (i == 03)
        {
            i=1;
        }

        SPIReadDRegister(pTmpData, Address,i);

        Len -= i;
        pTmpData += i;
        Address += i;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

  \brief  This function operates the SPI write access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i;
    UINT8 *pTmpData = (UINT8 *)pData;


    /* loop for all bytes to be written */
    while ( Len )
    {

        i= (Len>4) ? 4:Len;

        if(Address & 01)
        {
           i=1;
        }
        else if (Address & 02)
        {
           i= (i&1) ? 1:2;
        }
        else if (i == 03)
        {
            i=1;
        }
        //DISABLE_AL_EVENT_INT;
        DISABLE_AL_EVENT_INT;
        /* start transmission */
        SPIWriteRegister(pTmpData, Address, i);

        ENABLE_AL_EVENT_INT;
   
        /* next address */
        Len -= i;
        pTmpData += i;
        Address += i;

    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  The SPI PDI requires an extra ESC write access functions from interrupts service routines.
        The behaviour is equal to "HW_EscWrite()"
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EscWriteIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i ;
    UINT8 *pTmpData = (UINT8 *)pData;

  
    /* loop for all bytes to be written */
    while ( Len )
    {

        i= (Len > 4) ? 4 : Len;

        if(Address & 01)
        {
           i=1;
        }
        else if (Address & 02)
        {
           i= (i&1) ? 1:2;
        }
        else if (i == 03)
        {
            i=1;
        }
        
       /* start transmission */
       SPIWriteRegister(pTmpData, Address, i);
       
       /* next address */
        Len -= i;
        pTmpData += i;
        Address += i;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     channel        Sync Manager channel

 \brief    This function disables a Sync Manager channel
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_DisableSyncManChannel(UINT8 channel)
{
    UINT16 Offset;
    //The register 0x806 is only readable from PDI => writing 0 is valid
    UINT16 smStatus = SM_SETTING_PDI_DISABLE;
    Offset = (ESC_SYNCMAN_ACTIVE_OFFSET + (SIZEOF_SM_REGISTER*channel));

    HW_EscWriteWord(smStatus,Offset);
    
    /*wait until SyncManager is disabled*/
    do
    {
        HW_EscReadWord(smStatus, Offset);
    }while(!(smStatus & SM_SETTING_PDI_DISABLE));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     channel        Sync Manager channel

 \brief    This function enables a Sync Manager channel
*////////////////////////////////////////////////////////////////////////////////////////
void Ethercat::HW_EnableSyncManChannel(UINT8 channel)
{
    UINT16 Offset;
    //The register 0x806 is only readable from PDI => writing 0 is valid
    UINT16 smStatus = 0x0000;
    Offset = (ESC_SYNCMAN_ACTIVE_OFFSET + (SIZEOF_SM_REGISTER*channel));

    HW_EscWriteWord(smStatus,Offset);
    
    /*wait until SyncManager is enabled*/
    do
    {
        HW_EscReadWord(smStatus,Offset);
    }while((smStatus & SM_SETTING_PDI_DISABLE));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param channel    requested Sync Manger channel information

 \return    Pointer to the SYNC Manager channel description

 \brief  This function is called to read the SYNC Manager channel descriptions of the
             process data SYNC Managers.
*////////////////////////////////////////////////////////////////////////////////////////

TSYNCMAN ESCMEM * Ethercat::HW_GetSyncMan(UINT8 channel)
{
    // get a temporary structure of the Sync Manager:
    HW_EscRead( (MEM_ADDR *) &TmpSyncMan, ESC_SYNCMAN_REG_OFFSET + (channel * SYNCMAN_REG_SIZE), SYNCMAN_REG_SIZE );

    return &TmpSyncMan;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the PDI interrupt from the EtherCAT Slave Controller
*////////////////////////////////////////////////////////////////////////////////////////

//void __attribute__ ((__interrupt__, no_auto_psv)) EscIsr & SYNC0(void)
void SIRQ_IRQHandler()
{
    	ethercat.PDI_Isr();
}
void SYNC_IRQHandler()
{
    	ethercat.Sync0_Isr();
}





