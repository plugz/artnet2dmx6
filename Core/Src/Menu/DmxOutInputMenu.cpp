#include "DmxOutInputMenu.hpp"

#include "Config.hpp"
#include "LiquidCrystalI2C.hpp"

#include <cstdio>

namespace Menu {

enum {
    LINE_BUFF_SIZE = LiquidCrystalI2C::COLS + 1,
};

DmxOutInputMenu::DmxOutInputMenu(CommonStuff const& common) : GenericMenu<bool>(common, {})
{}

DmxOutInputMenu::~DmxOutInputMenu() {}

void DmxOutInputMenu::initDmxOutInputMenu(uint8_t idx) {
    _idx = idx;
    _config.min = false;
    _config.max = true;

    snprintf(_config.name, sizeof(_config.name), "Input type");
    snprintf(_config.inName, sizeof(_config.desc), "Input type");
    snprintf(_config.desc, sizeof(_config.inName), "for DMX Out %i", (uint8_t)(_idx + 1));

    _config.getValueCallback = [this]() -> bool { return _common.config->dmxOutInputDmx(_idx); };
    _config.setValueCallback = [this](bool val) -> void { _common.config->setDmxOutInputDmx(_idx, val); };
}

void DmxOutInputMenu::_displayValue() {
    char buff[LINE_BUFF_SIZE];
    snprintf(buff, sizeof(buff), "  %s", (_value ? "DMX In" : "Artnet"));
    _common.screen->printLine(2, buff);
}

} // namespace Menu
