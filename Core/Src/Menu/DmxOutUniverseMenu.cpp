#include "DmxOutUniverseMenu.hpp"

#include "Config.hpp"

#include <cstdio>

namespace Menu {

DmxOutUniverseMenu::DmxOutUniverseMenu(CommonStuff const& common)
    : GenericMenu<uint16_t>(common, {})
{
}

DmxOutUniverseMenu::~DmxOutUniverseMenu() {
}

void DmxOutUniverseMenu::initDmxOutUniverseMenu(uint8_t idx) {
    _idx = idx;
    _config.min = 0;
    _config.max = 0x7fff;
    snprintf(_config.name, sizeof(_config.name), "In Universe");
    snprintf(_config.inName, sizeof(_config.desc), "Artnet In Universe");
    snprintf(_config.desc, sizeof(_config.inName), "for DMX Out %i", _idx + 1);
    _config.getValueCallback = [this]() -> uint16_t { return _common.config->dmxOutInputUniverse(_idx); };
    _config.setValueCallback = [this](uint16_t val) -> void { _common.config->setDmxOutInputUniverse(_idx, val); };
}

} // namespace Menu
