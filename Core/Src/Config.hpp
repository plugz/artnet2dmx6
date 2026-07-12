#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <cstdint>
#include <functional>

class M95640R;

class Config {
public:
    struct Conf {
        uint32_t ip;
        uint8_t subnet;
        uint16_t universes[6];
    };

    // void(uint32_t ip, uint8_t subnet)
    using NetworkCallback = std::function<void(uint32_t, uint8_t)>;
    // void(uint8_t idx, uint16_t universe)
    using UniverseCallback = std::function<void(uint8_t, uint16_t)>;

public:
    Config(M95640R* eeprom);

    void setup(NetworkCallback const& networkCallback, UniverseCallback const& universeCallback);

    uint32_t ip() const;
    uint8_t subnet() const;
    uint16_t universe(uint8_t idx) const;

    void setNetwork(uint32_t ip, uint8_t subnet);
    void setUniverse(uint8_t idx, uint16_t universe);

private:
    void _loadConfig();
    void _writeConfig();

private:
    M95640R* _eeprom;
    Conf _conf;
    NetworkCallback _networkCallback;
    UniverseCallback _universeCallback;
};

#endif
