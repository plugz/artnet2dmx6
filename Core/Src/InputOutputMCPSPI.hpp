#ifndef __INPUTOUTPUTMCPSPI_HPP__
#define __INPUTOUTPUTMCPSPI_HPP__

#include "MCP23S17.hpp"

#include <cstdint>

// Bank A input, bank B output

class InputOutputMCPSPI : public MCP23S17
{
public:
    void setup(Pin const& csPin, SPI_HandleTypeDef* spiHandle, uint8_t haenAddr);
    void tick();

    void setInterruptPin(Pin const& pin);

    void read();
    uint8_t getCurrentValue() const;
    bool getCurrentValue(uint8_t pin) const;

    void setValue(uint8_t pin, bool value);
    bool needWrite() const { return _needWrite; }
    void write();

private:
    uint8_t _currentReadValue = 0xff;
    uint8_t _currentWriteValue = 0x00;
    bool _needWrite = false;

    Pin _interruptPin;
};

#endif
