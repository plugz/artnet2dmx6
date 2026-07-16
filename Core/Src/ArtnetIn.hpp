#ifndef __ARTNETIN_HPP__
#define __ARTNETIN_HPP__

#include <cstdint>
#include <memory>
#include <utility>

static constexpr uint16_t ARTNET_POLL =              0x2000;
static constexpr uint16_t ARTNET_POLLREPLY =         0x2100;
static constexpr uint16_t ARTNET_DIAGDATA =          0x2300;
static constexpr uint16_t ARTNET_COMMAND =           0x2400;
static constexpr uint16_t ARTNET_DMX =               0x5000;
static constexpr uint16_t ARTNET_NZS =               0x5100;
static constexpr uint16_t ARTNET_ADDRESS =           0x6000;
static constexpr uint16_t ARTNET_INPUT =             0x7000;
static constexpr uint16_t ARTNET_TODREQUEST =        0x8000;
static constexpr uint16_t ARTNET_TODDATA =           0x8100;
static constexpr uint16_t ARTNET_TODCONTROL =        0x8200;
static constexpr uint16_t ARTNET_RDM =               0x8300;
static constexpr uint16_t ARTNET_RDMSUB =            0x8400;
static constexpr uint16_t ARTNET_VIDEOSTEUP =        0xa010;
static constexpr uint16_t ARTNET_VIDEOPALETTE =      0xa020;
static constexpr uint16_t ARTNET_VIDEODATA =         0xa040;
static constexpr uint16_t ARTNET_MACMASTER =         0xf000;
static constexpr uint16_t ARTNET_MACSLAVE =          0xf100;
static constexpr uint16_t ARTNET_FIRMWAREMASTER =    0xf200;
static constexpr uint16_t ARTNET_FIRMWAREREPLY =     0xf300;
static constexpr uint16_t ARTNET_FILETNMASTER =      0xf400;
static constexpr uint16_t ARTNET_FILEFNMASTER =      0xf500;
static constexpr uint16_t ARTNET_FILEFNREPLY =       0xf600;
static constexpr uint16_t ARTNET_IPPROG =            0xf800;
static constexpr uint16_t ARTNET_IPREPLY =           0xf900;
static constexpr uint16_t ARTNET_MEDIA =             0x9000;
static constexpr uint16_t ARTNET_MEDIAPATCH =        0x9100;
static constexpr uint16_t ARTNET_MEDIACONTROL =      0x9200;
static constexpr uint16_t ARTNET_MEDIACONTROLREPLY = 0x9300;
static constexpr uint16_t ARTNET_TIMECODE =          0x9700;
static constexpr uint16_t ARTNET_TIMESYNC =          0x9800;
static constexpr uint16_t ARTNET_TRIGGER =           0x9900;
static constexpr uint16_t ARTNET_DIRECTORY =         0x9a00;
static constexpr uint16_t ARTNET_DIRECTORYREPLY =    0x9b00;

class ArtnetIn {
public:
    static constexpr uint16_t PORT = 0x1936;

public:
    class Packet {
    public:
        Packet(std::shared_ptr<uint8_t*> const& data, uint16_t dataSize);

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
