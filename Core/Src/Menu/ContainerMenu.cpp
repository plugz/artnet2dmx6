#include "ContainerMenu.hpp"

#include "LiquidCrystalI2C.hpp"
#include "MainMenu.hpp"
#include "UniversesMenu.hpp"

#include <cstdio>

namespace Menu {

enum {
    LINE_BUFF_SIZE = LiquidCrystalI2C::MAX_COLS + 1,
};

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
            snprintf(line, sizeof(line), "  %s", _subMenus[idx]->name());
        }
        else {
            line[0] = '\0';
        }
        _common.screen->printLine(1, line);
    }
    {
        auto idx = _currentMenuIdx;
        snprintf(line, sizeof(line), "> %s", _subMenus[idx]->name());
        _common.screen->printLine(2, line);
    }
    {
        if (_currentMenuIdx != _subMenus.size() - 1) {
            auto idx = (_currentMenuIdx + 1) % _subMenus.size();
            snprintf(line, sizeof(line), "  %s", _subMenus[idx]->name());
        }
        else {
            line[0] = '\0';
        }
        _common.screen->printLine(3, line);
    }
}

template class ContainerMenu<IpMenu, UniversesMenu>;
template class ContainerMenu<UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu, UniverseMenu>;

} // namespace Menu
