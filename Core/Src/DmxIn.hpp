#ifndef __DMXIN_HPP__
#define __DMXIN_HPP__

#include "Packet.hpp"

#include <cstdint>

class DmxIn {
public:
    // DmxIn calls this to get a new packet buffer
    using NewPacketCallback = Packet (*)();
    // DmxIn calls this to send a buffer that has been filled
    using PacketReadyCallback = void(*)(Packet const&);

public:
    DmxIn();
    ~DmxIn();

    void init(NewPacketCallback npcb, PacketReadyCallback prcb);
    void tick();

    // called from interrupt : no heavy processing here
    inline void handleBreak() {
        if (_currentPacket.dataSize() == 17)
            return;

        _swapAndSend();
    }

    // called from interrupt : no heavy processing here
    inline void handleByte(uint8_t byte) {
        _currentPacket.pushByte(byte);
        if (_currentPacket.dataSize() < 513 + 17)
            return;

        _swapAndSend();
    }

private:
    inline void _swapAndSend() {
        _readyToSendPacket = _currentPacket;
        _currentPacket = _nextPacket;
        _nextPacket = {};
    }

private:
    Packet _currentPacket;
    Packet _readyToSendPacket;

    // have an empty packet ready so the new packet creation is done outside of an interrupt
    Packet _nextPacket;

    NewPacketCallback _npcb = nullptr;
    PacketReadyCallback _prcb = nullptr;
};

#endif
