#include "w5500.h"

void W5500::begin()
{
	
	spiDevW5500.devNum = 2;
	spiDevW5500.mode = SPI_MODE3;
	spiDevW5500.prescaler = SPI_CLOCK_DIV4;
	spiDevW5500.bitOrder = SPI_BITODER_MSB;
	
	SPIClASS::begin(&spiDevW5500);
	cs->mode(OUTPUT_PP);
	cs->set();
}
void W5500::reset()
{
	rstPin->reset();
	delay_us(500);
	rstPin->set();
}

void W5500::write(u32 addrbsb, u8 data)
{
//	  IINCHIP_ISR_DISABLE();                        // Interrupt Service Routine Disable
   cs->reset();                              // CS=0, SPI start
   transfer( (addrbsb & 0x00FF0000)>>16);// Address byte 1
   transfer( (addrbsb & 0x0000FF00)>> 8);// Address byte 2
   transfer( (addrbsb & 0x000000F8) + 4);    // Data write command and Write data length 1
   transfer(data);                    // Data write (write 1byte data)
   cs->set();                                 // CS=1,  SPI end
//   IINCHIP_ISR_ENABLE();                         // Interrupt Service Routine Enable


}
u8  W5500::read(u32 addrbsb)
{
   u8 data = 0;
//   IINCHIP_ISR_DISABLE();                        // Interrupt Service Routine Disable
   cs->reset();                          // CS=0, SPI start
   transfer( (addrbsb & 0x00FF0000)>>16);// Address byte 1
   transfer( (addrbsb & 0x0000FF00)>> 8);// Address byte 2
   transfer( (addrbsb & 0x000000F8))    ;// Data read command and Read data length 1
   data = transfer(0xf0);                // Data read (read 1byte data)
   cs->set();                            // CS=1,  SPI end
//   IINCHIP_ISR_ENABLE();               // Interrupt Service Routine Enable
   return data;    

}
u16 W5500::write(u32 addrbsb,u8* buf, u16 len)
{
   u16 idx = 0;

//   IINCHIP_ISR_DISABLE();
   cs->reset();                               // CS=0, SPI start
   transfer( (addrbsb & 0x00FF0000)>>16);// Address byte 1
   transfer( (addrbsb & 0x0000FF00)>> 8);// Address byte 2
   transfer( (addrbsb & 0x000000F8) + 4);    // Data write command and Write data length 1
   for(idx = 0; idx < len; idx++)                // Write data in loop
   {
     transfer(buf[idx]);
   }
   cs->set();                                 // CS=1, SPI end
//   IINCHIP_ISR_ENABLE();                         // Interrupt Service Routine Enable    

   return len;  

}
u16 W5500::read(u32 addrbsb,u8* buf, u16 len)
{
  u16 idx = 0;
//  IINCHIP_ISR_DISABLE();
   cs->reset();                               // CS=0, SPI start
  transfer( (addrbsb & 0x00FF0000)>>16);// Address byte 1
  transfer( (addrbsb & 0x0000FF00)>> 8);// Address byte 2
  transfer( (addrbsb & 0x000000F8));    // Data write command and Write data length 1
  for(idx = 0; idx < len; idx++)                    // Write data in loop
  {
    buf[idx] = transfer(0x00);
  }
   cs->set();                                 // CS=1, SPI end
//  IINCHIP_ISR_ENABLE();                             // Interrupt Service Routine Enable
  
  return len;

}
/**
@brief  This function sets up MAC address.
*/
void W5500::setSHAR(  
	u8 * addr  /**< a pointer to a 6 -byte array responsible to set the MAC address. */
  )
{
  write(SHAR0, addr, 6);  
}
/**
@brief  This function sets up Source IP address.
*/
void W5500::setSIPR(
  u8 * addr  /**< a pointer to a 4 -byte array responsible to set the Source IP address. */
  )
{
    write(SIPR0, addr, 4);  
}
/**
@brief  It sets up SubnetMask address
*/
void W5500::setSUBR(u8 * addr)
{   
    write(SUBR0, addr, 4);
}
/**
@brief  This function sets up gateway IP address.
*/
void W5500::setGAR(
  u8 * addr  /**< a pointer to a 4 -byte array responsible to set the Gateway IP address. */
  )
{
    write(GAR0, addr, 4);
}





void W5500::getSHAR(u8 * addr )//mac
{
  read(SHAR0, addr, 6);  
}void  W5500::getSIPR(u8 * addr)//ip
{
    read(SIPR0, addr, 4);
}
void  W5500::getSUBR(u8 * addr)//mask
{
    read(SUBR0, addr, 4);
}
void W5500::getGWIP(u8 * addr)//gateway
{
    read(GAR0, addr, 4);
}
void  W5500::getGAR(u8 * addr)//gateway
{
    read(GAR0, addr, 4);
}


void W5500::setMR(u8 val)
{
  write(MR,val);
}
/**
@brief  This function gets Interrupt register in common register.
 */
u8 W5500::getIR( void )
{
   return read(IR);
}

/**
@brief  This function sets up Retransmission time.

If there is no response from the peer or delay in response then retransmission
will be there as per RTR (Retry Time-value Register)setting
*/
void W5500::setRTR(u16 timeout)
{
  write(RTR0,(u8)((timeout & 0xff00) >> 8));
  write(RTR1,(u8)(timeout & 0x00ff));
}

/**
@brief  This function set the number of Retransmission.

If there is no response from the peer or delay in response then recorded time
as per RTR & RCR register seeting then time out will occur.
*/
void W5500::setRCR(u8 retry)
{
  write(WIZ_RCR,retry);
}

/**
@brief  This function set the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)

If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
set in IR register.
*/
void W5500::clearIR(u8 mask)
{
  write(IR, ~mask | getIR() ); // must be setted 0x10.
}
/**
@brief  This sets the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*/
void W5500::setSn_MSS(u8 s, u16 Sn_MSSR)
{
  write( Sn_MSSR0(s), (u8)((Sn_MSSR & 0xff00) >> 8));
  write( Sn_MSSR1(s), (u8)(Sn_MSSR & 0x00ff));
}

void W5500::setSn_TTL(u8 s, u8 ttl)
{    
   write( Sn_TTL(s) , ttl);
}
/**
@brief  get socket interrupt status

These below functions are used to read the Interrupt & Soket Status register
*/
u8  W5500::getSn_IR(u8 s)
{
   return read(Sn_IR(s));
}

/**
@brief   get socket status
*/
u8  W5500::getSn_SR(u8 s)
{
   return read(Sn_SR(s));
}

/**
@brief  get socket TX free buf size

This gives free buffer size of transmit buffer. This is the data size that user can transmit.
User shuold check this value first and control the size of transmitting data
*/
u16 W5500::getSn_TX_FSR(u8 s)
{
  u16 val=0,val1=0;
  do
  {
    val1 = read(Sn_TX_FSR0(s));
    val1 = (val1 << 8) + read(Sn_TX_FSR1(s));
      if (val1 != 0)
    {
        val = read(Sn_TX_FSR0(s));
        val = (val << 8) + read(Sn_TX_FSR1(s));
    }
  } while (val != val1);
   return val;
}
/**
@brief   get socket RX recv buf size

This gives size of received data in receive buffer.
*/
u16 W5500::getSn_RX_RSR(u8 s)
{
  u16 val=0,val1=0;
  do
  {
    val1 = read(Sn_RX_RSR0(s));
    val1 = (val1 << 8) + read(Sn_RX_RSR1(s));
    if(val1 != 0)
    {
        val = read(Sn_RX_RSR0(s));
        val = (val << 8) + read(Sn_RX_RSR1(s));
    }
  } while (val != val1);
   return val;
}

/**
@brief   This function is being called by send() and sendto() function also.

This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
register. User should read upper byte first and lower byte later to get proper value.
*/
void W5500::send_data_processing(u8 s, u8 *data, u16 len)
{
  u16 ptr = 0;
  u32 addrbsb = 0;
  if(len == 0)
  {
    printf("CH: %d Unexpected1 length 0\r\n", s);
    return;
  }


  ptr = read( Sn_TX_WR0(s) );
  ptr = ((ptr & 0x00ff) << 8) + read(Sn_TX_WR1(s));

  addrbsb = ((u32)ptr<<8) + (s<<5) + 0x10;
  write(addrbsb, data, len);
  
  ptr += len;
  write( Sn_TX_WR0(s) ,(u8)((ptr & 0xff00) >> 8));
  write( Sn_TX_WR1(s),(u8)(ptr & 0x00ff));
}


/**
@brief  This function is being called by recv() also.

This function read the Rx read pointer register
and after copy the data from receive buffer update the Rx write pointer register.
User should read upper byte first and lower byte later to get proper value.
*/
void W5500::recv_data_processing(u8 s, u8 *data, u16 len)
{
  u16 ptr = 0;
  u32 addrbsb = 0;
  
  if(len == 0)
  {
    printf("CH: %d Unexpected2 length 0\r\n", s);
    return;
  }

  ptr = read( Sn_RX_RD0(s) );
  ptr = ((ptr & 0x00ff) << 8) + read( Sn_RX_RD1(s) );

  addrbsb = ((u32)ptr<<8) + (s<<5) + 0x18;
  read(addrbsb, data, len);
  ptr += len;

  write( Sn_RX_RD0(s), (u8)((ptr & 0xff00) >> 8));
  write( Sn_RX_RD1(s), (u8)(ptr & 0x00ff));
}

void W5500::setSn_IR(u8 s, u8 val)
{
    write(Sn_IR(s), val);
}





