/*
    ModbusIP.cpp - Source for Modbus IP Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "ModbusIP.h"

ModbusIP::ModbusIP(): _server(MODBUSIP_PORT)
{

}

void ModbusIP::config(uint8_t *mac)
{
    Ethernet.begin(mac);
    _server.begin();
}

void ModbusIP::config(uint8_t *mac, IPAddress ip)
{
    Ethernet.begin(mac, ip);
    Ethernet.localIP().printTo(uart1);
    _server.begin();
    ebox_printf("config\r\n");
}

void ModbusIP::config(uint8_t *mac, IPAddress ip, IPAddress dns)
{
    Ethernet.begin(mac, ip, dns);
    _server.begin();
}

void ModbusIP::config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway)
{
    Ethernet.begin(mac, ip, dns, gateway);
    _server.begin();
}

void ModbusIP::config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet)
{
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    _server.begin();
}
bool ModbusIP::task()
{
    bool flag = false;
    EthernetClient client = _server.available();
    if (client)
    {
        if (client.connected())
        {
            int i = 0;
            while (client.available())
            {
                flag = true;
                _MBAP[i] = client.read();
                i++;
                if (i == 7) break; //MBAP length has 7 bytes size
            }
            _len = _MBAP[4] << 8 | _MBAP[5];
            _len--;  // Do not count with last byte from MBAP

            if (_MBAP[2] != 0 || _MBAP[3] != 0) return false; //Not a MODBUSIP packet
            if (_len > MODBUSIP_MAXFRAME) return false;      //Length is over MODBUSIP_MAXFRAME

            _frame = (byte *) ebox_malloc(_len);
            
            i = 0;
            while (client.available())
            {
                _frame[i] = client.read();
                i++;
                if (i == _len) break;
            }
            
//			 MB_DBG("====frame context(%d)===\n",_len);
//            for(int i = 0; i < _len; i++)
//            {
//                MB_DBG_DATA("0x%x ",_frame[i]);
//            }
//            MB_DBG("====frame end===\n");
            
            if(cb_pre_deal != NULL){
                cb_pre_deal(_frame);
            }
            this->receivePDU(_frame);
            if (_reply != MB_REPLY_OFF)
            {
                //MBAP
                _MBAP[4] = (_len + 1) >> 8;   //_len+1 for last byte from MBAP
                _MBAP[5] = (_len + 1) & 0x00FF;

                byte *sendbuffer = (byte *)ebox_malloc(7 + _len);

                for (i = 0 ; i < 7 ; i++)
                {
                    sendbuffer[i] = _MBAP[i];
                }
                //PDU Frame
                for (i = 0 ; i < _len ; i++)
                {
                    sendbuffer[i + 7] = _frame[i];
                }
                data_com = true;
                client.write(sendbuffer, _len + 7);
                ebox_free(sendbuffer);
            }

#if  (TCP_KEEP_ALIVE == 0)
            client.stop();
#endif
            ebox_free(_frame);
            _len = 0;
            return flag;
        }
    }
    else
    {
    }
}