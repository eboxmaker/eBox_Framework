/*
    Modbus.cpp - Source for Modbus Base Library
    Copyright (C) 2014 Andr?Sarmento Barbosa
*/
#include "Modbus.h"
#include "ebox.h"

Modbus::Modbus()
{
    _regs_head = 0;
    _regs_last = 0;
    
    _input_regs_head = 0;
    _input_regs_last = 0;
    
    _ists_regs_head = 0;
    _ists_regs_last = 0;
}

TRegister *Modbus::searchRegister(uint16_t address)
{
    TRegister *reg = _regs_head;
    //if there is no register configured, bail
    if(reg == 0) return(0);
    //scan through the linked list until the end of the list or the register is found.
    //return the pointer.
    do
    {
        if (reg->address == address) return(reg);
        reg = reg->next;
    }
    while(reg);
    return(0);
}


void Modbus::addReg(uint16_t address, uint16_t value)
{
    TRegister *newreg;

    newreg = (TRegister *) ebox_malloc(sizeof(TRegister));
    newreg->address = address;
    newreg->value		= value;
    newreg->next		= 0;

    if(_regs_head == 0)
    {
        _regs_head = newreg;
        _regs_last = _regs_head;
    }
    else
    {
        //Assign the last register's next pointer to newreg.
        _regs_last->next = newreg;
        //then make temp the last register in the list.
        _regs_last = newreg;
    }
}

bool Modbus::Reg(uint16_t address, uint16_t value)
{
    TRegister *reg;
    //search for the register address
    reg = this->searchRegister(address);
    //if found then assign the register value to the new value.
    if (reg)
    {
        reg->value = value;
        return true;
    }
    else
        return false;
}

uint16_t Modbus::Reg(uint16_t address)
{
    TRegister *reg;
    reg = this->searchRegister(address);
    if(reg)
        return(reg->value);
    else
        return(0);
}


TRegister *Modbus::searchIstsRegister(uint16_t address)
{
    TRegister *reg = _ists_regs_head;
    //if there is no register configured, bail
    if(reg == 0) return(0);
    //scan through the linked list until the end of the list or the register is found.
    //return the pointer.
    do
    {
        if (reg->address == address) return(reg);
        reg = reg->next;
    }
    while(reg);
    return(0);
}
void Modbus::addIstsReg(uint16_t address, uint16_t value)
{
    TRegister *newreg;

    newreg = (TRegister *) ebox_malloc(sizeof(TRegister));
    newreg->address = address;
    newreg->value		= value;
    newreg->next		= 0;

    if(_ists_regs_head == 0)
    {
        _ists_regs_head = newreg;
        _ists_regs_last = _ists_regs_head;
    }
    else
    {
        //Assign the last register's next pointer to newreg.
        _ists_regs_last->next = newreg;
        //then make temp the last register in the list.
        _ists_regs_last = newreg;
    }
}

bool Modbus::IstsReg(uint16_t address, uint16_t value)
{
    TRegister *reg;
    //search for the register address
    reg = this->searchIstsRegister(address);
    //if found then assign the register value to the new value.
    if (reg)
    {
        reg->value = value;
        return true;
    }
    else
        return false;
}

uint16_t Modbus::IstsReg(uint16_t address)
{
    TRegister *reg;
    reg = this->searchIstsRegister(address);
    if(reg)
        return(reg->value);
    else
        return(0);
}



//////////////////////////////////////////////////////////////////////
TRegister *Modbus::searchInputRegister(uint16_t address)
{
    TRegister *reg = _input_regs_head;
    //if there is no register configured, bail
    if(reg == 0) return(0);
    //scan through the linked list until the end of the list or the register is found.
    //return the pointer.
    do
    {
        if (reg->address == address) return(reg);
        reg = reg->next;
    }
    while(reg);
    return(0);
}
void Modbus::addInputReg(uint16_t address, uint16_t value)
{
    TRegister *newreg;

    newreg = (TRegister *) ebox_malloc(sizeof(TRegister));
    newreg->address = address;
    newreg->value		= value;
    newreg->next		= 0;

    if(_input_regs_head == 0)
    {
        _input_regs_head = newreg;
        _input_regs_last = _input_regs_head;
    }
    else
    {
        //Assign the last register's next pointer to newreg.
        _input_regs_last->next = newreg;
        //then make temp the last register in the list.
        _input_regs_last = newreg;
    }
}

bool Modbus::InputReg(uint16_t address, uint16_t value)
{
    TRegister *reg;
    //search for the register address
    reg = this->searchInputRegister(address);
    //if found then assign the register value to the new value.
    if (reg)
    {
        reg->value = value;
        return true;
    }
    else
        return false;
}

uint16_t Modbus::InputReg(uint16_t address)
{
    TRegister *reg;
    reg = this->searchInputRegister(address);
    if(reg)
        return(reg->value);
    else
        return(0);
}
//////////////////////////////////////////////////////////////////////////

void Modbus::addHreg(uint16_t offset, uint16_t value)
{
    this->addReg(offset + 40001, value);
}

bool Modbus::Hreg(uint16_t offset, uint16_t value)
{
    return Reg(offset + 40001, value);
}

uint16_t Modbus::Hreg(uint16_t offset)
{
    return Reg(offset + 40001);
}

#ifndef USE_HOLDING_REGISTERS_ONLY
void Modbus::addCoil(uint16_t offset, bool value)
{
    this->addReg(offset + 1, value ? 0xFF00 : 0x0000);
}

void Modbus::addIsts(uint16_t offset, bool value)
{
    this->addIstsReg(offset + 10001, value ? 0xFF00 : 0x0000);
}

void Modbus::addIreg(uint16_t offset, uint16_t value)
{
    this->addInputReg(offset + 30001, value);
}

bool Modbus::Coil(uint16_t offset, bool value)
{
    return Reg(offset + 1, value ? 0xFF00 : 0x0000);
}

bool Modbus::Ists(uint16_t offset, bool value)
{
    return IstsReg(offset + 10001, value ? 0xFF00 : 0x0000);
}

bool Modbus::Ireg(uint16_t offset, uint16_t value)
{
    return InputReg(offset + 30001, value);
}

bool Modbus::Coil(uint16_t offset)
{
    if (Reg(offset + 1) == 0xFF00)
    {
        return true;
    }
    else return false;
}

bool Modbus::Ists(uint16_t offset)
{
    if (IstsReg(offset + 10001) == 0xFF00)
    {
        return true;
    }
    else return false;
}

uint16_t Modbus::Ireg(uint16_t offset)
{
    return InputReg(offset + 30001);
}
#endif


void Modbus::receivePDU(byte *frame)
{
    byte fcode  = frame[0];
    uint16_t field1 = (uint16_t)frame[1] << 8 | (uint16_t)frame[2];
    uint16_t field2 = (uint16_t)frame[3] << 8 | (uint16_t)frame[4];

    
    if(callbackBefor != NULL){
        callbackBefor(fcode,field1,field2);
    }
    switch (fcode)
    {
    case MB_FC_WRITE_REG:
        //field1 = reg, field2 = value
        this->writeSingleRegister(field1, field2);
        break;

    case MB_FC_READ_REGS:
        //field1 = startreg, field2 = numregs
        this->readRegisters(field1, field2);
        break;

    case MB_FC_WRITE_REGS:
        //field1 = startreg, field2 = status
    this->writeMultipleRegisters(frame, field1, field2, frame[5]);
        break;

#ifndef USE_HOLDING_REGISTERS_ONLY
    case MB_FC_READ_COILS:
        //field1 = startreg, field2 = numregs
        this->readCoils(field1, field2);
        break;

    case MB_FC_READ_INPUT_STAT:
        //field1 = startreg, field2 = numregs
        this->readInputStatus(field1, field2);
        break;

    case MB_FC_READ_INPUT_REGS:
        //field1 = startreg, field2 = numregs
        this->readInputRegisters(field1, field2);
        break;

    case MB_FC_WRITE_COIL:
        //field1 = reg, field2 = status
        this->writeSingleCoil(field1, field2);
        break;

    case MB_FC_WRITE_COILS:
        //field1 = startreg, field2 = numoutputs
        this->writeMultipleCoils(frame, field1, field2, frame[5]);
        break;

#endif
    default:
        this->exceptionResponse(fcode, MB_EX_ILLEGAL_FUNCTION);
    }
    if(callback != NULL)
        callback(fcode);  
}

void Modbus::exceptionResponse(byte fcode, byte excode)
{
    //Clean frame buffer
    ebox_free(_frame);
    _len = 2;
    _frame = (byte *) ebox_malloc(_len);
    _frame[0] = fcode + 0x80;
    _frame[1] = excode;

    _reply = MB_REPLY_NORMAL;
}

void Modbus::readRegisters(uint16_t startreg, uint16_t numregs)
{
    //Check value (numregs)
    if (numregs < 0x0001 || numregs > 0x007D)
    {
        this->exceptionResponse(MB_FC_READ_REGS, MB_EX_ILLEGAL_VALUE);
        return;
    }

    //Check Address
    //*** See comments on readCoils method.
    if (!this->searchRegister(startreg + 40001))
    {
        this->exceptionResponse(MB_FC_READ_REGS, MB_EX_ILLEGAL_ADDRESS);
        return;
    }


    //Clean frame buffer
    ebox_free(_frame);
    _len = 0;

    //calculate the query reply message length
    //for each register queried add 2 bytes
    _len = 2 + numregs * 2;

    _frame = (byte *) ebox_malloc(_len);
    if (!_frame)
    {
        this->exceptionResponse(MB_FC_READ_REGS, MB_EX_SLAVE_FAILURE);
        return;
    }

    _frame[0] = MB_FC_READ_REGS;
    _frame[1] = _len - 2;   //byte count

    uint16_t val;
    uint16_t i = 0;
    while(numregs--)
    {
        //retrieve the value from the register bank for the current register
        val = this->Hreg(startreg + i);
        //write the high byte of the register value
        _frame[2 + i * 2]  = val >> 8;
        //write the low byte of the register value
        _frame[3 + i * 2] = val & 0xFF;
        i++;
    }

    _reply = MB_REPLY_NORMAL;
}

void Modbus::writeSingleRegister(uint16_t reg, uint16_t value)
{
    //No necessary verify illegal value (EX_ILLEGAL_VALUE) - because using uint16_t (0x0000 - 0x0FFFF)
    //Check Address and execute (reg exists?)
    if (!this->Hreg(reg, value))
    {
        this->exceptionResponse(MB_FC_WRITE_REG, MB_EX_ILLEGAL_ADDRESS);
        return;
    }

    //Check for failure
    if (this->Hreg(reg) != value)
    {
        this->exceptionResponse(MB_FC_WRITE_REG, MB_EX_SLAVE_FAILURE);
        return;
    }

    _reply = MB_REPLY_ECHO;
}
void Modbus::writeMultipleRegisters(byte *frame, uint16_t startreg, uint16_t numoutputs, byte bytecount)
{
    byte *buffer;
    uint16_t size = ebox_get_sizeof_ptr(frame);
    buffer = (byte *)ebox_malloc(size);
    memcpy(buffer,frame,size);
    //Check value
    if (numoutputs < 0x0001 || numoutputs > 0x007B || bytecount != 2 * numoutputs)
    {
        this->exceptionResponse(MB_FC_WRITE_REGS, MB_EX_ILLEGAL_VALUE);
        return;
    }

    //Check Address (startreg...startreg + numregs)
    for (int k = 0; k < numoutputs; k++)
    {
        if (!this->searchRegister(startreg + 40001 + k))
        {
            this->exceptionResponse(MB_FC_WRITE_REGS, MB_EX_ILLEGAL_ADDRESS);
            return;
        }
    }

    //Clean frame buffer
    ebox_free(_frame);
    _len = 5;
    _frame = (byte *) ebox_malloc(_len);
    if (!_frame)
    {
        this->exceptionResponse(MB_FC_WRITE_REGS, MB_EX_SLAVE_FAILURE);
        return;
    }
    _frame[0] = MB_FC_WRITE_REGS;
    _frame[1] = startreg >> 8;
    _frame[2] = startreg & 0x00FF;
    _frame[3] = numoutputs >> 8;
    _frame[4] = numoutputs & 0x00FF;

    uint16_t val;
    uint16_t i = 0;
    while(numoutputs--)
    {
        val = (((uint16_t)buffer[6 + i * 2]) << 8 | (uint16_t)buffer[7 + i * 2]);
        this->Hreg(startreg + i, val);
        i++;
    }
    ebox_free(buffer);
    _reply = MB_REPLY_NORMAL;
}

#ifndef USE_HOLDING_REGISTERS_ONLY
void Modbus::readCoils(uint16_t startreg, uint16_t numregs)
{
    //Check value (numregs)
    if (numregs < 0x0001 || numregs > 0x07D0)
    {
        this->exceptionResponse(MB_FC_READ_COILS, MB_EX_ILLEGAL_VALUE);
        return;
    }

    //Check Address
    //Check only startreg. Is this correct?
    //When I check all registers in range I got errors in ScadaBR
    //I think that ScadaBR request more than one in the single request
    //when you have more then one datapoint configured from same type.
    if (!this->searchRegister(startreg + 1))
    {
        this->exceptionResponse(MB_FC_READ_COILS, MB_EX_ILLEGAL_ADDRESS);
        return;
    }

    //Clean frame buffer
    ebox_free(_frame);
    _len = 0;

    //Determine the message length = function type, byte count and
    //for each group of 8 registers the message length increases by 1
    _len = 2 + numregs / 8;
    if (numregs % 8) _len++; //Add 1 to the message length for the partial byte.

    _frame = (byte *) ebox_malloc(_len);
    if (!_frame)
    {
        this->exceptionResponse(MB_FC_READ_COILS, MB_EX_SLAVE_FAILURE);
        return;
    }

    _frame[0] = MB_FC_READ_COILS;
    _frame[1] = _len - 2; //byte count (_len - function code and byte count)

    byte bitn = 0;
    uint16_t totregs = numregs;
    uint16_t i;
    while (numregs)
    {
        i = (totregs - numregs) / 8;
        if (this->Coil(startreg))
            bitSet(_frame[2 + i], bitn);
        else
            bitClear(_frame[2 + i], bitn);
        //increment the bit index
        bitn++;
        if (bitn == 8) bitn = 0;
        //increment the register
        startreg++;
        numregs--;
    }

    _reply = MB_REPLY_NORMAL;
}

void Modbus::readInputStatus(uint16_t startreg, uint16_t numregs)
{
    //Check value (numregs)
    if (numregs < 0x0001 || numregs > 0x07D0)
    {
        this->exceptionResponse(MB_FC_READ_INPUT_STAT, MB_EX_ILLEGAL_VALUE);
        return;
    }

    //Check Address
    //*** See comments on readCoils method.
    if (!this->searchIstsRegister(startreg + 10001))
    {
        this->exceptionResponse(MB_FC_READ_COILS, MB_EX_ILLEGAL_ADDRESS);
        return;
    }

    //Clean frame buffer
    ebox_free(_frame);
    _len = 0;

    //Determine the message length = function type, byte count and
    //for each group of 8 registers the message length increases by 1
    _len = 2 + numregs / 8;
    if (numregs % 8) _len++; //Add 1 to the message length for the partial byte.

    _frame = (byte *) ebox_malloc(_len);
    if (!_frame)
    {
        this->exceptionResponse(MB_FC_READ_INPUT_STAT, MB_EX_SLAVE_FAILURE);
        return;
    }

    _frame[0] = MB_FC_READ_INPUT_STAT;
    _frame[1] = _len - 2;

    byte bitn = 0;
    uint16_t totregs = numregs;
    uint16_t i;
    while (numregs)
    {
        i = (totregs - numregs) / 8;
        if (this->Ists(startreg))
            bitSet(_frame[2 + i], bitn);
        else
            bitClear(_frame[2 + i], bitn);
        //increment the bit index
        bitn++;
        if (bitn == 8) bitn = 0;
        //increment the register
        startreg++;
        numregs--;
    }

    _reply = MB_REPLY_NORMAL;
}

void Modbus::readInputRegisters(uint16_t startreg, uint16_t numregs)
{
    //Check value (numregs)
    if (numregs < 0x0001 || numregs > 0x007D)
    {
        this->exceptionResponse(MB_FC_READ_INPUT_REGS, MB_EX_ILLEGAL_VALUE);
//        MB_DBG("err numregs = %d",numregs);
        return;
    }

    //Check Address
    //*** See comments on readCoils method.
    if (!this->searchInputRegister(startreg + 30001))
    {
        this->exceptionResponse(MB_FC_READ_COILS, MB_EX_ILLEGAL_ADDRESS);
//        MB_DBG("err startreg = %d",startreg);
        return;
    }

    //Clean frame buffer
    ebox_free(_frame);
    _len = 0;

    //calculate the query reply message length
    //for each register queried add 2 bytes
    _len = 2 + numregs * 2;

    _frame = (byte *) ebox_malloc(_len);
    if (!_frame)
    {
        this->exceptionResponse(MB_FC_READ_INPUT_REGS, MB_EX_SLAVE_FAILURE);
        return;
    }

    _frame[0] = MB_FC_READ_INPUT_REGS;
    _frame[1] = _len - 2;

    uint16_t val;
    uint16_t i = 0;
    while(numregs--)
    {
        //retrieve the value from the register bank for the current register
        val = this->Ireg(startreg + i);
        //write the high byte of the register value
        _frame[2 + i * 2]  = val >> 8;
        //write the low byte of the register value
        _frame[3 + i * 2] = val & 0xFF;
        i++;
    }

    _reply = MB_REPLY_NORMAL;
}

void Modbus::writeSingleCoil(uint16_t reg, uint16_t status)
{
//    MB_DBG("reg:%d,status:%d",reg,status);
    //Check value (status)
    if (status != 0xFF00 && status != 0x0000)
    {
        this->exceptionResponse(MB_FC_WRITE_COIL, MB_EX_ILLEGAL_VALUE);
        return;
    }

    //Check Address and execute (reg exists?)
    if (!this->Coil(reg, (bool)status))
    {
        this->exceptionResponse(MB_FC_WRITE_COIL, MB_EX_ILLEGAL_ADDRESS);
        return;
    }

    //Check for failure
    if (this->Coil(reg) != (bool)status)
    {
        this->exceptionResponse(MB_FC_WRITE_COIL, MB_EX_SLAVE_FAILURE);
        return;
    }

    _reply = MB_REPLY_ECHO;
}

void Modbus::writeMultipleCoils(byte *frame, uint16_t startreg, uint16_t numoutputs, byte bytecount)
{
//    MB_DBG("startreg:%d\tnumoutputs:%d\tbytecount:%d\n",startreg,numoutputs,bytecount);
    byte *buffer;
    uint16_t size =  ebox_get_sizeof_ptr(frame);
    buffer = (byte *)ebox_malloc(size);
    memcpy(buffer,frame,size);
    
    //Check value
    uint16_t bytecount_calc = numoutputs / 8;
    if (numoutputs % 8) bytecount_calc++;
    if (numoutputs < 0x0001 || numoutputs > 0x07B0 || bytecount != bytecount_calc)
    {
        this->exceptionResponse(MB_FC_WRITE_COILS, MB_EX_ILLEGAL_VALUE);
        return;
    }

    //Check Address (startreg...startreg + numregs)
    for (int k = 0; k < numoutputs; k++)
    {
        if (!this->searchRegister(startreg + 1 + k))
        {
            this->exceptionResponse(MB_FC_WRITE_COILS, MB_EX_ILLEGAL_ADDRESS);
            return;
        }
    }

    //Clean frame buffer
    ebox_free(_frame);
    _len = 5;
    _frame = (byte *) ebox_malloc(_len);
    if (!_frame)
    {
        this->exceptionResponse(MB_FC_WRITE_COILS, MB_EX_SLAVE_FAILURE);
        return;
    }

    _frame[0] = MB_FC_WRITE_COILS;
    _frame[1] = startreg >> 8;
    _frame[2] = startreg & 0x00FF;
    _frame[3] = numoutputs >> 8;
    _frame[4] = numoutputs & 0x00FF;

    byte bitn = 0;
    uint16_t totoutputs = numoutputs;
    uint16_t i;
    while (numoutputs--)
    {
        i = (totoutputs - numoutputs) / 8;
        this->Coil(startreg, bitRead(buffer[6 + i], bitn));
//        MB_DBG("bit %d: 0x%02x\t",i,buffer[6 + i]);
//        MB_DBG("bit %d: 0x%02x\t",i,frame[6 + i]);
//        MB_DBG("bit %d: %d\n",i,bitRead(buffer[6 + i], bitn));
        //increment the bit index
        bitn++;
        if (bitn == 8) bitn = 0;
        //increment the register
        startreg++;
    }
    ebox_free(buffer);
    _reply = MB_REPLY_NORMAL;

}
#endif



