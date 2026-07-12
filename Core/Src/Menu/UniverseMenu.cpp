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
    if (idx < 5) {
        snprintf(_config.name, sizeof(_config.name), "DMX OUT %i", (uint8_t)idx);
        snprintf(_config.inName, sizeof(_config.desc), "Artnet Universe");
        snprintf(_config.desc, sizeof(_config.inName), "for DMX OUT %i", (uint8_t)idx);
    }
    else {
        snprintf(_config.name, sizeof(_config.name), "DMX IN");
        snprintf(_config.inName, sizeof(_config.desc), "Artnet Universe");
        snprintf(_config.desc, sizeof(_config.inName), "for DMX IN");
    }
    _config.getValueCallback = [this]() -> uint16_t { return _common.config->universe(_idx); };
    _config.setValueCallback = [this](uint16_t val) -> void { _common.config->setUniverse(_idx, val); };
}

} // namespace Menu
