#include "MCP23017.h"
#include <TinyWireM.h>

MCP23017::MCP23017(uint8_t addr)
{
    i2c_addr = addr;
}

void MCP23017::begin()
{
    TinyWireM.begin();
    delay(10);
    send(IOCON, 0x0); // default config: seauential addressing, separate INT
                      //   send(IODIRA, 0xFF);   // set all GPA to input
                      //   send(IODIRB, 0x0);    // set all GPB to output
    send(IPOLA, 0x0); // GPA register bit will reflect the same logic state of the input pin.
    send(IPOLB, 0x0); // GPB register bit will reflect the same logic state of the input pin.
                      //   send(GPPUA, 0xFF);    // enable pull-ups for GPA
    // send(GPPUA, 0x0); // enable pull-ups for GPA
    // send(GPPUB, 0x0); // disable pull-ups for GPB

    //   send(INTCONA, 0xFF);  // compare GPA to DEFVAL
    send(DEFVALA, 0xFF); // set GPA default compare value to 1 (normally pulled high)
    send(DEFVALB, 0xFF); // set GPA default compare value to 1 (normally pulled high)
    //   send(GPINTENA, 0xFF); // set GPA interrupt on change
}

void MCP23017::pinMode(uint8_t pin, uint8_t mode)
{
    if (mode == MCP_OUTPUT)
    {
        bitClear(io_dir, pin);
        bitClear(io_pull, pin);
    }

    send(IODIRA, io_dir >> 8);
    send(IODIRB, io_dir & 0xFF);

    send(GPPUA, io_pull >> 8);
    send(GPPUB, io_pull & 0xFF);
}

uint8_t MCP23017::digitalRead(uint8_t pin)
{
    return read(GPIOA) & 1 << pin;
}

void MCP23017::digitalWrite(uint8_t pin, uint8_t value)
{
    if(value) 
        bitSet(io, pin);
    else 
        bitClear(io, pin);
    writeGPIO(io);
}

uint16_t MCP23017::readGPIO()
{
    return read(GPIOA);
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
    TinyWireM.write(GPIOA);
    TinyWireM.endTransmission();        // Send 1 byte to the slave
    TinyWireM.requestFrom(i2c_addr, 2); // Request 2 byte from slave
    return TinyWireM.read() << 8 | TinyWireM.read();
}

void MCP23017::send(uint8_t addr, uint8_t data, bool stop)
{
    TinyWireM.beginTransmission(i2c_addr);
    TinyWireM.write(addr);
    TinyWireM.write(data);
    TinyWireM.endTransmission(stop);
}

uint16_t MCP23017::getRegister(uint8_t baseAddress, uint8_t port)
{
    uint16_t reg = baseAddress;
    // reg *= 2;
    if (port > 7)
        reg++;
    return reg;
}