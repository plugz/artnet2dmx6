#include "UniverseMenu.hpp"

#include <cstdio>

namespace Menu {

UniverseMenu::UniverseMenu(CommonStuff const& common)
    : GenericMenu<uint32_t>(common, {})
{
}

UniverseMenu::~UniverseMenu() {
}

void UniverseMenu::initUniverseMenu(uint8_t idx) {
    _config.min = 0;
    _config.max = 100; // TODO artnet spec
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
    _config.getValueCallback = [this]() -> uint32_t { return 1; /* TODO return config blabla */ };
    _config.setValueCallback = [this](uint32_t val) -> void {/* TODO */ };
}

} // namespace Menu
