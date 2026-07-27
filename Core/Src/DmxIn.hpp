#ifndef __DMXIN_HPP__
#define __DMXIN_HPP__

#include "Packet.hpp"

#include <cstdint>

class DmxIn {
public:
    // DmxIn calls this to get a new packet buffer
    using NewPacketCallback = void (*)(Packet*, Packet*);
    // DmxIn calls this to send a buffer that has been filled
    using PacketReadyCallback = void(*)(Packet const&, Packet const&);

public:
    DmxIn();
    ~DmxIn();

    void init(NewPacketCallback npcb, PacketReadyCallback prcb);
    void tick();

    // called from interrupt : no heavy processing here
    inline void handleBreak() {
        if (_currentPackets[0].dataSize() == 17)
            return;

        _swapAndSend();
    }

    // called from interrupt : no heavy processing here
    inline void handleByte(uint8_t byte) {
        for (auto& currentPacket: _currentPackets)
            currentPacket.pushByte(byte);
        if (_currentPackets[0].dataSize() < 513 + 17)
            return;

        _swapAndSend();
    }

private:
    inline void _swapAndSend() {
        for (unsigned int i = 0; i < std::size(_currentPackets); ++i) {
            _readyToSendPackets[i] = _currentPackets[i];
            _currentPackets[i] = _nextPackets[i];
            _nextPackets[i] = {};
        }
    }

private:
    Packet _currentPackets[2];
    Packet _readyToSendPackets[2];

    // have an empty packet ready so the new packet creation is done outside of an interrupt
    Packet _nextPackets[2];

    NewPacketCallback _npcb = nullptr;
    PacketReadyCallback _prcb = nullptr;
};

#endif
