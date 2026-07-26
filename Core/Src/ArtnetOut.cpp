#include "ArtnetOut.hpp"

#include "udp.h"

ArtnetOut::ArtnetOut() {}

ArtnetOut::~ArtnetOut() {}

void ArtnetOut::init(PacketSentCallback cb) {
    _cb = cb;

    _pbuf = pbuf_alloc(PBUF_TRANSPORT, 18 + 512, PBUF_RAM);
}

void ArtnetOut::setNetwork(udp_pcb* udp) {
    _udp = udp;
}

void ArtnetOut::setUniverse(uint16_t universe) {
    _universe = universe;
}

void ArtnetOut::tick() {
    if (!(_buffers[_nextBufferIdx]))
        return;

    _currentBufferIdx = (_currentBufferIdx + 1) % std::size(_buffers);
    _nextBufferIdx = (_nextBufferIdx + 1) % std::size(_buffers);

    auto& packet = _buffers[_currentBufferIdx];

    packet.fillForArtnetOut(_universe);

    std::copy(packet.data(), packet.data() + packet.dataSize(), (uint8_t*)(_pbuf->payload));

    udp_send(_udp, _pbuf);

    if (_cb)
        _cb(packet);

    packet = {};
}

void ArtnetOut::sendDmx(Packet const& dmxPacket) {
    _buffers[_nextBufferIdx] = dmxPacket;
}
