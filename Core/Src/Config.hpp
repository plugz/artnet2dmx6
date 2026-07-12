#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <cstdint>

class M95640R;

class Config {
public:
    struct Conf {
        uint32_t ip;
        uint8_t netmask;
        uint16_t universes[6];
    };

public:
    Config(M95640R* eeprom);

    void setup();

    uint32_t ip() const;
    uint8_t netmask() const;
    uint16_t universe(uint8_t idx) const;

    void setNetwork(uint32_t ip, uint8_t netmask);
    void setUniverse(uint8_t idx, uint16_t universe);

private:
    void _loadConfig();
    void _writeConfig();

private:
    M95640R* _eeprom;
    Conf _conf;
};

#endif
