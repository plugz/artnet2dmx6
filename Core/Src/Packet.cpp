#include "Packet.hpp"

#include "ArtnetIn.hpp"

#include <cstring>

static constexpr char const ARTNET_CODE_STR[] = "Art-Net";

Packet::Packet(std::shared_ptr<uint8_t*> const& data, uint16_t dataSize, bool artnet) {

    _dataContainer = data;
    _data = *_dataContainer;
    _dataSize = dataSize;
    _valid = false;

    if (!artnet) {
        _valid = true;
        return;
    }

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

Packet::Packet() { _valid = false; }

Packet::~Packet() {}
