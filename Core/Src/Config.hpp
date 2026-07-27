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
        uint16_t universe;
        bool manualTargetIp;
        uint32_t targetIp;
    };

    struct Conf {
        uint32_t ip;
        uint8_t subnet;
        DmxOutConf dmxOuts[5];
        ArtnetOutConf artnetOut;
    };

    // void(uint32_t ip, uint8_t subnet)
    using NetworkCallback = std::function<void(uint32_t, uint8_t)>;
    // void(uint16_t universe, bool manualIp, uint32_t targetIp)
    using ArtnetOutCallback = std::function<void(uint16_t, bool, uint32_t)>;

public:
    Config(M95640R* eeprom);

    void setup(NetworkCallback const& networkCallback, ArtnetOutCallback const& artnetOutCallback);

    void setNetwork(uint32_t ip, uint8_t subnet);
    uint32_t ip() const { return _conf.ip; }
    uint8_t subnet() const { return _conf.subnet; }

    void setDmxOutInputDmx(uint8_t idx, bool inputDmx);
    void setDmxOutInputUniverse(uint8_t idx, uint16_t universe);
    bool dmxOutInputDmx(uint8_t outputIdx) const { return _conf.dmxOuts[outputIdx].inputDmx; }
    uint16_t dmxOutInputUniverse(uint8_t outputIdx) const { return _conf.dmxOuts[outputIdx].inputArtnetUniverse; }

    void setArtnetOutEnable(bool enable);
    void setArtnetOutUniverse(uint16_t universe);
    void setArtnetOutManualTargetIp(bool manualTargetIp);
    void setArtnetOutTargetIp(uint32_t targetIp);
    bool artnetOutEnable() const { return _conf.artnetOut.enable; }
    bool artnetOutManualTargetIp() const { return _conf.artnetOut.manualTargetIp; }
    uint32_t artnetOutTargetIp() const {
        if (_conf.artnetOut.manualTargetIp)
            return _conf.artnetOut.targetIp;
        uint32_t subnetMask = ~(uint64_t(1 << (32 - _conf.subnet)) - 1);
        return _conf.ip | ~subnetMask;
    }
    uint16_t artnetOutUniverse() const { return _conf.artnetOut.universe; }

    void applyConfSplitter();
    void applyConfArtnetToDmx();
    void applyConfResetAll();

private:
    void _loadConfig();
    void _writeConfig();

private:
    M95640R* _eeprom;
    volatile Conf _conf;
    NetworkCallback _networkCallback;
    ArtnetOutCallback _artnetOutCallback;
};

#endif
