#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <cstdint>
#include <functional>

class M95640R;

class Config {
public:
    struct DmxOutConf {
        bool inputDmx;
        uint16_t inputArtnetUniverse;
    };

    struct ArtnetOutConf {
        bool enable;
        bool manualTargetIp;
        uint32_t targetIp;
        uint16_t universe;
    };

    struct Conf {
        uint32_t ip;
        uint8_t subnet;
        DmxOutConf dmxOuts[5];
        ArtnetOutConf artnetOut;
    };

    // void(uint32_t ip, uint8_t subnet)
    using NetworkCallback = std::function<void(uint32_t, uint8_t)>;
    // void(uint8_t idx, uint16_t universe)
    using DmxOutCallback = std::function<void(uint8_t, uint16_t)>;
    // void(uint8_t idx, uint16_t universe)
    using ArtnetOutCallback = std::function<void(bool, uint32_t)>;

public:
    Config(M95640R* eeprom);

    void setup(NetworkCallback const& networkCallback, DmxOutCallback const& dmxOutCallback, ArtnetOutCallback const& artnetOutCallback);

    void setNetwork(uint32_t ip, uint8_t subnet);
    uint32_t ip() const { return _conf.ip; }
    uint8_t subnet() const { return _conf.subnet; }

    void setDmxOutInputDmx(uint8_t idx, bool inputDmx);
    void setDmxOutInputUniverse(uint8_t idx, uint16_t universe);
    bool dmxOutInputDmx(uint8_t outputIdx) const { return _conf.dmxOuts[outputIdx].inputDmx; }
    uint16_t dmxOutInputUniverse(uint8_t outputIdx) const { return _conf.dmxOuts[outputIdx].inputArtnetUniverse; }

    void setArtnetOutEnable(bool enable);
    void setArtnetOutTargetIp(bool manual, uint32_t targetIp = 0);
    bool artnetOutEnable() const { return _conf.artnetOut.enable; }
    bool artnetOutUnicast() const { return _conf.artnetOut.manualTargetIp; }
    uint32_t artnetOutTargetIp() const { return _conf.artnetOut.targetIp; }

private:
    void _loadConfig();
    void _writeConfig();

private:
    M95640R* _eeprom;
    volatile Conf _conf;
    NetworkCallback _networkCallback;
    DmxOutCallback _dmxOutCallback;
    ArtnetOutCallback _artnetOutCallback;
};

#endif
