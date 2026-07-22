#include "Config.hpp"

#include "M95640R.hpp"

Config::Config(M95640R* eeprom) : _eeprom(eeprom) {
}

void Config::setup(NetworkCallback const& networkCallback, DmxOutCallback const& dmxOutCallback, ArtnetOutCallback const& artnetOutCallback) {
    _networkCallback = networkCallback;
    _dmxOutCallback = dmxOutCallback;
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

    if (_dmxOutCallback) {
        _dmxOutCallback(idx, universe);
    }
}

void Config::setArtnetOutEnable(bool enable) {
    _conf.artnetOut.enable = enable;
    _writeConfig();
}

void Config::setArtnetOutTargetIp(bool manual, uint32_t targetIp) {
    _conf.artnetOut.manualTargetIp = manual;
    _conf.artnetOut.targetIp = targetIp;
    _writeConfig();

    if (_artnetOutCallback) {
        _artnetOutCallback(manual, targetIp);
    }
}

void Config::_loadConfig() {
    _eeprom->EepromRead(0, sizeof(Conf), (uint8_t*)&_conf);

    // set bools to 0 or 1
    for (auto& outConf: _conf.dmxOuts) {
        outConf.inputDmx = (uint8_t)outConf.inputDmx ? true : false;
    }

    _conf.artnetOut.enable = (uint8_t)_conf.artnetOut.enable ? true : false;
    _conf.artnetOut.manualTargetIp = (uint8_t)_conf.artnetOut.manualTargetIp ? true : false;
}

void Config::_writeConfig() {
    __disable_irq();
    _eeprom->EepromWrite(0, sizeof(Conf), (uint8_t*)&_conf);
    __enable_irq();
}
