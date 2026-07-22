#include "UniverseMenu.hpp"

#include "Config.hpp"

#include <cstdio>

namespace Menu {

UniverseMenu::UniverseMenu(CommonStuff const& common)
    : GenericMenu<uint16_t>(common, {})
{
}

UniverseMenu::~UniverseMenu() {
}

void UniverseMenu::initUniverseMenu(uint8_t idx) {
    _idx = idx;
    _config.min = 0;
    _config.max = 0x7fff;
    snprintf(_config.name, sizeof(_config.name), "Input Universe");
    snprintf(_config.inName, sizeof(_config.desc), "Artnet Universe");
    snprintf(_config.desc, sizeof(_config.inName), "for DMX Out %i", _idx + 1);
    _config.getValueCallback = [this]() -> uint16_t { return _common.config->dmxOutInputUniverse(_idx); };
    _config.setValueCallback = [this](uint16_t val) -> void { _common.config->setDmxOutInputUniverse(_idx, val); };
}

} // namespace Menu
