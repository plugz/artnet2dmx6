#include "ArtnetIn.hpp"

#include "Packet.hpp"

ArtnetIn::ArtnetIn() {}

void ArtnetIn::init() {}

void ArtnetIn::setPacketCallback(uint16_t type, PacketCallback cb) {
    unsigned int i;
    for (i = 0; i < _callbackCount && i < std::size(_callbacks); ++i) {
        if (_callbacks[i].first == type)
            break;
    }

    // callbacks already full
    if (i == std::size(_callbacks))
        return;

    _callbacks[i] = {type, cb};
    if (i == _callbackCount)
        ++_callbackCount;
}

void ArtnetIn::handlePacket(std::shared_ptr<uint8_t*> const& data, uint16_t dataSize) {
    Packet newPacket{data, dataSize, true};
    if (!newPacket)
        return;

    for (unsigned int i = 0; i < _callbackCount; ++i) {
        if (_callbacks[i].first == newPacket.code()) {
            _callbacks[i].second(newPacket);
            break;
        }
    }
}
