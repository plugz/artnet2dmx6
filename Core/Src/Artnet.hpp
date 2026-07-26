#ifndef __ARTNET_HPP__
#define __ARTNET_HPP__

#include <cstdint>

static constexpr uint16_t ARTNET_CMD_POLL =              0x2000;
static constexpr uint16_t ARTNET_CMD_POLLREPLY =         0x2100;
static constexpr uint16_t ARTNET_CMD_DIAGDATA =          0x2300;
static constexpr uint16_t ARTNET_CMD_COMMAND =           0x2400;
static constexpr uint16_t ARTNET_CMD_DMX =               0x5000;
static constexpr uint16_t ARTNET_CMD_NZS =               0x5100;
static constexpr uint16_t ARTNET_CMD_ADDRESS =           0x6000;
static constexpr uint16_t ARTNET_CMD_INPUT =             0x7000;
static constexpr uint16_t ARTNET_CMD_TODREQUEST =        0x8000;
static constexpr uint16_t ARTNET_CMD_TODDATA =           0x8100;
static constexpr uint16_t ARTNET_CMD_TODCONTROL =        0x8200;
static constexpr uint16_t ARTNET_CMD_RDM =               0x8300;
static constexpr uint16_t ARTNET_CMD_RDMSUB =            0x8400;
static constexpr uint16_t ARTNET_CMD_VIDEOSTEUP =        0xa010;
static constexpr uint16_t ARTNET_CMD_VIDEOPALETTE =      0xa020;
static constexpr uint16_t ARTNET_CMD_VIDEODATA =         0xa040;
static constexpr uint16_t ARTNET_CMD_MACMASTER =         0xf000;
static constexpr uint16_t ARTNET_CMD_MACSLAVE =          0xf100;
static constexpr uint16_t ARTNET_CMD_FIRMWAREMASTER =    0xf200;
static constexpr uint16_t ARTNET_CMD_FIRMWAREREPLY =     0xf300;
static constexpr uint16_t ARTNET_CMD_FILETNMASTER =      0xf400;
static constexpr uint16_t ARTNET_CMD_FILEFNMASTER =      0xf500;
static constexpr uint16_t ARTNET_CMD_FILEFNREPLY =       0xf600;
static constexpr uint16_t ARTNET_CMD_IPPROG =            0xf800;
static constexpr uint16_t ARTNET_CMD_IPREPLY =           0xf900;
static constexpr uint16_t ARTNET_CMD_MEDIA =             0x9000;
static constexpr uint16_t ARTNET_CMD_MEDIAPATCH =        0x9100;
static constexpr uint16_t ARTNET_CMD_MEDIACONTROL =      0x9200;
static constexpr uint16_t ARTNET_CMD_MEDIACONTROLREPLY = 0x9300;
static constexpr uint16_t ARTNET_CMD_TIMECODE =          0x9700;
static constexpr uint16_t ARTNET_CMD_TIMESYNC =          0x9800;
static constexpr uint16_t ARTNET_CMD_TRIGGER =           0x9900;
static constexpr uint16_t ARTNET_CMD_DIRECTORY =         0x9a00;
static constexpr uint16_t ARTNET_CMD_DIRECTORYREPLY =    0x9b00;

static constexpr char const ARTNET_CODE_STR[] = "Art-Net";

static constexpr uint16_t ARTNET_DEFAULT_PORT = 0x1936;

#endif
