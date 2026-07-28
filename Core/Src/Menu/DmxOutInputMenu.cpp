#include "DmxOutInputMenu.hpp"

#include "Config.hpp"
#include "LiquidCrystalI2C.hpp"

#include <cstdio>

namespace Menu {

enum {
    LINE_BUFF_SIZE = LiquidCrystalI2C::COLS + 1,
};

DmxOutInputMenu::DmxOutInputMenu(CommonStuff const& common) : QuickBoolMenu(common)
{}

DmxOutInputMenu::~DmxOutInputMenu() {}

char const* DmxOutInputMenu::name() {
    std::snprintf(_name, sizeof(_name), "In Type \xa5%s\xa5", _config.getValueCallback() ? "DMX In" : "Artnet");
    return _name;
}

void DmxOutInputMenu::initDmxOutInputMenu(uint8_t idx) {
    _idx = idx;
    _config.min = false;
    _config.max = true;

    std::snprintf(_config.name, sizeof(_config.name), "Input type");
    std::snprintf(_config.inName, sizeof(_config.desc), "Input type");
    std::snprintf(_config.desc, sizeof(_config.inName), "for DMX Out %i", (uint8_t)(_idx + 1));

    _config.getValueCallback = [this]() -> bool { return _common.config->dmxOutInputDmx(_idx); };
    _config.setValueCallback = [this](bool val) -> void { _common.config->setDmxOutInputDmx(_idx, val); };
}

} // namespace Menu
