/*
    Modbus.h - Header for Modbus Base Library
    Copyright (C) 2014 Andr?Sarmento Barbosa
*/

#ifndef MODBUS_H
#define MODBUS_H
#include "ebox.h"
#include "ebox_mem.h"

#define MODBUS_DEBUG   false

// Logging functions
#if MODBUS_DEBUG && EBOX_DEBUG
#define MB_DBG(...)  DBG("[MB DBG]"), DBG(__VA_ARGS__)
#define MB_DBG_DATA(...)   DBG(__VA_ARGS__)
#else
#define MB_DBG(...)
#define MB_DBG_DATA(...)
#endif


#define MAX_REGS     32
#define MAX_FRAME   128
//#define USE_HOLDING_REGISTERS_ONLY
typedef void (*MBcallbackType)(uint8_t);
typedef void (*MBcallbackBeforeType)(uint8_t,uint16_t,uint16_t);

typedef unsigned int u_int;

//Function Codes
enum
{
    MB_FC_READ_COILS       = 0x01, // Read Coils (Output) Status 0xxxx
    MB_FC_READ_INPUT_STAT  = 0x02, // Read Input Status (Discrete Inputs) 1xxxx
    MB_FC_READ_REGS        = 0x03, // Read Holding Registers 4xxxx
    MB_FC_READ_INPUT_REGS  = 0x04, // Read Input Registers 3xxxx
    MB_FC_WRITE_COIL       = 0x05, // Write Single Coil (Output) 0xxxx
    MB_FC_WRITE_REG        = 0x06, // Preset Single Register 4xxxx
    MB_FC_WRITE_COILS      = 0x0F, // Write Multiple Coils (Outputs) 0xxxx
    MB_FC_WRITE_REGS       = 0x10, // Write block of contiguous registers 4xxxx
};

//Exception Codes
enum
{
    MB_EX_ILLEGAL_FUNCTION = 0x01, // Function Code not Supported
    MB_EX_ILLEGAL_ADDRESS  = 0x02, // Output Address not exists
    MB_EX_ILLEGAL_VALUE    = 0x03, // Output Value not in Range
    MB_EX_SLAVE_FAILURE    = 0x04, // Slave Deive Fails to process request
};

//Reply Types
enum
{
    MB_REPLY_OFF    = 0x01,
    MB_REPLY_ECHO   = 0x02,
    MB_REPLY_NORMAL = 0x03,
};

typedef struct TRegister
{
    uint16_t address;
    uint16_t value;
    struct TRegister *next;
} TRegister;

class Modbus
{
private:
    TRegister *_regs_head;
    TRegister *_regs_last;

    TRegister *_ists_regs_head;
    TRegister *_ists_regs_last;

    TRegister *_input_regs_head;
    TRegister *_input_regs_last;

    void readRegisters(uint16_t startreg, uint16_t numregs);
    void writeSingleRegister(uint16_t reg, uint16_t value);
    void writeMultipleRegisters(byte *frame, uint16_t startreg, uint16_t numoutputs, byte bytecount);
    void exceptionResponse(byte fcode, byte excode);
#ifndef USE_HOLDING_REGISTERS_ONLY
    void readCoils(uint16_t startreg, uint16_t numregs);
    void readInputStatus(uint16_t startreg, uint16_t numregs);
    void readInputRegisters(uint16_t startreg, uint16_t numregs);
    void writeSingleCoil(uint16_t reg, uint16_t status);
    void writeMultipleCoils(byte *frame, uint16_t startreg, uint16_t numoutputs, byte bytecount);
#endif

    TRegister *searchRegister(uint16_t addr);
    void addReg(uint16_t address, uint16_t value = 0);
    bool Reg(uint16_t address, uint16_t value);
    uint16_t Reg(uint16_t address);

    TRegister *searchIstsRegister(uint16_t addr);
    void addIstsReg(uint16_t address, uint16_t value = 0);
    bool IstsReg(uint16_t address, uint16_t value);
    uint16_t IstsReg(uint16_t address);

    TRegister *searchInputRegister(uint16_t addr);
    void addInputReg(uint16_t address, uint16_t value = 0);
    bool InputReg(uint16_t address, uint16_t value);
    uint16_t InputReg(uint16_t address);

    MBcallbackType callback;
    MBcallbackBeforeType callbackBefor;
protected:
    byte *_frame;
    byte  _len;
    byte  _reply;
    void receivePDU(byte *frame);

public:
    Modbus();

    void addHreg(uint16_t offset, uint16_t value = 0);
    bool Hreg(uint16_t offset, uint16_t value);
    uint16_t Hreg(uint16_t offset);

#ifndef USE_HOLDING_REGISTERS_ONLY
    void addCoil(uint16_t offset, bool value = false);
    void addIsts(uint16_t offset, bool value = false);
    void addIreg(uint16_t offset, uint16_t value = 0);

    bool Coil(uint16_t offset, bool value);
    bool Ists(uint16_t offset, bool value);
    bool Ireg(uint16_t offset, uint16_t value);

    bool Coil(uint16_t offset);
    bool Ists(uint16_t offset);
    uint16_t Ireg(uint16_t offset);
#endif
    void attach(MBcallbackType cb){callback = cb;};
    void attachBefor(MBcallbackBeforeType cb){callbackBefor = cb;};

};

#endif //MODBUS_H
