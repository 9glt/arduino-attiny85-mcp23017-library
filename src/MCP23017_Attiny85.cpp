#include "MCP23017_Attiny85.h"
#include <TinyWireM.h>

MCP23017::MCP23017(uint8_t addr)
{
    i2c_addr = addr;
}

void MCP23017::begin()
{
    TinyWireM.begin();

    send(IOCON, 0x0); // default config: seauential addressing, separate INT

    send(IPOLA, 0x0); // GPA register bit will reflect the same logic state of the input pin.
    send(IPOLB, 0x0); // GPB register bit will reflect the same logic state of the input pin.

    send(DEFVALA, 0xFF); // set GPA default compare value to 1 (normally pulled high)
    send(DEFVALB, 0xFF); // set GPB default compare value to 1 (normally pulled high)
}

void MCP23017::setInterrupt(uint8_t pin, bool enabled)
{
    pin = fixPin(pin);
    if(enabled) bitSet(iot, pin);
    else bitClear(iot, pin);
    send(GPINTENA, iot >> 8); // set GPA interrupt on change
    send(GPINTENA, iot & 0xFF); // set GPA interrupt on change
}

void MCP23017::pinMode(uint8_t pin, uint8_t mode)
{
    pin = fixPin(pin);

    if (mode == MCP_OUTPUT)
    {
        bitClear(io_dir, pin);
        bitClear(io_pull, pin);
    }
    else
    {
        bitSet(io_dir, pin);
        bitSet(io_pull, pin);
    }

    send(IODIRA, io_dir >> 8);
    send(IODIRB, io_dir & 0xFF);

    send(GPPUA, io_pull >> 8);
    send(GPPUB, io_pull & 0xFF);
}

bool MCP23017::digitalRead(uint8_t pin)
{
    return (readGPIO() & (1 << pin)) > 0;
}

void MCP23017::digitalWrite(uint8_t pin, uint8_t value)
{
    if (value)
        bitSet(io, pin);
    else
        bitClear(io, pin);
    writeGPIO(io);
}

uint16_t MCP23017::readGPIO()
{
    return read(GPIOB) << 8 | read(GPIOA);
}

void MCP23017::writeGPIO(uint16_t value)
{
    io = value;
    TinyWireM.beginTransmission(i2c_addr);
    TinyWireM.write(GPIOA);
    TinyWireM.write(io & 0xFF);
    TinyWireM.endTransmission(true);

    TinyWireM.beginTransmission(i2c_addr);
    TinyWireM.write(GPIOB);
    TinyWireM.write(io >> 8);
    TinyWireM.endTransmission(true);
}

uint16_t MCP23017::read(uint8_t addr)
{
    TinyWireM.beginTransmission(i2c_addr);
    TinyWireM.write(addr);
    TinyWireM.endTransmission(true);    // Send 1 byte to the slave
    TinyWireM.requestFrom(i2c_addr, 1); // Request 2 byte from slave
    return TinyWireM.read();
}

void MCP23017::send(uint8_t addr, uint8_t data, bool stop)
{
    TinyWireM.beginTransmission(i2c_addr);
    TinyWireM.write(addr);
    TinyWireM.write(data);
    TinyWireM.endTransmission(stop);
}

uint8_t MCP23017::fixPin(uint8_t pin)
{
    if (pin > 7)
        pin -= 8;
    else
        pin += 8;
    return pin;
}