#include "Config.hpp"

#include "M95640R.hpp"

Config::Config(M95640R* eeprom) : _eeprom(eeprom) {
}

void Config::setup(NetworkCallback const& networkCallback, ArtnetOutCallback const& artnetOutCallback) {
    _networkCallback = networkCallback;
    _artnetOutCallback = artnetOutCallback;
    _loadConfig();
}


void Config::setNetwork(uint32_t ip, uint8_t subnet) {
    _conf.ip = ip;
    _conf.subnet = subnet;
    _writeConfig();

    if (_networkCallback) {
        _networkCallback(ip, subnet);
    }
}

void Config::setDmxOutInputDmx(uint8_t idx, bool inputDmx) {
    _conf.dmxOuts[idx].inputDmx = inputDmx;
    _writeConfig();
}

void Config::setDmxOutInputUniverse(uint8_t idx, uint16_t universe) {
    _conf.dmxOuts[idx].inputArtnetUniverse = universe;
    _writeConfig();
}

void Config::setArtnetOutEnable(bool enable) {
    _conf.artnetOut.enable = enable;
    _writeConfig();
}

void Config::setArtnetOutUniverse(uint16_t universe) {
    _conf.artnetOut.universe = universe;
    _writeConfig();

    if (_artnetOutCallback) {
        _artnetOutCallback(_conf.artnetOut.universe, _conf.artnetOut.manualTargetIp, _conf.artnetOut.targetIp);
    }
}

void Config::setArtnetOutManualTargetIp(bool manualTargetIp) {
    _conf.artnetOut.manualTargetIp = manualTargetIp;
    _writeConfig();

    if (_artnetOutCallback) {
        _artnetOutCallback(_conf.artnetOut.universe, _conf.artnetOut.manualTargetIp, _conf.artnetOut.targetIp);
    }
}

void Config::setArtnetOutTargetIp(uint32_t targetIp) {
    _conf.artnetOut.targetIp = targetIp;
    _writeConfig();

    if (_artnetOutCallback) {
        _artnetOutCallback(_conf.artnetOut.universe, _conf.artnetOut.manualTargetIp, _conf.artnetOut.targetIp);
    }
}

void Config::applyConfSplitter(){
    for (unsigned int i = 0 ; i < 5 ; ++i) {
        setDmxOutInputDmx(i, true);
    }
    setArtnetOutEnable(false);
}

void Config::applyConfArtnetToDmx(){
    for (unsigned int i = 0 ; i < 5 ; ++i) {
        setDmxOutInputDmx(i, false);
        setDmxOutInputUniverse(i, i);
    }
    setArtnetOutEnable(false);
}

void Config::applyConfResetAll(){
    _conf.ip = 0x0200001f;
    _conf.subnet = 8;

    for (unsigned int i = 0; i < 5; ++i) {
        _conf.dmxOuts[i].inputDmx = false;
        _conf.dmxOuts[i].inputArtnetUniverse = i;
    }

    _conf.artnetOut.enable = true;
    _conf.artnetOut.universe = 0;
    _conf.artnetOut.manualTargetIp = false;
    _conf.artnetOut.targetIp = 0x02000001;

    _writeConfig();

    if (_networkCallback)
        _networkCallback(_conf.ip, _conf.subnet);
    if (_artnetOutCallback)
        _artnetOutCallback(_conf.artnetOut.universe, _conf.artnetOut.manualTargetIp, _conf.artnetOut.targetIp);
}

void Config::_loadConfig() {
    _eeprom->EepromRead(0, sizeof(Conf), (uint8_t*)&_conf);

    // sanitation / first init
    // set bools to 0 or 1
    // set ip to a default 2.0.0.31/8

    if ((_conf.ip == 0xffffffff) || (_conf.ip == 0))
        _conf.ip = 0x0200001f;

    if ((_conf.subnet != 24) && (_conf.subnet != 16) && (_conf.subnet != 8))
        _conf.subnet = 8;

    unsigned int idx = 0;
    for (auto& outConf: _conf.dmxOuts) {
        if ((uint8_t)outConf.inputDmx == 0xff)
            outConf.inputDmx = false;
        outConf.inputDmx = (uint8_t)outConf.inputDmx ? true : false;
        if (outConf.inputArtnetUniverse == 0xffff)
            outConf.inputArtnetUniverse = idx;
        ++idx;
    }

    _conf.artnetOut.enable = (uint8_t)_conf.artnetOut.enable ? true : false;
    if ((uint8_t)_conf.artnetOut.manualTargetIp == 0xff)
        _conf.artnetOut.manualTargetIp = false;
    _conf.artnetOut.manualTargetIp = (uint8_t)_conf.artnetOut.manualTargetIp ? true : false;
    if ((_conf.artnetOut.targetIp == 0xffffffff) || (_conf.artnetOut.targetIp == 0))
        _conf.artnetOut.targetIp = 0x02000001;
    if (_conf.artnetOut.universe == 0xffff)
        _conf.artnetOut.universe = 0;
}

void Config::_writeConfig() {
    _eeprom->EepromWrite(0, sizeof(Conf), (uint8_t*)&_conf);
}
