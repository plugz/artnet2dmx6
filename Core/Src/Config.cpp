#include "Config.hpp"

#include "M95640R.hpp"

Config::Config(M95640R* eeprom) : _eeprom(eeprom) {
}

void Config::setup(NetworkCallback const& networkCallback, UniverseCallback const& universeCallback) {
    _networkCallback = networkCallback;
    _universeCallback = universeCallback;
    _loadConfig();
}

uint32_t Config::ip() const {
    return _conf.ip;
}

uint8_t Config::subnet() const {
    return _conf.subnet;
}

uint16_t Config::universe(uint8_t idx) const {
    return _conf.universes[idx];
}

void Config::setNetwork(uint32_t ip, uint8_t subnet) {
    _conf.ip = ip;
    _conf.subnet = subnet;
    _writeConfig();

    if (_networkCallback) {
        _networkCallback(ip, subnet);
    }
}

void Config::setUniverse(uint8_t idx, uint16_t universe) {
    _conf.universes[idx] = universe;
    _writeConfig();

    if (_universeCallback) {
        _universeCallback(idx, universe);
    }
}

void Config::_loadConfig() {
    _eeprom->EepromRead(0, sizeof(Conf), (uint8_t*)&_conf);
}

void Config::_writeConfig() {
    _eeprom->EepromWrite(0, sizeof(Conf), (uint8_t*)&_conf);
}
