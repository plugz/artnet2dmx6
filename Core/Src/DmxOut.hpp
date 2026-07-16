#ifndef __DMXOUT_HPP__
#define __DMXOUT_HPP__

#include "Chrono.hpp"

#include <cstdint>

template<int TUart>
class DmxOut {
public:
    DmxOut();

    void init();

    void tick();

    void setTransmitDone() { _transmitDone = true; }

    uint8_t* buffer() { return _fullBuffer + 1; }

private:
    unsigned int _currentStep = 0;
    bool _transmitDone;
    Chrono::UsTimer _timer;

    uint8_t _fullBuffer[513];

    uint8_t _lastHalCmdStatus = 0xff;
};

#endif
