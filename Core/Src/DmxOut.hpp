#ifndef __DMXOUT_HPP__
#define __DMXOUT_HPP__

#include "Chrono.hpp"
#include "Packet.hpp"

#include <cstdint>

template<int TUart>
class DmxOut {
public:
    DmxOut();
    ~DmxOut();

    void init();

    void tick();

    void sendDmx(Packet const& dmxPacket);

    void setTransmitDone() { _transmitDone = true; }

private:
    unsigned int _currentStep = 0;
    bool _transmitDone;
    Chrono::UsTimer _timer;

    Packet _buffers[2];
    unsigned int _currentBufferIdx = 0; // currently sending out
    unsigned int _nextBufferIdx = 1; // awaiting data

    uint8_t _lastHalCmdStatus = 0xff;
};

#endif
