#include "InputOutputMCPSPI.hpp"

#include "Chrono.hpp"

#include "spi.h"

void InputOutputMCPSPI::setup(GPIO_TypeDef* csPinPeripheral, uint16_t csPin, SPI_HandleTypeDef* spiHandle, uint8_t haenAddr) {
    MCP23S17::setup(csPinPeripheral, csPin, spiHandle, haenAddr);
    MCP23S17::begin();
    setPullupPortA(true); // enable pullup on port A
    gpioPinModePortB(false); // enable output mode on port B

    Chrono::delay(Milliseconds(2)); // let pullups do their job
    read();
    write();
}

void InputOutputMCPSPI::tick() {
    if (_needWrite) {
        write();
        return;
    }
    read();
}

void InputOutputMCPSPI::read() {
    _currentReadValue = readGpioPortA();
}

uint8_t InputOutputMCPSPI::getCurrentValue() const {
    return _currentReadValue;
}

bool InputOutputMCPSPI::getCurrentValue(uint8_t pin) const {
    return !!(_currentReadValue & (1 << pin));
}

void InputOutputMCPSPI::setValue(uint8_t pin, bool value) {
    if (value)
        _currentWriteValue |= (1 << pin);
    else
        _currentWriteValue &= ~(1 << pin);
    _needWrite = true;
}

void InputOutputMCPSPI::write() {
    writeGpioPortB(_currentWriteValue);
    _needWrite = false;
}
