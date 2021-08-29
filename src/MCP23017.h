#include <Arduino.h>
#include <TinyWireM.h>

#define MCP_PULLUP 0x01
#define MCP_OUTPUT 0x00

const uint8_t IOCON = 0x0A;

const uint8_t IODIRA = 0x00;
const uint8_t IODIRB = 0x01;
const uint8_t IPOLA = 0x02;
const uint8_t IPOLB = 0x03;
const uint8_t GPINTENA = 0x04;
const uint8_t GPINTENB = 0x05;
const uint8_t DEFVALA = 0x06;
const uint8_t DEFVALB = 0x07;
const uint8_t INTCONA = 0x08;
const uint8_t INTCONB = 0x09;

const uint8_t GPPUA = 0x0C;
const uint8_t GPPUB = 0x0D;
const uint8_t INTFA = 0x0E;
const uint8_t INTFB = 0x0F;
const uint8_t INTCAPA = 0x10;
const uint8_t INTCAPB = 0x11;
const uint8_t GPIOA = 0x12;
const uint8_t GPIOB = 0x13;
const uint8_t OLATA = 0x14;
const uint8_t OLATB = 0x15;

class MCP23017
{
public:
    MCP23017(uint8_t addr = 0x20);

    void begin();

    void pinMode(uint8_t pin, uint8_t mode);
    bool digitalRead(uint8_t pin);
    void digitalWrite(uint8_t pin, uint8_t value);

    void setInterrupt(uint8_t pin, bool enabled);

    uint16_t readGPIO();
    void writeGPIO(uint16_t value);

private:
    uint8_t i2c_addr;
    uint16_t io_dir = 0;
    uint16_t io_pull = 0;
    uint16_t io = 0;
    uint16_t iot = 0;

    void send(uint8_t addr, uint8_t data, bool stop = true);
    uint16_t read(uint8_t addr);
    uint8_t fixPin(uint8_t pin);
};