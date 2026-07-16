#include "ArtnetIn.hpp"

#include <cstring>

static constexpr char const ARTNET_CODE_STR[] = "Art-Net";

ArtnetIn::Packet::Packet(std::shared_ptr<uint8_t*> const& data, uint16_t dataSize) {
    _dataContainer = data;
    _data = *_dataContainer;
    _dataSize = dataSize;
    _valid = false;

    if (dataSize < 12)
        return;

    if (std::memcmp(ARTNET_CODE_STR, _data, sizeof(ARTNET_CODE_STR)))
        return;

    // little endian in artnet packet
    _code = (uint16_t(_data[9]) << 8) | uint16_t(_data[8]);

    switch (_code) {
    case ARTNET_DMX: {
        if (_dataSize < 20)
            return; // invalid
        _dmxUniverse = (uint16_t(_data[15]) << 8) | uint16_t(_data[14]); // little endian in artnet packet
    } break;
    }
    _valid = true;
}

ArtnetIn::Packet::Packet() { _valid = false; }

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
    Packet newPacket{data, dataSize};
    if (!newPacket)
        return;

    for (unsigned int i = 0; i < _callbackCount; ++i) {
        if (_callbacks[i].first == newPacket.code()) {
            _callbacks[i].second(newPacket);
            break;
        }
    }
}
