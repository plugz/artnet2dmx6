#include "Menu.hpp"

#include "LiquidCrystalI2C.hpp"

namespace Menu {

enum {
    LINE_BUFF_SIZE = LiquidCrystalI2C::MAX_COLS + 1,
};

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
//    _common.config->save(); // TODO
    if (changeMenu)
        left(Button::Event::PRESS);
}

void Menu::_applyConfig() {}

void Menu::_enable() { _display(); }

void Menu::_disable() {}

// ContainerMenu

template<typename... TSubmenus>
void ContainerMenu<TSubmenus...>::up(Button::Event e) {
    if (e != Button::Event::PRESS)
        return;
    if (_currentMenuIdx == 0)
        return;
    _currentMenuIdx = (_currentMenuIdx + (_subMenus.size() - 1)) % _subMenus.size();
    _display();
}

template<typename... TSubmenus>
void ContainerMenu<TSubmenus...>::down(Button::Event e) {
    if (e != Button::Event::PRESS)
        return;
    if (_currentMenuIdx == _subMenus.size() - 1)
        return;
    _currentMenuIdx = (_currentMenuIdx + 1) % _subMenus.size();
    _display();
}

template<typename... TSubmenus>
void ContainerMenu<TSubmenus...>::right(Button::Event e) {
    if (e != Button::Event::PRESS)
        return;
    _disable();
    _subMenus[_currentMenuIdx]->enable();
}

template<typename... TSubmenus>
void ContainerMenu<TSubmenus...>::_display() {
    _common.screen->printLine(0, name());
    char line[LINE_BUFF_SIZE];
    {
        if (_currentMenuIdx != 0) {
            auto idx = (_currentMenuIdx + (_subMenus.size() - 1)) % _subMenus.size();
            snprintf("  %s", sizeof(line), _subMenus[idx]->name());
        }
        else {
            line[0] = '\0';
        }
        _common.screen->printLine(1, line);
    }
    {
        auto idx = _currentMenuIdx;
        snprintf("> %s", sizeof(line), _subMenus[idx]->name());
        _common.screen->printLine(2, line);
    }
    {
        if (_currentMenuIdx != _subMenus.size() - 1) {
            auto idx = (_currentMenuIdx + 1) % _subMenus.size();
            snprintf("  %s", sizeof(line), _subMenus[idx]->name());
        }
        else {
            line[0] = '\0';
        }
        _common.screen->printLine(3, line);
    }
}

} // namespace Menu
