#ifndef __DMXIN_HPP__
#define __DMXIN_HPP__

#include "Chrono.hpp"

#include <array>
#include <cstdint>

class Packet;

class DmxIn {
public:
    using PacketCallback = void(*)(Packet const&);

private:
    using Step = void (DmxIn::*)();

public:
    DmxIn();
    ~DmxIn();

    void init(PacketCallback cb);

    inline void tick() { (this->*_currentStep)(); }

private:
    void _waitForBreak();
    void _waitForBreakEnd();
    void _afterBreak();
    void _readData();

private:
    // 512 +18 to have same size as artnet dmx packet
    std::array<uint8_t, 512 + 18> _buffers[3];
    unsigned int _currentBufferIdx = 0;

    unsigned int _currentByteIdx;

    PacketCallback _callback = nullptr;

    Step _currentStep = &DmxIn::_waitForBreak;

    Chrono::UsTimer _timer;
};

#endif
