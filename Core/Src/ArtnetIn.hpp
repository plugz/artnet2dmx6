#ifndef __ARTNETIN_HPP__
#define __ARTNETIN_HPP__

#include <cstdint>
#include <memory>
#include <utility>

class Packet;

class ArtnetIn {
public:
    using PacketCallback = void (*)(Packet const&);

public:
    ArtnetIn();

    void init();

    void setPacketCallback(uint16_t type, PacketCallback cb);

    void handlePacket(std::shared_ptr<uint8_t*> const& data, uint16_t dataSize);

private:
    std::pair<uint16_t, PacketCallback> _callbacks[16];
    unsigned int _callbackCount = 0;
};

#endif
