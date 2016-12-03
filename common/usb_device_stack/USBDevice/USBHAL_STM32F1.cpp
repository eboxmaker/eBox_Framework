/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stm32f10x.h"
#include "USBHAL.h"
#include "usb_regs.h"
#include "stdint.h"



/* buffer table base address */
#define BTABLE_ADDRESS      (0x00)


/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK (CNTR_CTRM | CNTR_RESETM )





static volatile int epComplete = 0;

static u16  wInterrupt_Mask;
volatile u16 wIstr;  /* ISTR register last read value */

/* 4 endpoins reserved  4*8(0x20)bytes for buffer description table*/
static volatile uint32_t bufferEnd = 0x20;

static uint8_t usb_address = 0;

void                    *pData; 

#ifdef __cplusplus
extern "C" {
#endif

void USB_LP_CAN1_RX0_IRQHandler(void)
{
		 reinterpret_cast<USBHAL*>(pData)->usb_callback();
}


#ifdef __cplusplus
}
#endif


/*******************************************************************************
* Function Name  : UserToPMABufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf: pointer to user memory area.
*                  - wPMABufAddr: address into PMA.
*                  - wNBytes: no. of bytes to be copied.
* Output         : None.
* Return         : None	.
*******************************************************************************/
void UserToPMABufferCopy(u8 *pbUsrBuf, u16 wPMABufAddr, u16 wNBytes)
{
  u32 n = (wNBytes + 1) >> 1;   /* n = (wNBytes + 1) / 2 */
  u32 i, temp1, temp2;
  u16 *pdwVal;
  pdwVal = (u16 *)(wPMABufAddr * 2 + PMAAddr);
  for (i = n; i != 0; i--)
  {
    temp1 = (u16) * pbUsrBuf;
    pbUsrBuf++;
    temp2 = temp1 | (u16) * pbUsrBuf << 8;
    *pdwVal++ = temp2;
    pdwVal++;
    pbUsrBuf++;
  }
}
/*******************************************************************************
* Function Name  : PMAToUserBufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf    = pointer to user memory area.
*                  - wPMABufAddr = address into PMA.
*                  - wNBytes     = no. of bytes to be copied.
* Output         : None.
* Return         : None.
*******************************************************************************/
void PMAToUserBufferCopy(u8 *pbUsrBuf, u16 wPMABufAddr, u16 wNBytes)
{
     u32 n = (wNBytes + 1) >> 1;/* /2*/
     u32 i;
     u32 *pdwVal;
     pdwVal = (u32 *)(wPMABufAddr * 2 + PMAAddr);  //*2得到真正的地址
     for (i = n; i != 0; i--)
     {
         *(u16*)pbUsrBuf++ = *pdwVal++;
         pbUsrBuf++;
     }
}





void USBHAL::usb_callback(void)
{
	usbisr();
}

uint32_t USBHAL::endpointReadcore(uint8_t endpoint, uint8_t *buffer) {
    return 0;
}

USBHAL::USBHAL(void) {

		u16 wRegVal;
  
		pData = this;
	
	
		epCallback[0] = &USBHAL::EP1_OUT_callback;
		epCallback[1] = &USBHAL::EP1_IN_callback;
		epCallback[2] = &USBHAL::EP2_OUT_callback;
		epCallback[3] = &USBHAL::EP2_IN_callback;
		epCallback[4] = &USBHAL::EP3_OUT_callback;
		epCallback[5] = &USBHAL::EP3_IN_callback;

  // Select USBCLK source 
     RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);  // 72MHZ sysclk

  /* Enable USB clock */
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
	

		/*** CNTR_PWDN = 0 ***/ //USB 收发器内部参照电压
		wRegVal = CNTR_FRES;
		_SetCNTR(wRegVal);

		/*** CNTR_FRES = 0 ***/
		wInterrupt_Mask = 0;
		_SetCNTR(wInterrupt_Mask);
		/*** Clear pending interrupts ***/
		_SetISTR(0);
		/*** Set interrupt mask ***/
		wInterrupt_Mask = IMR_MSK;
		_SetCNTR(wInterrupt_Mask);
		
		SetBTABLE(BTABLE_ADDRESS); //?
		
		_SetDADDR(DADDR_EF);//?
					
}

USBHAL::~USBHAL(void) {
  /* disable all ints and force USB reset */
  _SetCNTR(CNTR_FRES);
  /* clear interrupt status register */
  _SetISTR(0);
  /* Disable the Pull-Up*/
 
  /* switch-off device */
  _SetCNTR(CNTR_FRES + CNTR_PDWN);
  /* sw variables reset */
}

void USBHAL::connect(void) {
	  NVIC_InitTypeDef NVIC_InitStructure; 
		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;//make for ebox
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

void USBHAL::disconnect(void) {
	  NVIC_InitTypeDef NVIC_InitStructure; 
		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;//make for ebox
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStructure);	
}

void USBHAL::configureDevice(void) {
    // Not needed
}

void USBHAL::unconfigureDevice(void) {
    // Not needed
}

void USBHAL::setAddress(uint8_t address) {
		 usb_address = address;
}

bool USBHAL::realiseEndpoint(uint8_t endpoint, uint32_t maxPacket,
                             uint32_t flags) {														 
    switch(endpoint) {
        case EP0OUT:
						SetEPType(ENDP0, EP_CONTROL);
						SetEPRxAddr(ENDP0, bufferEnd);
						Clear_Status_Out(ENDP0);//single buffer 
						SetEPRxCount(ENDP0,maxPacket);
						_SetEPAddress(ENDP0,0);
						SetEPRxValid(ENDP0);
            break;
        case EP0IN:
						SetEPType(ENDP0, EP_CONTROL);
						SetEPTxStatus(ENDP0, EP_TX_STALL);
						SetEPTxAddr(ENDP0, bufferEnd);
						_SetEPAddress(ENDP0,0);
						SetEPTxCount(ENDP0,maxPacket);
            break;
        case EPINT_OUT:
						SetEPType(ENDP1, EP_INTERRUPT);
						SetEPRxAddr(ENDP1, bufferEnd);
						SetEPRxCount(ENDP1, maxPacket);
						_SetEPAddress(ENDP1,1);
						SetEPRxStatus(ENDP1, EP_RX_VALID);
            break;
        case EPINT_IN:
						SetEPType(ENDP1, EP_INTERRUPT);
						SetEPTxAddr(ENDP1, bufferEnd);
						SetEPTxCount(ENDP1, maxPacket);
						_SetEPAddress(ENDP1,1);
						SetEPTxStatus(ENDP1, EP_TX_NAK);
            break;
        case EPBULK_OUT:
						SetEPType(ENDP2, EP_BULK);
						SetEPRxAddr(ENDP2, bufferEnd);
						SetEPRxCount(ENDP2,maxPacket);
						_SetEPAddress(ENDP2,2);
						SetEPRxStatus(ENDP2, EP_RX_VALID);
            break;
        case EPBULK_IN:
						SetEPType(ENDP2, EP_BULK);
						SetEPTxAddr(ENDP2, bufferEnd);
						SetEPTxCount(ENDP2,maxPacket);
						_SetEPAddress(ENDP2,2);
						SetEPTxStatus(ENDP2, EP_TX_NAK);
            break;
        case EP3OUT:
						SetEPType(ENDP3, EP_ISOCHRONOUS);
						SetEPRxAddr(ENDP3, bufferEnd);
						SetEPRxCount(ENDP3,maxPacket);
						_SetEPAddress(ENDP3,3);
						SetEPRxStatus(ENDP3, EP_RX_VALID);
            break;
        case EP3IN:
						SetEPType(ENDP3, EP_ISOCHRONOUS);
						SetEPTxAddr(ENDP3, bufferEnd);
						SetEPTxCount(ENDP3,maxPacket);
						_SetEPAddress(ENDP3,3);
						SetEPTxStatus(ENDP3, EP_TX_NAK);
            break;
        default:
            ;
            return false;
    }
		bufferEnd += maxPacket;
    return true;
}

// read setup packet
void USBHAL::EP0setup(uint8_t *buffer) {
		PMAToUserBufferCopy(buffer, GetEPRxAddr(ENDP0), MAX_PACKET_SIZE_EP0);
}

void USBHAL::EP0readStage(void) {
}

void USBHAL::EP0read(void) {
		SetEPRxValid(ENDP0);
}

uint32_t USBHAL::EP0getReadResult(uint8_t *buffer) {

    uint32_t length = GetEPRxCount(ENDP0);
		
		PMAToUserBufferCopy(buffer, GetEPRxAddr(ENDP0), length);

    return length;
}

void USBHAL::EP0write(uint8_t *buffer, uint32_t size) {
    endpointWrite(EP0IN, buffer, size);
}

void USBHAL::EP0getWriteResult(void) {
}

void USBHAL::EP0stall(void) {
    // If we stall the out endpoint here then we have problems transferring
    // and setup requests after the (stalled) get device qualifier requests.
    // TODO: Find out if this is correct behavior, or whether we are doing
    // something else wrong
    stallEndpoint(EP0IN);
//    stallEndpoint(EP0OUT);
}

EP_STATUS USBHAL::endpointRead(uint8_t endpoint, uint32_t maximumSize) {
    uint32_t epIndex = endpoint >> 1;
		
		SetEPRxValid(epIndex);

    epComplete &= ~(1 << endpoint);
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointReadResult(uint8_t endpoint, uint8_t * buffer, uint32_t *bytesRead) {
    if (!(epComplete & (1 << endpoint))) {
        return EP_PENDING;
    }
		uint32_t epIndex = endpoint >> 1;
		
		uint32_t length = GetEPRxCount(epIndex);
		
		PMAToUserBufferCopy(buffer, GetEPRxAddr(epIndex),length);
		
    *bytesRead = length;
    return EP_COMPLETED;
}

EP_STATUS USBHAL::endpointWrite(uint8_t endpoint, uint8_t *data, uint32_t size) {
    
		uint8_t epIndex = endpoint >> 1;    
		UserToPMABufferCopy(data, GetEPTxAddr(epIndex), size);
		SetEPTxCount(epIndex, size);
    SetEPTxValid(epIndex);

    epComplete &= ~(1 << endpoint);
	
    return EP_PENDING;
}

EP_STATUS USBHAL::endpointWriteResult(uint8_t endpoint) {
    if (epComplete & (1 << endpoint)) {
        epComplete &= ~(1 << endpoint);
        return EP_COMPLETED;
    }

    return EP_PENDING;
}

void USBHAL::stallEndpoint(uint8_t endpoint) {  
    if (endpoint & 0x1) { // In EP
				SetEPTxStatus(endpoint >> 1, EP_TX_STALL);
    }
    else {  // Out EP
				SetEPRxStatus(endpoint >> 1, EP_RX_STALL);
    }
}

void USBHAL::unstallEndpoint(uint8_t endpoint) {

}

bool USBHAL::getEndpointStallState(uint8_t endpoint) {
    return false;
}

void USBHAL::remoteWakeup(void) {
}


void USBHAL::_usbisr(void) {
}
void USBHAL::usbisr(void) {

  	wIstr = _GetISTR();
  	if (wIstr & ISTR_RESET & wInterrupt_Mask)
  	{
  		//ISTR 程式只能清0，无法写1，
  		 //用要清除的位为0，其他位写为1
    	_SetISTR((u16)CLR_RESET); 
			
			bufferEnd = 0x20;
			
			realiseEndpoint(EP0IN, MAX_PACKET_SIZE_EP0, 0);
			realiseEndpoint(EP0OUT, MAX_PACKET_SIZE_EP0, 0);
			
			busReset();
			
			_SetDADDR(DADDR_EF);
			
		}
		  if (wIstr & ISTR_SUSP & wInterrupt_Mask)//USB掛起
  {
    	/* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
    	_SetISTR((u16)CLR_SUSP);		
	}
	  	if (wIstr & ISTR_SOF & wInterrupt_Mask)
  	{
    	_SetISTR((u16)CLR_SOF);

    }			
		  	if (wIstr & ISTR_ESOF & wInterrupt_Mask)
  	{
    	_SetISTR((u16)CLR_ESOF); //帧期望标志
			
		}
		
		
		if (wIstr & ISTR_CTR & wInterrupt_Mask)
  	{
    	/* servicing of the endpoint correct transfer interrupt */
    	/* clear of the CTR flag into the sub */
			  u32 wEPVal = 0;
			
  /* stay in loop while pending ints */
  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)
  {

		_SetISTR((u16)CLR_CTR); /* clear CTR flag */
    /* extract highest priority endpoint number */
    u8 EPindex = (u8)(wIstr & ISTR_EP_ID);
    if (EPindex == 0)
    {
      /* Decode and service control endpoint interrupt */
      /* calling related service routine */
      /* (Setup0_Process, In0_Process, Out0_Process) */


      /* DIR bit = origin of the interrupt */

      if ((wIstr & ISTR_DIR) == 0)
      {
        /* DIR = 0 */

        /* DIR = 0      => IN  int */
        /* DIR = 0 implies that (EP_CTR_TX = 1) always  */


        _ClearEP_CTR_TX(ENDP0);
				
        EP0in();
				//if((usb_address > 0)&& ( GetEPTxCount(0) == 0))
				if((usb_address > 0))
        {
          _SetDADDR(usb_address| DADDR_EF); /* set device address and enable function */
          usb_address = 0;
		
        }
				
          return;
      }
      else
      {
        /* DIR = 1 */

        /* DIR = 1 & CTR_RX       => SETUP or OUT int */
        /* DIR = 1 & (CTR_TX | CTR_RX) => 2 int pending */

        wEPVal = _GetENDPOINT(ENDP0);
        if ((wEPVal &EP_SETUP) != 0)
        {
          _ClearEP_CTR_RX(ENDP0); /* SETUP bit kept frozen while CTR_RX = 1 */
					EP0setupCallback();
          return; 
        }

        else if ((wEPVal & EP_CTR_RX) != 0)
        {
          _ClearEP_CTR_RX(ENDP0);
          EP0out();
          return;
        }
      }
    }/* if(EPindex == 0) */
    else
    {
      /* Decode and service non control endpoints interrupt  */
      /* process related endpoint register */
      wEPVal = _GetENDPOINT(EPindex);
      if ((wEPVal & EP_CTR_RX) != 0)
      {
        /* clear int flag */
        _ClearEP_CTR_RX(EPindex);

        /* call OUT service function */
				u8 endpoint = EPindex *2; //ep1 out -> callback[0] 2->[2] 3->[4]
        epComplete |= (1 << endpoint);
        if ((this->*(epCallback[endpoint - 2]))()) {
                    epComplete &= (1 << endpoint);
        }

      } /* if((wEPVal & EP_CTR_RX) */

      if ((wEPVal & EP_CTR_TX) != 0)
      {
        /* clear int flag */
        _ClearEP_CTR_TX(EPindex);

        /* call IN service function */
				u8 endpoint = EPindex *2 + 1; //ep1 in -> callback[1] 2->[3] 3->[5]
        epComplete |= (1 << (endpoint));
        if ((this->*(epCallback[endpoint-2]))()) {
                    epComplete &= (1 << endpoint);
        }
      } /* if((wEPVal & EP_CTR_TX) != 0) */

    }/* if(EPindex == 0) else */

  }/* while(...) */
			
			
		}
}

