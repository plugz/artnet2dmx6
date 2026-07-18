#include "ScreenOffMenu.hpp"

#include "LiquidCrystalI2C.hpp"

namespace Menu {

ScreenOffMenu::ScreenOffMenu(CommonStuff const& common) : Menu(common) {}
ScreenOffMenu::~ScreenOffMenu() {}

char const* ScreenOffMenu::name() {
    return "Screen OFF";
}

void ScreenOffMenu::up(Button::Event e) {
    left(e);
}

void ScreenOffMenu::down(Button::Event e) {
    left(e);
}

void ScreenOffMenu::right(Button::Event e) {
    left(e);
}

void ScreenOffMenu::_enable() {
    _common.screen->setBacklight(false);
}

void ScreenOffMenu::_disable() {
    _common.screen->setBacklight(true);
}

void ScreenOffMenu::_display() {
}

} // namespace Menu
