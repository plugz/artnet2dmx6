#include "MCP23S17.hpp"

#include "Chrono.hpp"

#include <initializer_list>

static uint8_t s_haenEnabledCSPins[32] = {0,};

MCP23S17::MCP23S17(){
}

void MCP23S17::setup(Pin const& csPin, SPI_HandleTypeDef* spiHandle, uint8_t haenAddr){
    _csPin = csPin;
    _spiHandle = spiHandle;
    if (haenAddr > 7)
        haenAddr = 7;
    _addr = haenAddr + 0x20;
    _readCmd =  (_addr << 1) | 1;
    _writeCmd = _addr << 1;
}

void MCP23S17::begin() {
    HAL_GPIO_WritePin(_csPin.port, _csPin.pin, GPIO_PIN_SET);
    Chrono::delay(Chrono::Microseconds{2}); // Datasheet says 100ns CS disable time

    if (!s_haenEnabledCSPins[_csPin.pin])
    {
        // See https://www.electro-tech-online.com/threads/how-to-init-multiple-mcp23s17.126668/
        // -> Send message to set IOCON.HAEN (0bxxxxXxxx) to 1
        // send it to address 0 and to address 4
        //
        // With HAEN enabled, it's possible to have,
        // on the same spi wire, 8 MCP23S17 per CS pin.
        for (uint8_t tempAddr : {0, 4})
        {
            MCP23S17 temp;
            temp.setup(_csPin, _spiHandle, tempAddr);
            // BANK MIRROR SEQOP DISSLW HAEN ODR INTPOL -NC-
            // 0    0      1     0      1    0   0      0
            temp._gpioWriteByte(MCP23S17_IOCON, 0b00101000);
        }

        s_haenEnabledCSPins[_csPin.pin] = true;
    }
    _gpioDirection = 0xFFFF; //all in
    _gpioState = 0xFFFF; //all low
}

void MCP23S17::gpioPinMode(uint16_t mode){
    _gpioDirection = mode;
    _gpioState &= mode;
    _gpioWriteWord(MCP23S17_IODIR, _gpioDirection);
}

void MCP23S17::gpioPinModePortA(bool mode) {
    if (mode) {
        _gpioDirection |= 0xff00;
        _gpioWriteByte(MCP23S17_IODIRA, 0xff);
    }
    else {
        _gpioDirection &= 0x00ff;
        _gpioState &= 0x00ff;
        _gpioWriteByte(MCP23S17_IODIRA, 0x00);
    }
}

void MCP23S17::gpioPinModePortB(bool mode) {
    if (mode) {
        _gpioDirection |= 0x00ff;
        _gpioWriteByte(MCP23S17_IODIRB, 0xff);
    }
    else {
        _gpioDirection &= 0xff00;
        _gpioState &= 0xff00;
        _gpioWriteByte(MCP23S17_IODIRB, 0x00);
    }
}

void MCP23S17::writeGpioPort(uint16_t value) {
    _gpioState = value;
    _gpioWriteWord(MCP23S17_GPIO, _gpioState);
}

void MCP23S17::writeGpioPortA(uint8_t value) {
    _gpioState = (_gpioState & 0xff) | ((uint16_t)value << 8);
    _gpioWriteWord(MCP23S17_GPIOA, value);
}

void MCP23S17::writeGpioPortB(uint8_t value) {
    _gpioState = (_gpioState & 0xff00) | ((uint16_t)value);
    _gpioWriteWord(MCP23S17_GPIOB, value);
}

uint16_t MCP23S17::readGpioPort() {
    return _gpioReadWord(MCP23S17_GPIO);
}

uint8_t MCP23S17::readGpioPortA() {
    return _gpioReadByte(MCP23S17_GPIOA);
}

uint8_t MCP23S17::readGpioPortB() {
    return _gpioReadByte(MCP23S17_GPIOB);
}

uint16_t MCP23S17::readGpioPortFast() {
    return _gpioState;
}

void MCP23S17::setPullup(uint16_t data) {
    _gpioState = data;
    _gpioWriteWord(MCP23S17_GPPU, _gpioState);
}

void MCP23S17::setPullupPortA(bool pullup) {
    if (pullup) {
        _gpioState |= 0xff00;
        _gpioWriteByte(MCP23S17_GPPUA, 0xff);
    }
    else {
        _gpioState &= 0x00ff;
        _gpioWriteByte(MCP23S17_GPPUA, 0x00);
    }
}

void MCP23S17::setPullupPortB(bool pullup) {
    if (pullup) {
        _gpioState |= 0x00ff;
        _gpioWriteByte(MCP23S17_GPPUB, 0xff);
    }
    else {
        _gpioState &= 0xff00;
        _gpioWriteByte(MCP23S17_GPPUB, 0x00);
    }
}

void MCP23S17::setInterruptOnChangePortA(bool enable) {
    if (enable) {
        _gpioWriteByte(MCP23S17_GPINTENA, 0xff);
        _gpioWriteByte(MCP23S17_INTCONA, 0x00);
    }
    else {
        _gpioWriteByte(MCP23S17_GPINTENA, 0x00);
    }
}

void MCP23S17::setInterruptOnChangePortB(bool enable) {
    if (enable) {
        _gpioWriteByte(MCP23S17_GPINTENB, 0xff);
        _gpioWriteByte(MCP23S17_INTCONB, 0x00);
    }
    else {
        _gpioWriteByte(MCP23S17_GPINTENB, 0x00);
    }
}

void MCP23S17::gpioDigitalWrite(uint8_t pin, bool value) {
    if (pin < 8) {
        return gpioDigitalWritePortA(pin, value);
    }
    if (pin < 16) {
        return gpioDigitalWritePortB(pin - 8, value);
    }
}

void MCP23S17::gpioDigitalWritePortA(uint8_t pin, bool value) {
    if (pin >= 8)
        return;
    if (value) {
        _gpioState |= (1 << (pin + 8));
    }
    else {
        _gpioState &= ~(1 << (pin + 8));
    }
    gpioUpdatePortA();
}

void MCP23S17::gpioDigitalWritePortB(uint8_t pin, bool value) {
    if (pin >= 8)
        return;
    if (value) {
        _gpioState |= (1 << pin);
    }
    else {
        _gpioState &= ~(1 << pin);
    }
    gpioUpdatePortB();
}

void MCP23S17::gpioUpdate() {
    _gpioWriteWord(MCP23S17_GPIO, _gpioState);
}

void MCP23S17::gpioUpdatePortA() {
    _gpioWriteByte(MCP23S17_GPIOA, _gpioState);
}

void MCP23S17::gpioUpdatePortB() {
    _gpioWriteByte(MCP23S17_GPIOB, _gpioState);
}
