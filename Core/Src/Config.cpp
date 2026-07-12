#include "Config.hpp"

#include "M95640R.hpp"

Config::Config(M95640R* eeprom) : _eeprom(eeprom) {
}

void Config::setup() {
    _loadConfig();
}

uint32_t Config::ip() const {
    return _conf.ip;
}

uint8_t Config::netmask() const {
    return _conf.netmask;
}

uint16_t Config::universe(uint8_t idx) const {
    return _conf.universes[idx];
}

void Config::setNetwork(uint32_t ip, uint8_t netmask) {
    _conf.ip = ip;
    _conf.netmask = netmask;
    _writeConfig();
}

void Config::setUniverse(uint8_t idx, uint16_t universe) {
    _conf.universes[idx] = universe;
    _writeConfig();
}

void Config::_loadConfig() {
    _eeprom->EepromRead(0, sizeof(Conf), (uint8_t*)&_conf);
}

void Config::_writeConfig() {
    _eeprom->EepromWrite(0, sizeof(Conf), (uint8_t*)&_conf);
}
