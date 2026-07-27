#include "ArtnetOut.hpp"

#include "udp.h"

ArtnetOut::ArtnetOut() {}

ArtnetOut::~ArtnetOut() {}

void ArtnetOut::init(PacketSentCallback cb) {
    _cb = cb;
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

    pbuf* p = (pbuf*)(((uint8_t*)(packet.dataContainer())) - offsetof(pbuf, payload));

    // This always returns ERR_OK for some reason
    bool success = (udp_send(_udp, p) == ERR_OK);

    if (_cb)
        _cb(packet, success);

    packet = {};
}

void ArtnetOut::sendDmx(Packet const& dmxPacket) {
    _buffers[_nextBufferIdx] = dmxPacket;
}
