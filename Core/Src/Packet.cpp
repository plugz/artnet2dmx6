#include "Packet.hpp"

#include "Artnet.hpp"
#include "ArtnetIn.hpp"

#include <cstring>

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
    case ARTNET_CMD_DMX: {
        if (_dataSize < 20)
            return; // invalid
        _dmxUniverse = (uint16_t(_data[15]) << 8) | uint16_t(_data[14]); // little endian in artnet packet
    } break;
    }
    _valid = true;
}

Packet::Packet() {
    _valid = false;
    _data = nullptr;
}

Packet::~Packet() {}

void Packet::fillForArtnetOut(uint16_t universe) {
    static uint16_t sequence = 1;

    _dmxUniverse = universe;
    _code = ARTNET_CMD_DMX;
    _valid = true;

    std::copy(ARTNET_CODE_STR, ARTNET_CODE_STR + sizeof(ARTNET_CODE_STR), _data);

    // OPCODE
    _data[8] = ARTNET_CMD_DMX;
    _data[9] = ARTNET_CMD_DMX >> 8;

    // version 14
    _data[10] = 0;
    _data[11] = 0x0e;

    // sequence
    _data[12] = sequence++;
    sequence = sequence ? 1 : sequence;

    // physical
    _data[13] = 0;

    // universe
    _data[14] = universe;
    _data[15] = universe >> 8;

    // size must be even and in the range 2-512
    uint16_t size = _dataSize - 18;
    size += size % 2;
    size = size ? size : 2;
    _data[16] = size;
    _data[17] = size >> 8;
}
