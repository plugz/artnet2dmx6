#include "ArtnetOut.hpp"

#include "Artnet.hpp"

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

void ArtnetOut::setTargetIp(uint32_t targetIp) {
    _targetIp = targetIp;
}

void ArtnetOut::tick() {
    if (!(_buffers[_nextBufferIdx]))
        return;

    _currentBufferIdx = (_currentBufferIdx + 1) % std::size(_buffers);
    _nextBufferIdx = (_nextBufferIdx + 1) % std::size(_buffers);

    auto& packet = _buffers[_currentBufferIdx];

    packet.fillForArtnetOut(_universe);

    pbuf* p = (pbuf*)(((uint8_t*)(packet.dataContainer())) - offsetof(pbuf, payload));

    // reduce pbuf size
    pbuf_realloc(p, packet.dataSize());

    ip4_addr_t targetAddress;
    IP4_ADDR(&targetAddress, uint8_t(_targetIp >> 24), uint8_t(_targetIp >> 16), uint8_t(_targetIp >> 8), uint8_t(_targetIp));
    // This always returns ERR_OK for some reason
    bool success = (udp_sendto(_udp, p, &targetAddress, ARTNET_DEFAULT_PORT) == ERR_OK);

    if (_cb)
        _cb(packet, success);

    packet = {};
}

void ArtnetOut::sendDmx(Packet const& dmxPacket) {
    _buffers[_nextBufferIdx] = dmxPacket;
}
