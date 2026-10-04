#include "AboutMenu.hpp"

#include "Config.hpp"
#include "LiquidCrystalI2C.hpp"

namespace Menu {

enum {
    LINE_BUFF_SIZE = LiquidCrystalI2C::COLS + 1,
};

AboutMenu::AboutMenu(CommonStuff const& common)
    : Menu(common) {}

AboutMenu::~AboutMenu() {}

char const* AboutMenu::name() { return "About"; }

void AboutMenu::up(Button::Event e) {
    left(e);
}

void AboutMenu::down(Button::Event e) {
    left(e);
}

void AboutMenu::right(Button::Event e) {
    left(e);
}

void AboutMenu::_display() {
    _common.screen->printLine(0, "About");
    _common.screen->printLine(1, "");
    _common.screen->printLine(2, " Device ID : %" PRIu32, (uint32_t)Config::DEVICE_ID);
    _common.screen->printLine(3, " Software V.%s", Config::SOFTWARE_VERSION);
}

} // namespace Menu
