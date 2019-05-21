#ifndef __DRI_DMX512_H
#define __DRI_DMX512_H

#include "ebox.h"


class DMXReceiver
{
    public:
        DMXReceiver(Uart *uart)
        {
            this->uart = uart;
        }
        void begin(uint16_t maxSlotNum);
        void rx_event();
        uint8_t getValue(uint16_t ch);
        bool is_online();
        
        
    private:
        typedef enum
        {
            WAIT_BREAK,
            RECV_SC,
            RECV_SLOTS,
        }DMX512_State_t;
        
        Uart *uart;
        uint8_t *rxBuf;
        uint16_t maxSlotNum;
        uint32_t last_on_time;
        uint16_t slot_counter;
        DMX512_State_t state;
};


class DMXMaster
{
    public:
        DMXMaster(Uart *uart)
        {
            this->uart = uart;
        }
        void begin(uint16_t maxSlotNum);
        uint8_t setValue(uint16_t ch,uint8_t value);
        void loop();
    private:
        typedef enum
        {
            WAIT_BREAK,
            RECV_SC,
            RECV_SLOTS,
        }DMX512_State_t;
        
        Uart *uart;
        uint8_t *txBuf;
        uint16_t maxSlotNum;
        uint16_t slot_counter;
        uint32_t last_send_time;
        DMX512_State_t state;
};
#endif

