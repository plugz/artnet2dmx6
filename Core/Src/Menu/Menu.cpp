#include "Menu.hpp"

namespace Menu {

// Menu

Menu* Menu::_current = nullptr;

Menu::~Menu() {}

void Menu::enable() {
    _current = this;
    _enable();
}

Menu* Menu::current() { return _current; }

void Menu::left(Button::Event e) {
    if (e != Button::Event::PRESS)
        return;
    if (_parent) {
        _disable();
        _parent->enable();
    }
}

void Menu::tick() {}

void Menu::applyConfig(bool changeMenu) {
    _applyConfig();
//    _common.config->save(); // TODO ? already done in Config::set*
    if (changeMenu)
        left(Button::Event::PRESS);
}

void Menu::_applyConfig() {}

void Menu::_enable() { _display(); }

void Menu::_disable() {}

} // namespace Menu
