#include "socket.h"

static uint16 local_port;

extern W5500 w5500;

W5500* eth = &w5500;

/**
@brief   This Socket function initialize the channel in perticular mode, and set the port and wait for W5200 done it.
@return  0 for sucess else -1.
*/
uint8 socket(SOCKET s, uint8 protocol, uint16 port, uint8 flag)
{
   uint8 ret;
   if (
        ((protocol&0x0F) == Sn_MR_TCP)    ||
        ((protocol&0x0F) == Sn_MR_UDP)    ||
        ((protocol&0x0F) == Sn_MR_IPRAW)  ||
        ((protocol&0x0F) == Sn_MR_MACRAW) ||
        ((protocol&0x0F) == Sn_MR_PPPOE)
      )
   {
      close(s);
      eth->write(Sn_MR(s) ,protocol | flag);
      if (port != 0) {
         eth->write( Sn_PORT0(s) ,(uint8)((port & 0xff00) >> 8));
         eth->write( Sn_PORT1(s) ,(uint8)(port & 0x00ff));
      } else {
         local_port++; // if don't set the source port, set local_port number.
         eth->write(Sn_PORT0(s) ,(uint8)((local_port & 0xff00) >> 8));
         eth->write(Sn_PORT1(s) ,(uint8)(local_port & 0x00ff));
      }
      eth->write( Sn_CR(s) ,Sn_CR_OPEN); // run sockinit Sn_CR

      /* wait to process the command... */
      while( eth->read(Sn_CR(s)) )
         ;
      /* ------- */
      ret = 0;
   }
   else
   {
      ret = 0xff;
   }
   return ret;
}

/**
@brief   This function close the socket and parameter is "s" which represent the socket number
*/
void close(SOCKET s)
{

	
   eth->write( Sn_CR(s) ,Sn_CR_CLOSE);

   /* wait to process the command... */
   while( eth->read(Sn_CR(s) ) )
      ;
   /* ------- */
        /* all clear */
   eth->write( Sn_IR(s) , 0xFF);
}
/**
@brief   This function established  the connection for the channel in passive (server) mode. This function waits for the request from the peer.
@return  1 for success else 0.
*/
uint8 listen(SOCKET s)
{
   uint8 ret;
   if (eth->read( Sn_SR(s) ) == SOCK_INIT)
   {
      eth->write( Sn_CR(s) ,Sn_CR_LISTEN);
      /* wait to process the command... */
      while( eth->read(Sn_CR(s) ) )
         ;
      /* ------- */
      ret = 1;
   }
   else
   {
      ret = 0;
   }
   return ret;
}
/**
@brief   This function established  the connection for the channel in Active (client) mode.
      This function waits for the untill the connection is established.

@return  1 for success else 0.
*/
uint8 connect(SOCKET s, uint8 * addr, uint16 port)
{
    uint8 ret;
    if
        (
            ((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
            ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
            (port == 0x00)
        )
    {
      ret = 0;
    }
    else
    {
        ret = 1;
        // set destination IP
        eth->write( Sn_DIPR0(s), addr[0]);
        eth->write( Sn_DIPR1(s), addr[1]);
        eth->write( Sn_DIPR2(s), addr[2]);
        eth->write( Sn_DIPR3(s), addr[3]);
        eth->write( Sn_DPORT0(s), (uint8)((port & 0xff00) >> 8));
        eth->write( Sn_DPORT1(s), (uint8)(port & 0x00ff));
        eth->write( Sn_CR(s) ,Sn_CR_CONNECT);
        /* wait for completion */
        while ( eth->read(Sn_CR(s) ) ) ;

        while ( eth->read(Sn_SR(s)) != SOCK_SYNSENT )
        {
            if(eth->read(Sn_SR(s)) == SOCK_ESTABLISHED)
            {
                break;
            }
            if (eth->getSn_IR(s) & Sn_IR_TIMEOUT)
            {
                eth->write(Sn_IR(s), (Sn_IR_TIMEOUT));  // clear TIMEOUT Interrupt
                ret = 0;
                break;
            }
        }
    }

   return ret;
}



/**
@brief   This function used for disconnect the socket and parameter is "s" which represent the socket number
@return  1 for success else 0.
*/
void disconnect(SOCKET s)
{
   eth->write( Sn_CR(s) ,Sn_CR_DISCON);

   /* wait to process the command... */
   while( eth->read(Sn_CR(s) ) )
      ;
   /* ------- */
}
/**
@brief   This function used to send the data in TCP mode
@return  1 for success else 0.
*/
uint16 send(SOCKET s, const uint8 * buf, uint16 len)
{
  uint8 status=0;
  uint16 ret=0;
  uint16 freesize=0;

  if (len > eth->getTxMAX(s)) ret = eth->getTxMAX(s); // check size not to exceed MAX size.
  else ret = len;

  // if freebuf is available, start.
  do
  {
    freesize = eth->getSn_TX_FSR(s);
    status = eth->read(Sn_SR(s));
    if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT))
    {
      ret = 0;
      break;
    }
  } while (freesize < ret);


  // copy data
  eth->send_data_processing(s, (uint8 *)buf, ret);
  eth->write( Sn_CR(s) ,Sn_CR_SEND);

  /* wait to process the command... */
  while( eth->read(Sn_CR(s) ) );

  while ( (eth->read(Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
  {
    status = eth->read(Sn_SR(s));
    if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT) )
    {
//      printf("SEND_OK Problem!!\r\n");
      close(s);
      return 0;
    }
  }
  eth->write( Sn_IR(s) , Sn_IR_SEND_OK);

//#ifdef __DEF_IINCHIP_INT__
//   putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
//#else
   eth->write( Sn_IR(s) , Sn_IR_SEND_OK);
//#endif

   return ret;
}
/**
@brief   This function is an application I/F function which is used to receive the data in TCP mode.
      It continues to wait for data as much as the application wants to receive.

@return  received data size for success else -1.
*/
uint16 recv(SOCKET s, uint8 * buf, uint16 len)
{
   uint16 ret=0;
   if ( len > 0 )
   {
      eth->recv_data_processing(s, buf, len);
      eth->write( Sn_CR(s) ,Sn_CR_RECV);
      /* wait to process the command... */
      while( eth->read(Sn_CR(s) ));
      /* ------- */
      ret = len;
   }
   return ret;
}
/**
@brief   This function is an application I/F function which is used to send the data for other then TCP mode.
      Unlike TCP transmission, The peer's destination address and the port is needed.

@return  This function return send data size for success else -1.
*/
uint16 sendto(SOCKET s, const uint8 * buf, uint16 len, uint8 * addr, uint16 port)
{
   uint16 ret=0;

   if (len > eth->getTxMAX(s)) ret = eth->getTxMAX(s); // check size not to exceed MAX size.
   else ret = len;

   if( ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) || ((port == 0x00)) )//||(ret == 0) )
   {
      /* added return value */
      ret = 0;
   }
   else
   {
      eth->write( Sn_DIPR0(s), addr[0]);
      eth->write( Sn_DIPR1(s), addr[1]);
      eth->write( Sn_DIPR2(s), addr[2]);
      eth->write( Sn_DIPR3(s), addr[3]);
      eth->write( Sn_DPORT0(s),(uint8)((port & 0xff00) >> 8));
      eth->write( Sn_DPORT1(s),(uint8)(port & 0x00ff));
      // copy data
      eth->send_data_processing(s, (uint8 *)buf, ret);
      eth->write( Sn_CR(s) ,Sn_CR_SEND);
      /* wait to process the command... */
      while( eth->read( Sn_CR(s) ) )
         ;
      /* ------- */

      while( (eth->read( Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
      {
         if (eth->read( Sn_IR(s) ) & Sn_IR_TIMEOUT)
         {
            /* clear interrupt */
            eth->write( Sn_IR(s) , (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */
            return 0;
         }
      }
      eth->write( Sn_IR(s) , Sn_IR_SEND_OK);
   }
   return ret;
}

/**
@brief   This function is an application I/F function which is used to receive the data in other then
   TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well.

@return  This function return received data size for success else -1.
*/
uint16 recvfrom(SOCKET s, uint8 * buf, uint16 len, uint8 * addr, uint16 *port)
{
   uint8 head[8];
   uint16 data_len=0;
   uint16 ptr=0;
   uint32 addrbsb =0;
   if ( len > 0 )
   {
      ptr     = eth->read(Sn_RX_RD0(s) );
      ptr     = ((ptr & 0x00ff) << 8) + eth->read(Sn_RX_RD1(s));
      addrbsb = (uint32)(ptr<<8) +  (s<<5) + 0x18;
      
      switch (eth->read(Sn_MR(s) ) & 0x07)
      {
      case Sn_MR_UDP :
        eth->read(addrbsb, head, 0x08);        
        ptr += 8;
        // read peer's IP address, port number.
        addr[0]  = head[0];
        addr[1]  = head[1];
        addr[2]  = head[2];
        addr[3]  = head[3];
        *port    = head[4];
        *port    = (*port << 8) + head[5];
        data_len = head[6];
        data_len = (data_len << 8) + head[7];

        addrbsb = (uint32)(ptr<<8) +  (s<<5) + 0x18;
        eth->read(addrbsb, buf, data_len);                
        ptr += data_len;

        eth->write( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
        eth->write( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
        break;

      case Sn_MR_IPRAW :
        eth->read(addrbsb, head, 0x06);        
        ptr += 6;
        addr[0]  = head[0];
        addr[1]  = head[1];
        addr[2]  = head[2];
        addr[3]  = head[3];
        data_len = head[4];
        data_len = (data_len << 8) + head[5];

        addrbsb  = (uint32)(ptr<<8) +  (s<<5) + 0x18;
        eth->read(addrbsb, buf, data_len);        
        ptr += data_len;

        eth->write( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
        eth->write( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
        break;

      case Sn_MR_MACRAW :
        eth->read(addrbsb, head, 0x02);
        ptr+=2;
        data_len = head[0];
        data_len = (data_len<<8) + head[1] - 2;
        if(data_len > 1514)
        {
//           printf("data_len over 1514\r\n");
           while(1);
        }

        addrbsb  = (uint32)(ptr<<8) +  (s<<5) + 0x18;
        eth->read(addrbsb, buf, data_len);
        ptr += data_len;

        eth->write( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
        eth->write( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
        break;

      default :
            break;
      }
      eth->write( Sn_CR(s) ,Sn_CR_RECV);

      /* wait to process the command... */
      while( eth->read( Sn_CR(s)) ) ;
      /* ------- */
   }
   return data_len;
}


