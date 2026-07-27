#ifndef __ARTNETOUT_HPP__
#define __ARTNETOUT_HPP__

#include "Packet.hpp"

struct udp_pcb;

class ArtnetOut {
public:
    using PacketSentCallback = void (*)(Packet const&, bool);

public:
    ArtnetOut();
    ~ArtnetOut();

    void init(PacketSentCallback cb);
    void setNetwork(udp_pcb* udp);
    void setUniverse(uint16_t universe);
    void tick();

    void sendDmx(Packet const& dmxPacket);

private:
    PacketSentCallback _cb = nullptr;
    udp_pcb* _udp = nullptr;
    uint16_t _universe = 0;

    Packet _buffers[2];
    unsigned int _currentBufferIdx = 0; // currently sending out
    unsigned int _nextBufferIdx = 1; // awaiting data
};

#endif
