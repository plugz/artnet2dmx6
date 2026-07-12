/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
                                          
    gpio_MCP23S17 - A complete library for Microchip MCP23S17 for many MCU's.
    
model:            company:        pins:        protocol:        Special Features:
---------------------------------------------------------------------------------------------------------------------
mcp23s17        Microchip         16            SPI                    INT/HAEN
---------------------------------------------------------------------------------------------------------------------
Version history:
0.9 Fixed an example, added getInterruptNumber function.
---------------------------------------------------------------------------------------------------------------------
        Copyright (c) 2013-2014, s.u.m.o.t.o.y [sumotoy(at)gmail.com]
---------------------------------------------------------------------------------------------------------------------

    gpio_MCP23S17 Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    gpio_MCP23S17 Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

/* ------------------------------ MCP23S17 WIRING ------------------------------------
This chip has a very useful feature called HAEN that allow you to share the same CS pin trough
8 different addresses. Of course chip has to be Microchip and should be assigned to different addresses!

Basic Address:  00100 A2 A1 A0 (from 0x20 to 0x27)
A2,A1,A0 tied to ground = 0x20
                __ __
        IOB-0 [|  U  |] IOA-7
        IOB-1 [|     |] IOA-6
        IOB-2 [|     |] IOA-5
        IOB-3 [|     |] IOA-4
        IOB-4 [|     |] IOA-3
        IOB-5 [|     |] IOA-2
        IOB-6 [|     |] IOA-1
        IOB-7 [|     |] IOA-0
        ++++  [|     |] INT-A
        GND   [|     |] INT-B
        CS    [|     |] RST (connect to +)
        SCK   [|     |] A2
        MOSI  [|     |] A1
        MISO  [|_____|] A0
*/

#ifndef __MCP23S17_HPP__
#define __MCP23S17_HPP__

#include "gpio.h"

#include <cstdint>

// registers
static constexpr uint8_t MCP23S17_IODIR =    0x00;
static constexpr uint8_t MCP23S17_IODIRA =   0x00;
static constexpr uint8_t MCP23S17_IODIRB =   0x01;
static constexpr uint8_t MCP23S17_IPOL =     0x02;
static constexpr uint8_t MCP23S17_GPINTEN =  0x04;
static constexpr uint8_t MCP23S17_DEFVAL =   0x06;
static constexpr uint8_t MCP23S17_INTCON =   0x08;
static constexpr uint8_t MCP23S17_IOCON =    0x0A;
static constexpr uint8_t MCP23S17_GPPU =     0x0C;
static constexpr uint8_t MCP23S17_GPPUA =    0x0C;
static constexpr uint8_t MCP23S17_GPPUB =    0x0D;
static constexpr uint8_t MCP23S17_INTF =     0x0E;
static constexpr uint8_t MCP23S17_INTCAP =   0x10;
static constexpr uint8_t MCP23S17_GPIO =     0x12;
static constexpr uint8_t MCP23S17_GPIOA =    0x12;
static constexpr uint8_t MCP23S17_GPIOB =    0x13;
static constexpr uint8_t MCP23S17_OLAT =     0x14;

static constexpr uint32_t MCP23S17_MAX_SPEED = 10000000UL;

/*   The IOCON register!
     .       7    6      5     4      3    2   1      0
     IOCON = BANK MIRROR SEQOP DISSLW HAEN ODR INTPOL -NC-
     -----------------------------------------------------------------------
     0b01101100
     BANK: (Controls how the registers are addressed)
     1 The registers associated with each port are separated into different banks
     0 The registers are in the same bank (addresses are sequential)
     MIRROR: (INT Pins Mirror bit)
     1 The INT pins are internally connected
     0 The INT pins are not connected. INTA is associated with PortA and INTB is associated with PortB
     SEQOP: (Sequential Operation mode bit)
     1 Sequential operation disabled, address pointer does not increment
     0 Sequential operation enabled, address pointer increments.
     DISSLW: (Slew Rate control bit for SDA output, only I2C)
     HAEN: (Hardware Address Enable bit, SPI only)
     1 Enables the MCP23S17 address pins
     0 Disables the MCP23S17 address pins
     ODR: (This bit configures the INT pin as an open-drain output)
     1 Open-drain output (overrides the INTPOL bit).
     0 Active driver output (INTPOL bit sets the polarity).
     INTPOL: (This bit sets the polarity of the INT output pin)
     1 Active high
     0 Active low
     */

class MCP23S17 {
    public:
public:


    MCP23S17();//For include inside other libraries

    // haenAddr is 0-7
    void             setup(GPIO_TypeDef* csPinPeripheral, uint16_t csPin, SPI_HandleTypeDef* spiHandle, uint8_t haenAddr);//used with other libraries only

    void             begin();

    void             gpioPinMode(uint16_t mode);                    //OUTPUT=all out,INPUT=all in,0xxxx=you choose, 1 is IN, 0 is OUT
    void             gpioPinModePortA(bool mode);
    void             gpioPinModePortB(bool mode);
    void             writeGpioPort(uint16_t value);                    //HIGH=all Hi, LOW=all Low,0xxxx=you choose witch low or hi
    void             writeGpioPortA(uint8_t value);
    void             writeGpioPortB(uint8_t value);
    uint16_t         readGpioPort();                                //read the state of the pins (all)
    uint8_t          readGpioPortA();
    uint8_t          readGpioPortB();
    uint16_t         readGpioPortFast(); // return cached values

    void             gpioDigitalWrite(uint8_t pin, bool value);  //write data to one pin
    void             gpioDigitalWritePortA(uint8_t pin, bool value);  //write data to one pin
    void             gpioDigitalWritePortB(uint8_t pin, bool value);  //write data to one pin
    void             setPullup(uint16_t data);                        // HIGH=all pullup, LOW=all pulldown,0xxxx=you choose witch
    void             setPullupPortA(bool pullup);
    void             setPullupPortB(bool pullup);

    void             gpioUpdate();
    void             gpioUpdatePortA();
    void             gpioUpdatePortB();

protected:


    inline void _GPIOstartSend(void) {
        HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_RESET);
    }


    inline void _GPIOendSend(void) {
        HAL_GPIO_WritePin(_csPinPeripheral, _csPin, GPIO_PIN_SET);
    }

    inline uint8_t _gpioReadByte(uint8_t addr) {
        _GPIOstartSend();
        uint8_t txBuff[2];
        uint8_t rxBuff[1] = {0,};
        txBuff[0] = _readCmd;
        txBuff[1] = addr;
        HAL_SPI_Transmit(_spiHandle, txBuff, sizeof(txBuff), HAL_MAX_DELAY);
        HAL_SPI_Receive(_spiHandle, rxBuff, sizeof(rxBuff), HAL_MAX_DELAY);
        _GPIOendSend();

        return rxBuff[0];
    }

    inline uint16_t _gpioReadWord(uint8_t addr) {
        _GPIOstartSend();
        uint8_t txBuff[2];
        uint8_t rxBuff[2] = {0,};
        txBuff[0] = _readCmd;
        txBuff[1] = addr;
        HAL_SPI_Transmit(_spiHandle, txBuff, sizeof(txBuff), HAL_MAX_DELAY);
        HAL_SPI_Receive(_spiHandle, rxBuff, sizeof(rxBuff), HAL_MAX_DELAY);
        _GPIOendSend();

        return ((uint16_t)rxBuff[0] << 8) | (uint16_t)rxBuff[1];
    }

    inline void _gpioWriteByte(uint8_t addr, uint8_t data) {
        _GPIOstartSend();
        uint8_t buff[3];
        buff[0] = _writeCmd;
        buff[1] = addr;
        buff[2] = data;
        HAL_SPI_Transmit(_spiHandle, buff, sizeof(buff), HAL_MAX_DELAY);
        _GPIOendSend();
    }

    template<unsigned int SIZE>
    inline void _gpioWriteBytes(uint8_t addr, uint8_t const* data) {
        _GPIOstartSend();
        uint8_t buff[2 + SIZE];
        buff[0] = _writeCmd;
        buff[1] = addr;
        for (unsigned int i = 0; i < SIZE; ++i)
            buff[2 + i] = data[i];
        HAL_SPI_Transmit(_spiHandle, buff, sizeof(buff), HAL_MAX_DELAY);
        _GPIOendSend();
    }

    inline void _gpioWriteWord(uint8_t addr, uint16_t data) {
        _GPIOstartSend();
        uint8_t buff[4];
        buff[0] = _writeCmd;
        buff[1] = addr;
        buff[2] = (uint8_t)(data >> 8);
        buff[3] = (uint8_t)data;
        HAL_SPI_Transmit(_spiHandle, buff, sizeof(buff), HAL_MAX_DELAY);
        _GPIOendSend();
    }


private:
    GPIO_TypeDef*      _csPinPeripheral;
    uint16_t           _csPin;
    SPI_HandleTypeDef* _spiHandle;
    uint8_t            _addr;


    uint8_t         _readCmd;
    uint8_t         _writeCmd;
    uint16_t        _gpioDirection;
    uint16_t        _gpioState;
};

#endif
