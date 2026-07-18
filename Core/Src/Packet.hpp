#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <cstdint>
#include <memory>

class Packet {
public:
    Packet(std::shared_ptr<uint8_t*> const& data, uint16_t dataSize, bool artnet);
    Packet();
    ~Packet();

    operator bool() const { return valid(); }
    bool valid() const { return _valid; }
    uint8_t* data() const { return _data; }
    uint16_t dataSize() const { return _dataSize; }

    uint16_t code() const { return _code; }

    uint8_t* dmxData() const { return _data + 18; }
    uint16_t dmxDataSize() const { return _dataSize - 18; }
    uint16_t dmxUniverse() const { return _dmxUniverse; }

private:
    std::shared_ptr<uint8_t*> _dataContainer; // we keep this here so it is freed when not needed anymore
    uint8_t* _data;
    uint16_t _dataSize;
    uint16_t _code;
    uint16_t _dmxUniverse;
    bool _valid;
};

#endif
